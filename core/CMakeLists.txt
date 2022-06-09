cmake_minimum_required(VERSION 3.22)

project(exe)

add_library(core STATIC
        include/Triplet.h include/Filling.h include/Octree.h source/Octree.cpp source/OctreeRoot.cpp include/OctreeRoot.h source/Vec3i.cpp include/Vec3i.h)
target_include_directories(core PUBLIC include)

add_executable(application main.cpp)
target_link_libraries(application core)

set(CMAKE_CXX_FLAGS "-std=c++20")
set(CMAKE_CXX_STANDARD 20)
set_property(TARGET core PROPERTY CXX_STANDARD 20)
