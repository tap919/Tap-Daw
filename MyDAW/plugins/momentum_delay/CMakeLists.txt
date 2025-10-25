cmake_minimum_required(VERSION 3.24)
project(momentum_delay)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SOURCES src/*.cpp)

# Create plugin library
add_library(momentum_delay SHARED ${SOURCES})

# Include directories - use absolute path to parent directory
target_include_directories(momentum_delay 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${PROJECT_SOURCE_DIR}/../..
)

# Installation
install(TARGETS momentum_delay DESTINATION plugins)
