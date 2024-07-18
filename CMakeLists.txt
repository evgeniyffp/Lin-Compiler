cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
project(Test)
add_executable(Test
        stdlib/test.c
        stdlib/printf/printf.c
        stdlib/printf/printf.h
        stdlib/asm/asmcore.o
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
        src/Compiler/Variable.h
        src/Compiler/VectorHasher.h
        src/Compiler/Functions.h
)
target_link_libraries(Compiler stdc++)
