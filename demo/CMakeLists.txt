cmake_minimum_required(VERSION 3.22)

project(demo)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(demo
        main.cpp

        source/Renderer.cpp
        source/Rect.cpp
        )

target_include_directories(demo PUBLIC include)

target_link_libraries(demo ${SDL2_LIBRARIES} SDL2_ttf X11 core)

# Copy folders which required for executable to work
file(COPY models DESTINATION .)
file(COPY font DESTINATION .)