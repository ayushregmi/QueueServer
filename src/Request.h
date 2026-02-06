#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <unordered_map>

#include "json.h"

struct Request
{
    std::unordered_map<std::string, std::string> headers;
    std::string method;
    std::string uri;

    JSON jsonMessage;
    bool isValid;
    std::string error;
    int errorCode;

    std::string getHeader(const std::string& key) {
        return this->headers.find(key) != headers.end()? this->headers.find(key)->second:"";
    }
};

#endif