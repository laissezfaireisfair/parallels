cmake_minimum_required(VERSION 3.27)
project(Task3_ClientServer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Task3_ClientServer src/main.cpp
        include/Server.h)
