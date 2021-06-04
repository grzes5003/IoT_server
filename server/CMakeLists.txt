cmake_minimum_required(VERSION 3.9)
project(server C)

set(CMAKE_C_STANDARD 11)

add_executable(server main.c server.h server.c control.c control.h common.c common.h)
