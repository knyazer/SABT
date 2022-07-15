cmake_minimum_required(VERSION 3.22)

project(gui)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)
add_executable(gui
        main.cpp
        source/Renderer.cpp include/Rect.cpp include/Rect.h)

target_link_libraries(gui ${SDL2_LIBRARIES})