cmake_minimum_required(VERSION 3.22)
project(SABT)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_subdirectory(tests)

add_executable(SABT
        main.cpp include/Octree.h include/OctreeNode.h include/Beam.h include/Renderer.h include/ConnectedStack.h source/ConnectedStack.cpp)
