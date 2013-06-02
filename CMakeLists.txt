cmake_minimum_required(VERSION 2.8)

project(APUE)

set(CMAKE_C_FLAGS "-std=c99 -g")

add_subdirectory(src)
add_subdirectory(utils)