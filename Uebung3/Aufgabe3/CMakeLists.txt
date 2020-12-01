cmake_minimum_required(VERSION 3.10)
project(scheduler)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread  ${CMAKE_CXX_FLAGS}")

include_directories(
        include
)

add_executable(scheduler
        src/main.cpp
        src/Task.cpp
        src/Scheduler.cpp
        )
