cmake_minimum_required(VERSION 3.26)
project(Chip-8-emulator)
add_executable(pchip8
  src/main.cpp
  src/chip8.cpp
  src/opcodes.cpp
)
include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 ${SDL2_LIBRARIES})
