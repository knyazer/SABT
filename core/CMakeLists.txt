cmake_minimum_required(VERSION 3.22)

project(core)

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

<<<<<<< HEAD
target_include_directories(core PUBLIC .)
=======
add_executable(application main.cpp)
target_link_libraries(application core)

set(CMAKE_CXX_FLAGS "-std=c++20")
set(CMAKE_CXX_STANDARD 20)
set_property(TARGET core PROPERTY CXX_STANDARD 20)
>>>>>>> 57d2e3786ac6e39968eb4c4f09a501bad7f54e09
