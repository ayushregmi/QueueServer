#ifndef QUEUESERVER_ROUTER_H
#define QUEUESERVER_ROUTER_H

#include <regex>
#include<string>

#include "HTTPRoute.h"

class Router {
    std::vector<HTTPRoute> routes;
    inline bool starts_with(std::string_view s, std::string_view prefix) {
        return s.size() >= prefix.size() &&
               s.compare(0, prefix.size(), prefix) == 0;
    }

public:
    void addRoute(const std::string&,const std::string &,const RouteHandler&);
    HTTPRoute *findRoute(const Request &req);


};


#endif