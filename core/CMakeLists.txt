cmake_minimum_required(VERSION 3.22)

project(core)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES source/*.cpp third-party/*.c third-party/*.cpp)
add_library(core STATIC ${SOURCES})

target_include_directories(core PUBLIC .)

target_link_libraries(core PUBLIC ccd)

if ($<CONFIG:DEBUG>)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(core PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()