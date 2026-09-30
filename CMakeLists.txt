cmake_minimum_required(VERSION 3.12)
project(cpp-marketmaker LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimise Build
if(NOT MSVC)
    add_compile_options(-O3 -march=native)
endif()

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

# Sources
file(GLOB SOURCES "src/*.cpp")

# Tests
file(GLOB TESTS "tests/*.cpp")

# Create Executables
add_executable(mm ${MAIN} ${SOURCES})
add_executable(unit_tests ${SOURCES} ${TESTS})

# Link against the Catch2 v3 main entry point target
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Automatically register test cases with CTest
include(Catch)
catch_discover_tests(unit_tests)