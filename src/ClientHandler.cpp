#include "ClientHandler.h"
#include "Logger.h"
#include "utils.h"
#include "DelimitedProtocol.h"

#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

ClientHandler::ClientHandler(int clientFd, ClientCallback funcPtr) : clientFd_(clientFd), callback(funcPtr)
{
    this->protocolHandler = std::make_shared<DelimitedProtocol>();
    Logger::log("ClientHandler :: Handler created for fd: " + get_peer_address(clientFd_));
}

ClientHandler::~ClientHandler()
{
    Logger::log("ClientHandler :: Closing client fd: " + get_peer_address(this->clientFd_));
    close(this->clientFd_);
}

int ClientHandler::getFd() const
{
    return this->clientFd_;
}

bool ClientHandler::handleRead()
{
    char buffer[1024];
    ssize_t n = read(clientFd_, buffer, sizeof(buffer));
    if (n == 0)
    {
        Logger::log("Client disconnected on fd: " + get_peer_address(clientFd_));
        return false;
    }
    else if (n < 0)
    {
        Logger::log("Read error on fd: " + get_peer_address(clientFd_));
        return false;
    }

    do
    {
        std::string readData(buffer, n);
        std::vector<ParsedMessage> messages = this->protocolHandler->parseData(readData);
        callback(clientFd_, messages);
        n = read(clientFd_, buffer, sizeof(buffer));
    } while (n > 0);
    return true;
}
