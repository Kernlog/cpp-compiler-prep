// Lexer implementation

#include "lexer.hpp"
#include <cctype>

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

// Identifiers and keywords
Token Lexer::identifier() {
    // Consume all alphanumeric characters and underscores
    while (!is_at_end() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }
    return make_token(identifier_kind());
}

// Check if the identifier we just scanned is a keyword
TokenKind Lexer::identifier_kind() {
    // trie-like approach: check first char, then compare rest
    // this is faster than a hash table for small keyword sets
    
    std::string_view text = source_.substr(start_, current_ - start_);
    
    switch (text[0]) {
        case 'e': if (text == "else") return TokenKind::Else; break;
        case 'f':
            if (text == "fn") return TokenKind::Fn;
            if (text == "false") return TokenKind::False;
            break;
        case 'i': if (text == "if") return TokenKind::If; break;
        case 'l': if (text == "let") return TokenKind::Let; break;
        case 'r': if (text == "return") return TokenKind::Return; break;
        case 't': if (text == "true") return TokenKind::True; break;
        case 'w': if (text == "while") return TokenKind::While; break;
    }
    
    return TokenKind::Identifier;
}

// Numbers
Token Lexer::number() {
    // consume all digits
    while (!is_at_end() && std::isdigit(peek())) {
        advance();
    }
    
    // check for decimal point followed by more digits
    if (peek() == '.' && std::isdigit(peek_next())) {
        advance();  // consume the '.'
        
        while (!is_at_end() && std::isdigit(peek())) {
            advance();
        }
        
        return make_token(TokenKind::FloatLit);
    }
    
    return make_token(TokenKind::IntLit);
}

Token Lexer::string() {
    // we already consumed open quote, now consume until close quote, or end of input
    while (!is_at_end() && peek() != '"') {
        if (peek() == '\n') {
            line_++;
            column_ = 1;
        }
        advance();
    }

    if (is_at_end()) {
        return error_token("Unterminated string");
    }

    advance(); // consume closing quote
    return make_token(TokenKind::StringLit);
}

Token Lexer::next_token() {
    // peeked a token earlier, return it now
    if (peeked_.has_value()) {
        Token t = peeked_.value();
        peeked_.reset();
        return t;
    }
    
    skip_whitespace();
    
    start_ = current_;  // Mark start of new token
    
    if (is_at_end()) {
        return make_token(TokenKind::Eof);
    }
    
    char c = advance();
    
    // identifiers start with letter or underscore
    if (std::isalpha(c) || c == '_') {
        return identifier();
    }
    
    // numbers start with digit
    if (std::isdigit(c)) {
        return number();
    }
    
    // Everything else
    switch (c) {
        // Single-character tokens
        case '(': return make_token(TokenKind::LParen);
        case ')': return make_token(TokenKind::RParen);
        case '{': return make_token(TokenKind::LBrace);
        case '}': return make_token(TokenKind::RBrace);
        case '[': return make_token(TokenKind::LBracket);
        case ']': return make_token(TokenKind::RBracket);
        case ',': return make_token(TokenKind::Comma);
        case ';': return make_token(TokenKind::Semicolon);
        case ':': return make_token(TokenKind::Colon);
        case '+': return make_token(TokenKind::Plus);
        case '*': return make_token(TokenKind::Star);
        
        // Two-character tokens (need to check second char)
        case '-':
            return make_token(match('>') ? TokenKind::Arrow : TokenKind::Minus);
        case '/':
            return make_token(TokenKind::Slash);
        case '=':
            return make_token(match('=') ? TokenKind::EqualEqual : TokenKind::Equal);
        case '!':
            return make_token(match('=') ? TokenKind::BangEqual : TokenKind::Bang);
        case '<':
            return make_token(match('=') ? TokenKind::LessEqual : TokenKind::Less);
        case '>':
            return make_token(match('=') ? TokenKind::GreaterEqual : TokenKind::Greater);
        case '&':
            if (match('&')) return make_token(TokenKind::AmpAmp);
            return error_token("Expected '&&'");
        case '|':
            if (match('|')) return make_token(TokenKind::PipePipe);
            return error_token("Expected '||'");
        
        // String literals
        case '"':
            return string();
    }
    
    return error_token("Unexpected character");
}

Token Lexer::peek_token() {
    if (!peeked_.has_value()) {
        peeked_ = next_token();
    }
    return peeked_.value();
}

} // namespace compiler
