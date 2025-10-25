cmake_minimum_required(VERSION 3.24)
project(rhythm_composer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SOURCES src/*.cpp)

# Create plugin library
add_library(rhythm_composer SHARED ${SOURCES})

# Include directories - use absolute path to parent directory
target_include_directories(rhythm_composer 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${PROJECT_SOURCE_DIR}/../..
)

# Installation
install(TARGETS rhythm_composer DESTINATION plugins)
