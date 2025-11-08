cmake_minimum_required(VERSION 3.14)
project(pqc_ot VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# ============================================================================
# OpenSSL (for ChaCha20 and AES encryption)
# ============================================================================
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL development libraries.")
endif()

# ============================================================================
# Google Test
# ============================================================================
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2  # Latest stable release
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ============================================================================
# Google Benchmark
# ============================================================================
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.1  # Latest stable release
)

# Don't build benchmark tests
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googlebenchmark)

# ============================================================================
# Project source files
# ============================================================================
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")  # Exclude main.cpp from library

# ============================================================================
# Main executable
# ============================================================================
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

# ============================================================================
# Tests
# ============================================================================
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

if(TEST_SOURCES)
    add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES} ${SOURCES})

    target_include_directories(${PROJECT_NAME}_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OPENSSL_INCLUDE_DIR}
    )

    target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    # Discover tests
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_tests)
endif()

# ============================================================================
# Benchmarks
# ============================================================================
file(GLOB_RECURSE BENCHMARK_SOURCES "benchmark/*.cpp")

if(BENCHMARK_SOURCES)
    add_executable(${PROJECT_NAME}_benchmarks ${BENCHMARK_SOURCES} ${SOURCES})

    target_include_directories(${PROJECT_NAME}_benchmarks PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OPENSSL_INCLUDE_DIR}
    )

    target_link_libraries(${PROJECT_NAME}_benchmarks PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

# ============================================================================
# Compiler warnings
# ============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

# ============================================================================
# Print configuration summary
# ============================================================================
message(STATUS "")
message(STATUS "=== Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenSSL: ${OPENSSL_VERSION}")
message(STATUS "Google Test: Enabled")
message(STATUS "Google Benchmark: Enabled")
message(STATUS "=============================")
message(STATUS "")