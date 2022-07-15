cmake_minimum_required(VERSION 3.22)

project(gui)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_executable(gui
        main.cpp
        source/Renderer.cpp)