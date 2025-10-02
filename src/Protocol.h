#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>

#include "ParsedMessage.h"

class Protocol
{
protected:
    std::string prevBuffer;

public:
    virtual ~Protocol() = default;
    virtual void clear() = 0;
    virtual std::vector<ParsedMessage> parseData(const std::string &buffer) = 0;
    virtual std::string prepareData(const std::string &buffer) = 0;
};

#endif