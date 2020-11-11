cmake_minimum_required(VERSION 3.10)
project(buecherei)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        include
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Book.cpp
        src/Shelf.cpp
        src/Library.cpp
        src/Visitor.cpp
        )
