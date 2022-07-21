cmake_minimum_required(VERSION 3.22)
project(tests)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        # TODO: Update this; Current version ~1st June
        URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# set(CMAKE_CXX_FLAGS "--coverage")

<<<<<<< HEAD
add_executable(tests
        ExampleTest.cpp
        StackTest.cpp
        TripletTest.cpp
        OctreeTest.cpp
        GJKTest.cpp
        MatTest.cpp
        CameraTest.cpp
        )

target_link_libraries(tests gtest_main gtest core)
=======
add_executable(tests ExampleTest.cpp StackTest.cpp TripletTest.cpp OctreeTest.cpp)
target_link_libraries(tests gtest_main gtest core)

set(CMAKE_CXX_FLAGS "-std=c++20")
set(CMAKE_CXX_STANDARD 20)
set_property(TARGET tests PROPERTY CXX_STANDARD 20)
>>>>>>> 57d2e3786ac6e39968eb4c4f09a501bad7f54e09
