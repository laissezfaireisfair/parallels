cmake_minimum_required(VERSION 3.28)
project(Task6_Plate)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_executable(Task6_Plate src/main.cpp)
