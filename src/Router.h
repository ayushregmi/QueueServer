#ifndef QUEUESERVER_ROUTER_H
#define QUEUESERVER_ROUTER_H

#include <regex>
#include<string>

#include "HTTPRoute.h"

class Router {
    std::vector<HTTPRoute> routes;
public:
    void addRoute(const std::string&, const std::regex &,const RouteHandler&);
    HTTPRoute *findRoute(const Request &req, std::smatch &pathMatches);
};


#endif