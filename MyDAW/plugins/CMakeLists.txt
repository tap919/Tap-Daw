cmake_minimum_required(VERSION 3.24)
project(MyDAW_Plugins)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add all plugin subdirectories
add_subdirectory(nostalgia_tron)
add_subdirectory(quantum_80)
add_subdirectory(rhythm_composer)
add_subdirectory(velocity_eq)
add_subdirectory(momentum_delay)
add_subdirectory(analytica_vaccine)
add_subdirectory(fractal_remixer)
add_subdirectory(sonnet_composer)

# Installation target
install(DIRECTORY ${CMAKE_BINARY_DIR}/plugins DESTINATION .)
