// Lexer interface
//
// The lexer takes source code as input and produces tokens on demand.
// It should be a "pull" model - caller asks for next token, lexer returns it.

#pragma once

#include <string_view>
#include <optional>
#include "token.hpp"
#include "common/error.hpp"

namespace compiler {
class Lexer {
public:
    Lexer(std::string_view source, std::string_view filename, ErrorReporter& errors);

    Token next_token();
    Token peek_token();

private:
    char advance();
    char peek() const;
    char peek_next() const;
    bool is_at_end() const;
    bool match(char expected);

    void skip_whitespace();
    void skip_line_comment();
    Token make_token(TokenKind kind);
    Token error_token(const char* message);

    Token identifier();
    Token number();
    Token string();

    TokenKind check_keyword(size_t start, std::string_view rest, TokenKind kind);
    TokenKind identifier_kind();

    std::string_view source_;
    std::string_view filename_;
    ErrorReporter& errors_;

    size_t start_ = 0;
    size_t current_ = 0;
    uint32_t line_ = 1;
    uint32_t column_ = 1;

    std::optional<Token> peeked_;
};
}