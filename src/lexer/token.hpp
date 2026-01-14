#pragma once

#include <string>
#include <string_view>
#include "common/error.hpp"

namespace compiler {

enum class TokenKind {
    // Literals
    Identifier,
    IntLit,
    FloatLit,
    StringLit,

    // Keywords
    Fn,
    Let,
    If,
    Else,
    While,
    Return,
    True,
    False,

    // Operators
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    EqualEqual,
    Bang,
    BangEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    AmpAmp,
    PipePipe,

    // Delimiters
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Comma,
    Semicolon,
    Colon,
    Arrow,

    // Special
    Eof,
    Error
};

struct Token {
    TokenKind kind;
    std::string_view lexeme;
    SourceLocation loc;
};

const char* token_kind_to_string(TokenKind kind);

} // namespace compiler
