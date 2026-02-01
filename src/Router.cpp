#include "Router.h"

void Router::addRoute(const std::string &method, const std::regex &regex, const RouteHandler &handler) {
    this->routes.push_back({method, regex, handler});
}

HTTPRoute* Router::findRoute(const Request &req, std::smatch &pathMatches) {
    for (auto &route : this->routes) {
        if (route.method == req.method) {
            if (std::regex_match(req.uri, pathMatches, route.pattern)) {
                return &route;
            }
        }
    }
    return nullptr;
}
