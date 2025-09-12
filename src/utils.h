#ifndef UTILS_H
#define UTILS_H

#define MAYBE_UNUSED [[maybe_unused]]

#include <arpa/inet.h>
#include <string>
#include <cctype>
#include <signal.h>

#include "Logger.h"

MAYBE_UNUSED inline std::string get_peer_address(int client_fd)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if (getpeername(client_fd, (struct sockaddr *)&client_addr, &addr_len) == -1)
    {
        Logger::log("Failed to get client address with getpeername()");
        return "";
    }

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str)) == nullptr)
    {
        Logger::log("Failed to convert client IP address");
        return "";
    }

    int port = ntohs(client_addr.sin_port);

    return std::string(ip_str) + ":" + std::to_string(port);
}

MAYBE_UNUSED inline std::string trim(const std::string &str)
{
    size_t first = 0;
    while (first < str.size() && std::isspace(static_cast<unsigned char>(str[first])))
        ++first;

    size_t last = str.size();
    while (last > first && std::isspace(static_cast<unsigned char>(str[last - 1])))
        --last;

    return str.substr(first, last - first);
}

#endif