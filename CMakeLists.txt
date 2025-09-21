cmake_minimum_required(VERSION 3.10)

# Project name and version
project(QueueServer VERSION 1.0 LANGUAGES CXX)

# Require C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source files for the project
set(SOURCES
    src/main.cpp
    src/ClientHandler.cpp
    src/Server.cpp
    src/DelimitedProtocol.cpp
    src/json.cpp
    src/JsonProtocol.cpp
)

# Add executable target named "queue-server"
add_executable(queue-server ${SOURCES})

# Link pthread library (required for threading and some std libs)
find_package(Threads REQUIRED)
target_link_libraries(queue-server PRIVATE Threads::Threads)

# Set compiler warnings for better code quality (optional but recommended)
if (MSVC)
    target_compile_options(queue-server PRIVATE /W4 /permissive-)
else()
    target_compile_options(queue-server PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
