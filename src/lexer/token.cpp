#include "token.hpp"

namespace compiler {
    const char* token_kind_to_string (TokenKind kind) {
        switch (kind) {
            case TokenKind::Identifier:   return "Identifier";
            case TokenKind::IntLit:       return "IntLit";
            case TokenKind::FloatLit:     return "FloatLit";
            case TokenKind::StringLit:    return "StringLit";
            case TokenKind::Fn:           return "Fn";
            case TokenKind::Let:          return "Let";
            case TokenKind::If:           return "If";
            case TokenKind::Else:         return "Else";
            case TokenKind::While:        return "While";
            case TokenKind::Return:       return "Return";
            case TokenKind::True:         return "True";
            case TokenKind::False:        return "False";
            case TokenKind::Plus:         return "Plus";
            case TokenKind::Minus:        return "Minus";
            case TokenKind::Star:         return "Star";
            case TokenKind::Slash:        return "Slash";
            case TokenKind::Equal:        return "Equal";
            case TokenKind::EqualEqual:   return "EqualEqual";
            case TokenKind::Bang:         return "Bang";
            case TokenKind::BangEqual:    return "BangEqual";
            case TokenKind::Less:         return "Less";
            case TokenKind::LessEqual:    return "LessEqual";
            case TokenKind::Greater:      return "Greater";
            case TokenKind::GreaterEqual: return "GreaterEqual";
            case TokenKind::AmpAmp:       return "AmpAmp";
            case TokenKind::PipePipe:     return "PipePipe";
            case TokenKind::LParen:       return "LParen";
            case TokenKind::RParen:       return "RParen";
            case TokenKind::LBrace:       return "LBrace";
            case TokenKind::RBrace:       return "RBrace";
            case TokenKind::LBracket:     return "LBracket";
            case TokenKind::RBracket:     return "RBracket";
            case TokenKind::Comma:        return "Comma";
            case TokenKind::Semicolon:    return "Semicolon";
            case TokenKind::Colon:        return "Colon";
            case TokenKind::Arrow:        return "Arrow";
            case TokenKind::Eof:          return "Eof";
            case TokenKind::Error:        return "Error";
        }
        return "Unknown";
    }
} // namespace compiler