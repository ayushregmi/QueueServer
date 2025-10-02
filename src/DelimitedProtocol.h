#ifndef DELIMITEDPROTOCOL_H
#define DELIMITEDPROTOCOL_H
#define MAX_MSG_LEN 1024

#include "Logger.h"
#include "ParsedMessage.h"
#include "Protocol.h"
#include "utils.h"

class DelimitedProtocol : public Protocol
{

public:
    DelimitedProtocol();
    ~DelimitedProtocol() override;
    std::vector<ParsedMessage> parseData(const std::string &buffer) override;
    void clear() override;
};

#endif