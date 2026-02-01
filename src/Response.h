#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <unordered_map>

#include "json.h"

struct Response
{
    std::unordered_map<std::string, std::string> headers;
    unsigned int statusCode;
    JSON body;
};

#endif