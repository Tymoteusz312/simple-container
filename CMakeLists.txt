cmake_minimum_required(VERSION 3.33)

set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simple-container)

add_executable(simple-container src/main.c)
