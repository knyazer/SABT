cmake_minimum_required(VERSION 3.22)
project(tests)

set(CMAKE_CXX_STANDARD 20)

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

add_executable(tests
        tests.cpp
        ExampleTest.cpp
        StackTest.cpp
        TripletTest.cpp
        OctreeTest.cpp
        GJKTest.cpp
        MatTest.cpp
        CameraTest.cpp
        )

target_link_libraries(tests gtest_main gtest core)
