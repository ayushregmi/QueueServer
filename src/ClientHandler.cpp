#include "ClientHandler.h"

ClientHandler::ClientHandler(int clientFd, ClientCallback funcPtr) : clientFd_(clientFd), callback(funcPtr)
{
    this->protocolHandler = std::make_shared<JsonProtocol>();
    Logger::log("ClientHandler", "Handler created for fd: " + get_peer_address(clientFd_));
}

ClientHandler::~ClientHandler()
{
    Logger::log("Client Handler", "Closing client fd: " + get_peer_address(this->clientFd_));
    close(this->clientFd_);
}

int ClientHandler::getFd() const
{
    return this->clientFd_;
}

bool ClientHandler::handleRead()
{
    char buffer[4096];
    while (true)
    {
        ssize_t n = read(clientFd_, buffer, sizeof(buffer));

        if (n > 0)
        {
            std::vector<ParsedMessage> messages;
            std::string readData(buffer, n);
            try
            {
                messages = this->protocolHandler->parseData(readData);
            }
            catch (const std::exception &e)
            {
                Logger::log("ClientHandler", "Fatal parsing error on fd: " +
                                                 get_peer_address(clientFd_) +
                                                 " (" + e.what() + ")");
                return false; // disconnect client
            }
            callback(this, messages);
        }
        else if (n == 0)
        {
            Logger::log("ClientHandler", "Client disconnected on fd: " + get_peer_address(clientFd_));
            return false;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else
            {
                Logger::log("ClientHandler", "Read error on fd: " + get_peer_address(clientFd_) + " (" + strerror(errno) + ")");
                return false;
            }
        }
    }
    return true;
}

void ClientHandler::handleSend(const std::string &message)
{
    std::string formattedMessage = protocolHandler->prepareData(message);
    size_t totalBytesSent = 0;
    size_t messageLen = formattedMessage.length();
    const char *data = formattedMessage.c_str();

    while (totalBytesSent < messageLen)
    {
        ssize_t n = send(this->clientFd_, data + totalBytesSent, messageLen - totalBytesSent, 0);

        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // Non-blocking socket is temporarily unavailable
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            else
            {
                Logger::log("Server", "Unable to send data to: " + get_peer_address(this->clientFd_) +
                                          " (" + std::strerror(errno) + ")");
                return;
            }
        }

        if (n == 0)
        {
            Logger::log("Server", "Connection closed while sending data to: " + get_peer_address(this->clientFd_));
            return;
        }
        totalBytesSent += static_cast<size_t>(n);
    }
}
