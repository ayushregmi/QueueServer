#ifndef HTTPROUTE_H
#define HTTPROUTE_H

#include <functional>
#include <regex>
#include<string>

#include "Request.h"
#include "Response.h"

using RouteHandler = std::function<void (const Request&, Response&)>;

struct HTTPRoute {
    std::string method;
    std::string prefix;
    RouteHandler handler;
};


#endif