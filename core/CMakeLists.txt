cmake_minimum_required(VERSION 3.22)

project(core)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES source/*)

add_library(core STATIC ${SOURCES})

target_include_directories(core PUBLIC .)
