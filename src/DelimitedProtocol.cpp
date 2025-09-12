#include "DelimitedProtocol.h"
#include "Logger.h"
#include "utils.h"

std::vector<ParsedMessage> DelimitedProtocol::parseData(const std::string &readBuffer_)
{
    prevBuffer += readBuffer_;
    std::vector<ParsedMessage> parsedMessages;
    while (!prevBuffer.empty())
    {
        size_t firstDelim = prevBuffer.find("|");

        if (firstDelim == std::string::npos)
            break;
        std::string cmd = trim(prevBuffer.substr(0, firstDelim));

        size_t secondDelim = prevBuffer.find("|", firstDelim + 1);
        if (secondDelim == std::string::npos)
            break;

        std::string msgLenStr = trim(prevBuffer.substr(firstDelim + 1, secondDelim - firstDelim - 1));
        int msgLen = 0;
        try
        {
            msgLen = std::stoi(msgLenStr);
        }
        catch (...)
        {
            throw std::runtime_error("Protocol Error: invalid length field '" + msgLenStr + "'");
        }

        if (msgLen < 0)
        {
            throw std::runtime_error("Protocol Error: Message length cannot be negative");
        }

        size_t totalMessageLen = secondDelim + 1 + msgLen;

        if (prevBuffer.size() < totalMessageLen)
            break;

        std::string payload = trim(prevBuffer.substr(secondDelim + 1, msgLen));

        prevBuffer = prevBuffer.substr(totalMessageLen);

        ParsedMessage message;
        message.command = cmd;
        message.msgLen = msgLen;
        message.payload = payload;

        parsedMessages.push_back(std::move(message));
    }
    return parsedMessages;
}

void DelimitedProtocol::clear()
{
}

DelimitedProtocol::DelimitedProtocol()
{
    this->prevBuffer = "";
}

DelimitedProtocol::~DelimitedProtocol()
{
}