cmake_minimum_required(VERSION 3.16)
project(compiler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type defaults to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Main compiler library (all components link into this)
add_library(compiler_lib STATIC
    src/common/error.cpp
    src/lexer/token.cpp
    src/lexer/lexer.cpp
)

# Main executable
add_executable(compiler src/main.cpp)
target_link_libraries(compiler PRIVATE compiler_lib)

# Testing
if(BUILD_TESTS)
    enable_testing()
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(compiler_tests
        tests/test_main.cpp
        tests/lexer_test.cpp
    )
    target_link_libraries(compiler_tests PRIVATE compiler_lib GTest::gtest GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(compiler_tests)
endif()

# Benchmarks
if(BUILD_BENCHMARKS)
    include(FetchContent)

    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googlebenchmark)

    add_executable(compiler_benchmarks
        benchmarks/bench_main.cpp
        benchmarks/lexer_bench.cpp
    )
    target_link_libraries(compiler_benchmarks PRIVATE compiler_lib benchmark::benchmark)
endif()
