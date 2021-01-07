cmake_minimum_required(VERSION 3.10)
project(busy_waiting)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread  ${CMAKE_CXX_FLAGS}")


include_directories(
        include
)

add_executable(busy_waiting
        main.cpp
        src/Konto.cpp
        src/Alternierend.cpp
        )
