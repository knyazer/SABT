cmake_minimum_required(VERSION 3.22)
project(SABT)

enable_testing()

set(CMAKE_CXX_STANDARD 20)

option(ENABLE_DOUBLE_PRECISION "Enable double precision" ON)

add_subdirectory(libccd)
add_subdirectory(core)
add_subdirectory(tests)

add_subdirectory(demo)