#include "JsonProtocol.h"

JsonProtocol::JsonProtocol() {}

JsonProtocol::~JsonProtocol() {}

std::vector<ParsedMessage> JsonProtocol::parseData(const std::string &buffer)
{
    prevBuffer += buffer;
    std::vector<ParsedMessage> messages;
    while (!prevBuffer.empty())
    {
        size_t msgStart = prevBuffer.find("<start>");
        if (msgStart == std::string::npos)
            break;
        size_t msgEnd = prevBuffer.find("<end>");
        if (msgEnd == std::string::npos)
            break;
        std::string message = prevBuffer.substr(msgStart + 7, msgEnd - msgStart - 7);
        prevBuffer = prevBuffer.substr(msgEnd);

        ParsedMessage parsedMessage;
        parsedMessage.jsonMessage = JSON::loads(message);
        messages.push_back(parsedMessage);
    }
    return messages;
}

void JsonProtocol::clear()
{
}
