#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>

enum class LogLevel
{
    INFO,
    SUCCESS,
    ERROR,
    DEBUG
};

class Logger
{
public:
    static void log(const std::string &module, const std::string &msg, LogLevel level = LogLevel::INFO)
    {
        std::lock_guard<std::mutex> lock(mtx);

        const char *colorCode;
        const char *levelStr;

        switch (level)
        {
        case LogLevel::INFO:
            colorCode = "\033[0m";
            levelStr = "INFO";
            break;
        case LogLevel::SUCCESS:
            colorCode = "\033[32m";
            levelStr = "SUCCESS";
            break;
        case LogLevel::ERROR:
            colorCode = "\033[31m";
            levelStr = "ERROR";
            break;
        case LogLevel::DEBUG:
            colorCode = "\033[36m";
            levelStr = "DEBUG";
            break;
        default:
            colorCode = "\033[0m";
            levelStr = "";
        }

        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

        std::cout << colorCode
                  << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X")
                  << "." << std::setfill('0') << std::setw(3) << ms.count() << "] "
                  << "[" << levelStr << "] "
                  << "[" << module << "] "
                  << msg
                  << "\033[0m" << std::endl;
    }

private:
    static std::mutex mtx;
};

inline std::mutex Logger::mtx;

#endif