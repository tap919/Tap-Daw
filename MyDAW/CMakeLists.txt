cmake_minimum_required(VERSION 3.24)
project(MyDAW LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB SRC
  app/*.cpp engine/*.cpp dsp/*.cpp host/*.cpp ui/*.cpp
  pads/*.cpp arrange/*.cpp ab/*.cpp io/*.cpp src/midi/*.cpp)
add_executable(MyDAW ${SRC})
target_include_directories(MyDAW PRIVATE . include)
