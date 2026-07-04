cmake_minimum_required(VERSION 3.33)

set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simple-container)

include_directories(simple-container include)

add_executable(simple-container src/main.c src/dispacher.c src/commands_registry.c src/commands/run.c src/commands/create.c src/opt_parser.c)
