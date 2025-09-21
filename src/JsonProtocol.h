#ifndef JSON_PROTOCOL_H
#define JSON_PROTOCOL_H

#include "Protocol.h"
#include "json.h"
#include "Logger.h"

class JsonProtocol : public Protocol
{
public:
    JsonProtocol();
    ~JsonProtocol();
    std::vector<ParsedMessage> parseData(const std::string &buffer);
    void clear();
};

#endif