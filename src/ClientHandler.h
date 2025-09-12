#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include "Protocol.h"
#include "ParsedMessage.h"

#include <string>
#include <memory>
#include <functional>
#include <vector>

using ClientCallback = std::function<void(int, std::vector<ParsedMessage>)>;

class ClientHandler
{

public:
    explicit ClientHandler(int clientFd, ClientCallback callback);
    ~ClientHandler();
    int getFd() const;
    bool handleRead();

private:
    std::shared_ptr<Protocol> protocolHandler;
    int clientFd_;
    ClientCallback callback;
    std::string readBuffer_;
};

#endif