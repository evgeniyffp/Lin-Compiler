#include <iostream>
#include <sstream>
#include <fstream>

#include "Compiler/Tokenization.h"
#include "Compiler/Parser.h"
#include "Compiler/Generation.h"

auto main(int argc, char** argv) -> int {
    if (argc != 2) {
        std::cerr << "Incorrect usage. Correct usage is...\n";
        std::cerr << "linc `file.lin`\n";
        exit(EXIT_FAILURE);
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::fstream::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
        input.close();
    }

    Core::Compiler::Tokenizer tokenizer(contents);
    const auto tokens = tokenizer.Tokenize();

    Core::Compiler::Parser parser(tokens);
    const auto tree = parser.ParseProgramm();

    if (!tree.has_value()) {
        std::cerr << "Invalid programm!\n";
        exit(EXIT_FAILURE);
    }

    std::fstream outfile("../lc-build/out.asm", std::ios::out);

    if (!outfile.is_open()) {
        std::cerr << "Don\'t can open/create the file!\n";
        exit(EXIT_FAILURE);
    }

    Core::Compiler::Generator generator(tree.value());
    outfile << generator.GenetateProgramm();

    outfile.close();

    if (system("nasm -felf64 ../lc-build/out.asm"))
        exit(EXIT_FAILURE);

    if (system("nasm -felf64 ../stdlib/asm/asmcore.asm"))
        exit(EXIT_FAILURE);

    if (system("clang -c -std=c99 ../stdlib/printf/printf.c -o ../stdlib/printf/printf.o"))
        exit(EXIT_FAILURE);

    if (system("clang -c -std=c99 ../stdlib/allocator/allocate.c -o ../stdlib/allocator/allocate.o"))
        exit(EXIT_FAILURE);

    if (system("ld ../lc-build/out.o ../stdlib/allocator/allocate.o ../stdlib/asm/asmcore.o ../stdlib/printf/printf.o -o ../lc-build/out"))
        exit(EXIT_FAILURE);

    return 0;
}
