cmake_minimum_required(VERSION 3.18)
project(server C)

set(CMAKE_C_STANDARD 11)
if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lws2_32 -lwsock32" )
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread" )
endif ()

set(
        IOT_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}
        CACHE STRING "IOT Include Directories"
)

add_subdirectory (src)

enable_testing()
add_subdirectory(tests)
