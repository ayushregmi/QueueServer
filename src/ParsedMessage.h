#ifndef PARSEDMESSAGE_H
#define PARSEDMESSAGE_H

#include <string>

struct ParsedMessage
{
    std::string command;
    int msgLen;
    std::string payload;
};

#endif