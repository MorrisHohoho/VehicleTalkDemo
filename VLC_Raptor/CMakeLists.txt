cmake_minimum_required(VERSION 3.27)
project(VLC_Raptor)
SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 11)

add_executable(VLC_Raptor main.cpp
        CRC.h)
