cmake_minimum_required(VERSION 3.22)

project(exe)

set(CMAKE_CXX_STANDARD 20)

add_library(core STATIC
        source/Octree.cpp
        source/OctreeRoot.cpp
        source/Vec3i.cpp
        source/Vec3f.cpp
        source/Vec2f.cpp
        source/Cube.cpp
        source/AlignedRect.cpp
        source/Polygon.cpp
        )

target_include_directories(core PUBLIC include)

add_executable(application main.cpp)
target_link_libraries(application core)
