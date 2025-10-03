#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include "DelimitedProtocol.h"
#include "Logger.h"
#include "json.h"
#include "JsonProtocol.h"
#include "ParsedMessage.h"
#include "Protocol.h"
#include "utils.h"

#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <errno.h>
#include <thread>
#include <chrono>

class ClientHandler;

class ClientHandler
{
private:
    using ClientCallback = std::function<void(ClientHandler *, std::vector<ParsedMessage>)>;

public:
    explicit ClientHandler(int clientFd, ClientCallback callback);
    ~ClientHandler();
    int getFd() const;
    bool handleRead();
    void handleSend(const std::string &message);
    void sendTimeoutMessage();

private:
    std::shared_ptr<Protocol> protocolHandler;
    int clientFd_;
    ClientCallback callback;
    std::string readBuffer_;
};

#endif