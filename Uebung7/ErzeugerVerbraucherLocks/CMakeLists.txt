cmake_minimum_required(VERSION 3.10)
project(ErzeugerVerbraucherLocks)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread  ${CMAKE_CXX_FLAGS}")

include_directories(
        include
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Konditor.cpp
        src/Verbraucher.cpp
        )