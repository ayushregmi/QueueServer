#ifndef UTILS_H
#define UTILS_H

#define MAYBE_UNUSED [[maybe_unused]]

#include <arpa/inet.h>
#include <string>
#include <cctype>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

#include "Logger.h"

MAYBE_UNUSED inline std::string get_peer_address(int client_fd)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if (getpeername(client_fd, (struct sockaddr *)&client_addr, &addr_len) == -1)
    {
        Logger::log("get_peer_address()", "Failed to get client address with getpeername()");
        return "";
    }

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str)) == nullptr)
    {
        Logger::log("get_peer_address()", "Failed to convert client IP address");
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

MAYBE_UNUSED inline unsigned long long generateMessageId()
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<unsigned long long> dis;
    return dis(gen);
}

MAYBE_UNUSED inline std::string epochToUTCString(unsigned long long epochSeconds)
{
    std::time_t t = static_cast<std::time_t>(epochSeconds);
    std::tm *utc_tm = std::gmtime(&t);

    std::stringstream ss;
    ss << std::put_time(utc_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

MAYBE_UNUSED inline std::string trim_2(const std::string &s)
{
    size_t start = s.find_first_not_of(" \r\n\t");
    size_t end = s.find_last_not_of(" \r\n\t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

MAYBE_UNUSED inline static std::string toLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

#endif
