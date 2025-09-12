#ifndef DELIMITEDPROTOCOL_H
#define DELIMITEDPROTOCOL_H

#include "Protocol.h"
#include "ParsedMessage.h"

class DelimitedProtocol : public Protocol
{

public:
    DelimitedProtocol();
    ~DelimitedProtocol();
    std::vector<ParsedMessage> parseData(const std::string &buffer);
    void clear();
};

#endif