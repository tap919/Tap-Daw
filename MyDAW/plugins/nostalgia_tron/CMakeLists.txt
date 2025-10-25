cmake_minimum_required(VERSION 3.24)
project(nostalgia_tron)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SOURCES src/*.cpp)

# Create plugin library
add_library(nostalgia_tron SHARED ${SOURCES})

# Include directories - use absolute path to parent directory
target_include_directories(nostalgia_tron 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${PROJECT_SOURCE_DIR}/../..
)

# Installation
install(TARGETS nostalgia_tron DESTINATION plugins)
