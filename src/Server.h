#ifndef SERVER_H
#define SERVER_H

#define SERVER_TIMEOUT_SECONDS 10
#define IDLE_TIMEOUT_SECONDS 100

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <netinet/in.h>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include "ClientHandler.h"
#include "Logger.h"
#include "Request.h"
#include "QueueManager.h"
#include "Router.h"

class ClientHandler;

class Server
{
public:
    explicit Server(int port);
    void run();
    ~Server();

private:
    int serverSocket_;
    int port_;
    int epollfd_;
    bool running_;

    std::unordered_map<int, std::chrono::steady_clock::time_point> clientLastActive;
    std::unique_ptr<QueueManager> queueManager;
    std::unordered_map<int, ClientHandler *> clients;
    std::unique_ptr<Router> router;

    static void signalHandler(int signum);
    void setupServerSocket();
    void setupRoutes() const;
    void setupEpoll();
    void eventLoop();
    void accecptConnection();
    void handleClient(int clientFd, uint32_t events);
    void removeClient(int clientFd);

    static std::mutex sendMutex;
    static Server *instance;

    static void handleClientMessage(ClientHandler *, const std::vector<Request> &message);
};
#endif