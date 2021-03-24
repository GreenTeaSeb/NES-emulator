cmake_minimum_required(VERSION 3.5)

project(NESEmulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(NESEmulator
    main.cpp
    cpu.cpp
    cpu_addressing_modes.cpp
    cpu_green.cpp
    cpu_red.cpp
    cpu_blue.cpp
    cpu_gray.cpp
    )


