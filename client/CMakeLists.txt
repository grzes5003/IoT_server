cmake_minimum_required(VERSION 3.19)
project(client C)

set(CMAKE_C_STANDARD 11)

set(
        IOT_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}
        CACHE STRING "IOT Include Directories"
)

add_subdirectory (src)
