#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

namespace compiler {

struct SourceLocation {
    std::string_view file;
    uint32_t line = 1;
    uint32_t column = 1;
};

enum class ErrorKind {
    Lexer,
    Parser,
    Semantic,
    Codegen,
    Runtime
};

struct Error {
    ErrorKind kind;
    SourceLocation loc;
    std::string message;
};

class ErrorReporter {
public:
    void report(ErrorKind kind, SourceLocation loc, std::string message);
    void report(ErrorKind kind, std::string message);

    [[nodiscard]] bool has_errors() const { return !errors_.empty(); }
    [[nodiscard]] const std::vector<Error>& errors() const { return errors_; }

    void clear() { errors_.clear(); }
    void print_all() const;

private:
    std::vector<Error> errors_;
};

} // namespace compiler
