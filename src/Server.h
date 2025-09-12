#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include "ParsedMessage.h"

class ClientHandler;

class Server
{
public:
    explicit Server(int port);
    void run();
    ~Server();

private:
    static Server *instance;
    int serverSocket_;
    int port_;
    int epollfd_;
    bool running_;
    std::unordered_map<int, ClientHandler *> clients;
    static void signalHandler(int signum);
    void setupServerSocket();
    void setupEpoll();
    void eventLoop();
    void accecptConnection();
    void handleClient(int clientFd, uint32_t events);
    void removeClient(int clientFd);
    static void handleClientMessage(int clientFd, const std::vector<ParsedMessage> &message);
};
#endif