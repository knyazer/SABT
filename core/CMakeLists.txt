cmake_minimum_required(VERSION 3.22)

project(core)

set(CMAKE_CXX_STANDARD 20)

add_library(core STATIC
        source/Octree.cpp
        source/OctreeRoot.cpp
        source/Vec3i.cpp
        source/Vec3f.cpp
        source/Vec2f.cpp
        source/Angle.cpp
        source/Cube.cpp
        source/AlignedRect.cpp
        source/Polygon.cpp
        source/Camera.cpp
        source/OctreeUnit.cpp
        source/OctreeBase.cpp
        )

target_include_directories(core PUBLIC .)
