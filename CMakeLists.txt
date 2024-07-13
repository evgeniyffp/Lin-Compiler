cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
project(Allocator)
add_executable(Allocator
        stdlib/allocator/test.c
        stdlib/printf/test.c
)

set(CMAKE_CXX_STANDARD 20)
project(Compiler)
add_executable(Compiler
        src/Compiler/Tokenization.cpp
        src/Compiler/Tokenization.h
        src/main.cpp
        src/Compiler/Parser.h
        src/Compiler/Parser.cpp
        src/Compiler/Generation.cpp
        src/Compiler/Generation.h
        src/Allocator/ArenaAllocator.cpp
        src/Allocator/ArenaAllocator.h
)
target_link_libraries(Compiler stdc++)

project(Printf)
add_executable(Printf
        stdlib/printf/test.c
)