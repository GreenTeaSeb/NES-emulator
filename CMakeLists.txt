cmake_minimum_required(VERSION 3.5)

project(NES-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



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
)

