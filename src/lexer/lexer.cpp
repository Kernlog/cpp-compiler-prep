// Lexer implementation

#include "lexer.hpp"

namespace compiler {

// constructor - stores ref and init pos
Lexer::Lexer(std::string_view source, std::string_view filename, ErrorReporter& errors) 
    : source_(source)
    , filename_(filename)
    , errors_(errors)
{}

// have we consumed all source code?
bool Lexer::is_at_end() const {
    return current_ >= source_.size();
}

// look at current char without consuming
char Lexer::peek() const {
    if (is_at_end()) return '\0';
    return source_[current_];
}

// look at next char without consuming (needed for == !=)
char Lexer::peek_next() const {
    if (current_ + 1 >= source_.size()) return '\0';
    return source_[current_ + 1];
}

// consume curr char and reutrn it, advance cursor
char Lexer:: advance() {
    char c = source_[current_];
    current_++;
    column_++;
    return c;
}



} // namespace compiler
