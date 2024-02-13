cmake_minimum_required(VERSION 3.27)
project(Task1_Sin)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_executable(Task1_Sin main.cpp)
add_executable(Task1_SinApprox main.cpp)
target_compile_definitions(Task1_SinApprox PUBLIC APPROX)
