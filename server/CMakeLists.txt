cmake_minimum_required(VERSION 3.19)
project(server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread" )

set(
        IOT_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}
        CACHE STRING "IOT Include Directories"
)

add_subdirectory (src)

enable_testing()
add_subdirectory(tests)
