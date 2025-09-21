#ifndef PARSEDMESSAGE_H
#define PARSEDMESSAGE_H

#include <string>

#include "json.h"

struct ParsedMessage
{
    std::string command;
    int msgLen;
    std::string payload;
    JSON jsonMessage;
};

#endif