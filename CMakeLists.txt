cmake_minimum_required(VERSION 3.33)

set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

project(simple-container)

add_library(libsc STATIC 
    src/libsc/runtime.c
)

target_include_directories(runtime PUBLIC ${CMAKE_SOURCE_DIR}/include/runtime)

add_executable(sc 
    src/cli/main.c 
    src/cli/dispacher.c 
    src/cli/commands_registry.c 
    src/cli/commands/run.c 
    src/cli/commands/create.c 
    src/cli/opt_parser.c 
    src/cli/core.c
)

target_include_directories(sc PUBLIC ${CMAKE_SOURCE_DIR}/include/cli ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(sc PRIVATE libsc)
