cmake_minimum_required(VERSION 3.33)

set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

project(simple-container)

add_library(libsc STATIC 
    libsc/src/runtime.c
    libsc/src/image.c
    libsc/src/storage.c
    libsc/src/utils.c
    libsc/src/fsystem.c
    libsc/src/list.c
    libsc/src/inspect.c
)

target_include_directories(libsc PUBLIC ${CMAKE_SOURCE_DIR}/libsc/include/libsc)

add_executable(sc 
    cli/src/main.c 
    cli/src/dispacher.c 
    cli/src/commands_registry.c 
    cli/src/commands/run.c 
    cli/src/commands/create.c 
    cli/src/commands/list.c
    cli/src/commands/inspect.c
    cli/src/opt_parser.c 
    cli/src/core.c
)

target_include_directories(sc PUBLIC ${CMAKE_SOURCE_DIR}/cli/include ${CMAKE_SOURCE_DIR}/libsc/include)

target_link_libraries(sc PRIVATE libsc)
