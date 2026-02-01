#ifndef HTTP_PROTOCOL_H
#define HTTP_PROTOCOL_H

#include "Response.h"
#include"Protocol.h"
#include "utils.h"

class HTTPProtocol: public Protocol
{
public:
    std::vector<Request>parseData(const std::string &buffer) override;
    std::string prepareResponse(const Response &response) override;
    static std::string getStatusMessage(unsigned int code);

    std::string prepareData(const std::string &buffer) override{
        return buffer;
    };
    void clear() override{};
};

#endif