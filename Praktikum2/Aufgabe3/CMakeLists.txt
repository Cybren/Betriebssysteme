cmake_minimum_required(VERSION 3.10)
project(blocks_world)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread ${CMAKE_CXX_FLAGS}")

include_directories(
        include
)

add_executable(${PROJECT_NAME}
        main.cpp
        src/Field.cpp
        src/Block.cpp
        src/CubeBlock.cpp
        src/PyramidBlock.cpp
        src/World.cpp
        )
