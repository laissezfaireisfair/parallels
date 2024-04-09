cmake_minimum_required(VERSION 3.27)
project(Task2_3)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Task2_3 src/main.cpp
        include/matrix.h
        src/matrix.cpp)
