cmake_minimum_required(VERSION 3.12)
project(cpp-marketmaker LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT MSVC)
    add_compile_options(-O3 -march=native)
endif()

# Main
file(GLOB MAIN "main.cpp")

# Create Executables
add_executable(mm ${MAIN})