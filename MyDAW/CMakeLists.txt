cmake_minimum_required(VERSION 3.24)
project(MyDAW LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main DAW executable
file(GLOB SRC
  app/*.cpp engine/*.cpp dsp/*.cpp host/*.cpp ui/*.cpp
  pads/*.cpp arrange/*.cpp ab/*.cpp io/*.cpp src/midi/*.cpp
  src/*.cpp)
add_executable(MyDAW ${SRC})
target_include_directories(MyDAW PRIVATE . include)

# Test executable for circadian phase system
add_executable(test_circadian_phase 
  tests/test_circadian_phase.cpp
  src/CircadianPhaseManager.cpp
  src/AlignmentLedger.cpp
  src/TelemetrySystem.cpp)
target_include_directories(test_circadian_phase PRIVATE . include)

# Add plugins subdirectory
add_subdirectory(plugins)
