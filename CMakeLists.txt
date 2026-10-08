cmake_minimum_required(VERSION 3.12)
project(ITCHMarketReplay LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable CMake's built-in testing features (CTest)
include(CTest)

# Fetch Catch2 Framework
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2 # Use a stable Catch2 v3 release tag
)
FetchContent_MakeAvailable(Catch2)

# Main
file(GLOB MAIN "main.cpp")

# Bench
file(GLOB BENCH "bench.cpp")

# Sources
file(GLOB SOURCES "src/*.cpp")

# Tests
file(GLOB TESTS "tests/*.cpp")

# Create Executables
add_executable(itchreplay ${MAIN} ${SOURCES})
add_executable(debug ${MAIN} ${SOURCES})
add_executable(bench ${BENCH} ${SOURCES})
add_executable(bench-debug ${BENCH} ${SOURCES})
add_executable(unit_tests ${SOURCES} ${TESTS})

# Compile and Link Options
if(NOT MSVC)
    target_compile_options(itchreplay PRIVATE -O3 -march=native)
    target_compile_options(debug PRIVATE -fsanitize=thread -g -O1)
    target_compile_options(bench PRIVATE -O3 -march=native)
    target_compile_options(bench-debug PRIVATE -fsanitize=thread -g -O1)
    target_link_options(debug PRIVATE -fsanitize=thread)
    target_link_options(bench-debug PRIVATE -fsanitize=thread)
endif()

# Link against the Catch2 v3 main entry point target
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Automatically register test cases with CTest
include(Catch)
catch_discover_tests(unit_tests)