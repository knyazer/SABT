cmake_minimum_required(VERSION 3.22)

project(exe)

set(CMAKE_CXX_STANDARD 20)

add_library(core SHARED
        source/Beam.cpp)
target_include_directories(core PUBLIC include)

add_executable(application main.cpp)
target_link_libraries(application core)
