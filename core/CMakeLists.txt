cmake_minimum_required(VERSION 3.22)

project(core)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES source/*.cpp)
add_library(core STATIC ${SOURCES})

target_include_directories(core PUBLIC .)

target_link_libraries(core PUBLIC ccd)
