cmake_minimum_required(VERSION 3.10)
project(Uebung8A2)
set(CMAKE_CXX_FLAGS " -pthread  ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

add_executable(Uebung8A2 main.cpp Reader.cpp Reader.h Writer.cpp Writer.h)