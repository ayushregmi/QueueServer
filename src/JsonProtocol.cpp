#include "JsonProtocol.h"

JsonProtocol::JsonProtocol() {}

JsonProtocol::~JsonProtocol() {}

std::vector<Request> JsonProtocol::parseData(const std::string &buffer)
{
    prevBuffer += buffer;
    std::vector<Request> messages;
    while (!prevBuffer.empty())
    {
        size_t msgStart = prevBuffer.find("<start>");
        if (msgStart == std::string::npos)
            break;
        size_t msgEnd = prevBuffer.find("<end>", msgStart + 7);
        if (msgEnd == std::string::npos)
            break;
        std::string message = prevBuffer.substr(msgStart + 7, msgEnd - msgStart - 7);
        prevBuffer = prevBuffer.substr(msgEnd + 5);

        Request parsedMessage;
        try
        {
            parsedMessage.jsonMessage = JSON::loads(message);
            parsedMessage.isValid = true;
        }
        catch (const std::runtime_error &e)
        {
            parsedMessage.isValid = false;
            parsedMessage.error = std::string(e.what());
            parsedMessage.jsonMessage["error"] = std::string(e.what());
        }
        messages.push_back(parsedMessage);
    }
    return messages;
}

std::string JsonProtocol::prepareData(const std::string &buffer)
{
    return "<start>" + buffer + "<end>";
}

void JsonProtocol::clear()
{
}
