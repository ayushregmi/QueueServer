#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>

#include "Request.h"
#include "Response.h"

class Protocol
{
protected:
    std::string prevBuffer;

public:
    virtual ~Protocol() {};
    virtual void clear() = 0;
    virtual std::vector<Request> parseData(const std::string &buffer) = 0;
    virtual std::string prepareData(const std::string &buffer) = 0;
    virtual std::string prepareResponse(const Response &response) = 0;
};

#endif