cmake_minimum_required(VERSION 3.14)
project(MathProjectTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest QUIET)

if(GTest_FOUND)
    message(STATUS "Using system Google Test")
else()
    message(STATUS "Downloading Google Test from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

include_directories(${CMAKE_SOURCE_DIR}/..)

add_executable(unit_tests unit_tests.cpp)

if(GTest_FOUND)
    target_link_libraries(unit_tests GTest::gtest GTest::gtest_main)
else()
    target_link_libraries(unit_tests gtest_main)
endif()

enable_testing()
add_test(NAME AllTests COMMAND unit_tests)