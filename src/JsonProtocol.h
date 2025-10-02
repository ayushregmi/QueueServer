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
    std::vector<ParsedMessage> parseData(const std::string &buffer) override;
    std::string prepareData(const std::string &buffer) override;
    void clear();
};

#endif