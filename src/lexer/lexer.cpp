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

// conditional advance, only consume if curr char matches expected
bool Lexer::match (char expected) {
    if (is_at_end()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    column_++;
    return true;
}

// create token from chars between start_ and current_
Token Lexer::make_token(TokenKind kind) {
    std::string_view lexeme = source_.substr(start_, current_ - start_);
    return Token{kind, lexeme, SourceLocation{filename_, line_, column_}};
}

// err token and report err
Token Lexer::error_token(const char* message) {
    errors_.report(ErrorKind::Lexer, SourceLocation{filename_, line_, column_}, message);
    return Token{TokenKind::Error, std::string_view{}, SourceLocation{filename_, line_, column_}};
}

// whitespace and comments
void Lexer::skip_whitespace() {
    while (!is_at_end()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                line_++;
                column_ = 1;
                advance();
                break;
            case '/':
                // Check for // comment
                if (peek_next() == '/') {
                    skip_line_comment();
                } else {
                    return;  // It's a division operator, not a comment
                }
                break;
            default:
                return;  // Not whitespace, stop skipping
        }
    }
}

void Lexer::skip_line_comment() {
    // Consume until end of line
    while (!is_at_end() && peek() != '\n') {
        advance();
    }
}


} // namespace compiler
