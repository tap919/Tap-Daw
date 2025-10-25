cmake_minimum_required(VERSION 3.24)
project(fractal_remixer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SOURCES src/*.cpp)

# Create plugin library
add_library(fractal_remixer SHARED ${SOURCES})

# Include directories - use absolute path to parent directory
target_include_directories(fractal_remixer 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${PROJECT_SOURCE_DIR}/../..
)

# Installation
install(TARGETS fractal_remixer DESTINATION plugins)
