#include "Router.h"

void Router::addRoute(const std::string &method,const std::string& prefix,const RouteHandler &handler) {
    this->routes.push_back({method, prefix,handler});
}

HTTPRoute* Router::findRoute(const Request &req) {
    for (auto &route : this->routes) {
        if (route.method == req.method) {
            if (starts_with(req.uri, route.prefix)) {
                return &route;
            }
        }
    }
    return nullptr;
}
