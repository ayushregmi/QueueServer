#include "Server.h"

constexpr int MAX_CLIENTS = 64;
Server *Server::instance = nullptr;
std::mutex Server::sendMutex;


std::string extractQueueName(const std::string &uri) {
    // assumes /queue/{name}
    size_t lastSlash = uri.find_last_of('/');
    return (lastSlash == std::string::npos) ? "" : uri.substr(lastSlash + 1);
}
void Server::signalHandler(int signum)
{
    std::string signalName = strsignal(signum);
    Logger::log("Server", "Caught signal " + std::to_string(signum) + " (" + signalName + "), shutting down...", LogLevel::SUCCESS);

    if (Server::instance)
    {
        Server::instance->running_ = false;
    }
}

Server::Server(int port) : serverSocket_(-1), port_(port), epollfd_(-1), running_(true), queueManager(std::make_unique<QueueManager>()), router(std::make_unique<Router>())
{
}

Server::~Server()
{
    close(this->serverSocket_);
    close(this->epollfd_);
    for (auto &[fd, handler] : this->clients)
    {
        delete handler;
    }
    Logger::log("Server", "Shutting down server...", LogLevel::SUCCESS);
}

void Server::run()
{
    try
    {
        Server::instance = this;
        this->setupRoutes();
        this->setupServerSocket();
        this->setupEpoll();
        this->eventLoop();
    }
    catch (const std::exception &e)
    {
        Logger::log("Server", e.what(), LogLevel::ERROR);
        if (Server::instance)
            Server::instance->running_ = false;
    }
}

void Server::setupRoutes() const {

    this->router->addRoute(
        "GET",
        std::regex(R"(^/queue/([^/]+)$)"),
        [this](const Request& request, Response & response) {
            std::string getQueueName = extractQueueName(request.uri);
            if (getQueueName.empty()) {
                response.body["error"] = "Queue name cannot be empty";
                response.statusCode = 404;
            }else {
                response.statusCode = this->queueManager->getMessagesFromQueue(getQueueName, response.body);
            }
        }
    );

    this->router->addRoute(
        "POST",
        std::regex(R"(^/queue/([^/]+)$)"),
        [this] (const Request& request, Response & response) {
            std::string queueName = extractQueueName(request.uri);
            if  (queueName.empty()) {
                response.body["error"] = "Queue name cannot be empty";
                response.statusCode = 404;
            }else {
                if (request.jsonMessage.is_null()) {
                    response.statusCode = this->queueManager->createNewQueue(queueName, response.body);
                }else {
                    response.statusCode = this->queueManager->addMessageToQueue(queueName, request.jsonMessage, response.body);
                }
            }
        }
    );

    this->router->addRoute(
        "DELETE",
        std::regex(R"(^/queue/([^/]+)$)"),
        [this] (const Request& request, Response & response) {
            std::string queueName = extractQueueName(request.uri);
            if ( queueName.empty()) {
                response.body["error"] = "Queue name cannot be empty";
                response.statusCode = 404;
            }else {
                response.statusCode = this->queueManager->deleteMessageFromQueue(queueName,request.jsonMessage, response.body);
            }
        }
    );
}

void Server::setupServerSocket()
{
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0)
    {
        perror("socket");
        exit(1);
    }
    int opt = 1;
    setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket_, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    if (listen(serverSocket_, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(1);
    }
    fcntl(serverSocket_, F_SETFL, O_NONBLOCK);
    Logger::log("Server", "socket initialized on port " + std::to_string(port_), LogLevel::SUCCESS);
}

void Server::setupEpoll()
{
    epollfd_ = epoll_create1(0);
    if (epollfd_ < 0)
    {
        perror("epoll_create1");
        exit(1);
    }
    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = serverSocket_;
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, serverSocket_, &ev))
    {
        perror("epoll_ctl: serverSocket");
        exit(0);
    }
    Logger::log("Server", "Epoll setup completed", LogLevel::SUCCESS);
}

