#include "error.hpp"
#include <iostream>

namespace compiler {

void ErrorReporter::report(ErrorKind kind, SourceLocation loc, std::string message) {
    errors_.push_back(Error{kind, loc, std::move(message)});
}

void ErrorReporter::report(ErrorKind kind, std::string message) {
    errors_.push_back(Error{kind, SourceLocation{}, std::move(message)});
}

void ErrorReporter::print_all() const {
    for (const auto& err : errors_) {
        const char* kind_str = nullptr;
        switch (err.kind) {
            case ErrorKind::Lexer:    kind_str = "lexer"; break;
            case ErrorKind::Parser:   kind_str = "parser"; break;
            case ErrorKind::Semantic: kind_str = "semantic"; break;
            case ErrorKind::Codegen:  kind_str = "codegen"; break;
            case ErrorKind::Runtime:  kind_str = "runtime"; break;
        }

        if (!err.loc.file.empty()) {
            std::cerr << err.loc.file << ":" << err.loc.line << ":" << err.loc.column << ": ";
        }
        std::cerr << kind_str << " error: " << err.message << "\n";
    }
}

} // namespace compiler
