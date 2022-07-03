cmake_minimum_required(VERSION 3.22)

project(demo)

set(CMAKE_CXX_STANDARD 20)

add_library(gui STATIC
        source/Renderer.cpp
        )

target_include_directories(gui PUBLIC include)

add_executable(demo main.cpp)