#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: compiler <source_file>\n";
        return 1;
    }

    std::cout << "Compiling: " << argv[1] << "\n";

    // TODO: lexer -> parser -> sema -> ir -> opt -> codegen -> vm

    return 0;
}
