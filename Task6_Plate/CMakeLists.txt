cmake_minimum_required(VERSION 3.28)
project(Task6_Plate)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -fopenacc)
endif ()

add_executable(Task6_Plate src/main.cpp
        include/Grid.h
        src/Grid.cpp
        include/CmdArguments.h
        include/Application.h
        src/Application.cpp)
