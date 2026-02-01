#include "DelimitedProtocol.h"

std::vector<Request> DelimitedProtocol::parseData(const std::string &readBuffer_)
{
    prevBuffer += readBuffer_;
    std::vector<Request> parsedMessages;
    while (!prevBuffer.empty())
    {
        size_t firstDelim = prevBuffer.find("|");

        if (firstDelim == std::string::npos)
            break;
        std::string msgLenStr = trim(prevBuffer.substr(0, firstDelim));

        int msgLen = 0;
        Request message;

        try
        {

            try
            {
                msgLen = std::stoi(msgLenStr);
            }
            catch (...)
            {
                throw std::runtime_error("Protocol Error: invalid length field '" + msgLenStr + "'");
            }

            if (msgLen <= 0)
            {

                throw std::runtime_error("Protocol Error: Message length cannot be negative");
            }

            if (msgLen > MAX_MSG_LEN)
            {
                throw std::runtime_error("Protocol Error: Message too long");
            }
            size_t totalMessageLen = firstDelim + 1 + msgLen;
            if (prevBuffer.size() < totalMessageLen)
                break;
            std::string payload = prevBuffer.substr(firstDelim + 1, msgLen);
            prevBuffer = prevBuffer.substr(totalMessageLen);
            message.jsonMessage = JSON::loads(payload);
        }
        catch (const std::runtime_error &e)
        {
            message.jsonMessage["error"] = std::string(e.what());
            prevBuffer = prevBuffer.substr(firstDelim + 1);
        }

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