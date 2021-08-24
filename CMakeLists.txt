cmake_minimum_required(VERSION 3.5)

project(NES-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)


add_executable(NES-emulator
    main.cpp
    bus.cpp
    rom.cpp
    CPU/cpu.cpp
    CPU/cpu_addressing_modes.cpp
    CPU/cpu_green.cpp
    CPU/cpu_red.cpp
    CPU/cpu_blue.cpp
    CPU/cpu_gray.cpp
    PPU/ppu.cpp
     window.cpp
     bitops.h
)


target_link_libraries(NES-emulator PRIVATE SDL2::SDL2)