void Server::eventLoop()
{
    epoll_event events[MAX_CLIENTS];
    Logger::log("Server", "Starting event loop", LogLevel::SUCCESS);
    signal(SIGINT, Server::signalHandler);
    Logger::log("Server", "Press Ctrl+C to stop the server");

    auto lastIdleCheck = std::chrono::steady_clock::now();

    while (running_)
    {
        int n = epoll_wait(epollfd_, events, MAX_CLIENTS, 1000);
        if (n < 0)
        {
            if (errno == EINTR)
            {
                break;
            }
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < n; i++)
        {
            int fd = events[i].data.fd;
            uint32_t ev = events[i].events;
            if (fd == serverSocket_)
            {
                accecptConnection();
            }
            else
            {
                handleClient(fd, ev);
            }
        }

        auto now = std::chrono::steady_clock::now();
        if (now - lastIdleCheck >= std::chrono::seconds(std::chrono::seconds(SERVER_TIMEOUT_SECONDS)))
        {
            std::vector<int> toDisconnect;

            for (const auto &p : clientLastActive)
            {
                auto idleDuration = now - p.second;

                if (idleDuration > std::chrono::seconds(IDLE_TIMEOUT_SECONDS))
                {
                    toDisconnect.push_back(p.first);
                }
            }

            for (int fd : toDisconnect)
            {
                if (clients.find(fd) != clients.end())
                {
                    clients[fd]->sendTimeoutMessage();
                    removeClient(fd);
                }
            }
            lastIdleCheck = now;
        }
    }
}

void Server::accecptConnection()
{
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(serverSocket_, (sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0)
    {
        perror("accept");
        return;
    }
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = clientFd;
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, clientFd, &ev) < 0)
    {
        perror("epoll_ctl: clientFd");
        close(clientFd);
        return;
    }
    clients[clientFd] = new ClientHandler(clientFd, Server::handleClientMessage);
    clientLastActive[clientFd] = std::chrono::steady_clock::now();
}

void Server::handleClient(int clientFd, uint32_t events)
{
    auto it = clients.find(clientFd);
    if (it == clients.end())
        return;

    if (events & EPOLLIN)
    {
        clientLastActive[clientFd] = std::chrono::steady_clock::now();
    }

    if (events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
    {
        Logger::log("Server", "Client fd " + get_peer_address(clientFd) + " disconnected or errored.");
        removeClient(clientFd);
        clientLastActive.erase(clientFd);
        return;
    }
    if (events & EPOLLIN)
    {
        if (!it->second->handleRead())
        {
            removeClient(clientFd);
            clientLastActive.erase(clientFd);
        }
    }
}

void Server::removeClient(int clientFd)
{
    Logger::log("Server", "Disconnecting client " + get_peer_address(clientFd));
    epoll_ctl(epollfd_, EPOLL_CTL_DEL, clientFd, nullptr);
    delete clients[clientFd];
    clients.erase(clientFd);
    clientLastActive.erase(clientFd);
}

void Server::handleClientMessage(ClientHandler *clientHandler, const std::vector<Request> &messages)
{
    for (const auto &msg : messages)
    {
        Response resp;
        try
        {
            if (!msg.isValid)
            {
                resp.body["Error"] = msg.error;
                resp.statusCode = 400;
            }
            else
            {
                std::smatch pathMatch;
                HTTPRoute* route = instance->router->findRoute(msg, pathMatch);

                if (!route) {
                    resp.statusCode = 400;
                    resp.body["Error"] = "Route not found";
                }else {
                    JSON messageContent = msg.jsonMessage;
                    route->handler(msg, resp);
                }
            }
        }
        catch (std::runtime_error &err)
        {
            // Logger::log("Server", "Invalid Message from " + get_peer_address(clientFd) + ". What: " + err.what() + ". Received Message: " + msg.jsonMessage.dump());
            const std::string errorMessage = err.what();
            resp.body["error"] = errorMessage;
            resp.statusCode = 500;
        }
        clientHandler->handleSend(resp);
    }
}
