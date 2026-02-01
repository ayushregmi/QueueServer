#ifndef JSON_PROTOCOL_H
#define JSON_PROTOCOL_H

#include "Protocol.h"
#include "json.h"
#include "Logger.h"
#include "utils.h"

class JsonProtocol : public Protocol
{
public:
    JsonProtocol();
    ~JsonProtocol();
    std::vector<Request> parseData(const std::string &buffer) override;
    std::string prepareData(const std::string &buffer) override;
    std::string prepareResponse(MAYBE_UNUSED const Response &response) override{return "";};
    void clear() override;
};

#endif