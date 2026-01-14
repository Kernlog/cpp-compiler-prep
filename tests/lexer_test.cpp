#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace compiler;

TEST(LexerTest, SingleTokens) {
    ErrorReporter errors;
    Lexer lexer("+ - * / ( ) { }", "test", errors);

    EXPECT_EQ(lexer.next_token().kind, TokenKind::Plus);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Minus);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Star);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Slash);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::LParen);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::RParen);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::LBrace);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::RBrace);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Eof);
}

TEST(LexerTest, TwoCharOperators) {
    ErrorReporter errors;
    Lexer lexer("== != <= >= -> && ||", "test", errors);

    EXPECT_EQ(lexer.next_token().kind, TokenKind::EqualEqual);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::BangEqual);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::LessEqual);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::GreaterEqual);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Arrow);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::AmpAmp);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::PipePipe);
}

TEST(LexerTest, Keywords) {
    ErrorReporter errors;
    Lexer lexer("fn let if else while return true false", "test", errors);

    EXPECT_EQ(lexer.next_token().kind, TokenKind::Fn);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Let);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::If);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Else);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::While);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Return);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::True);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::False);
}

TEST(LexerTest, Identifiers) {
    ErrorReporter errors;
    Lexer lexer("foo bar_baz _private x1 y2z", "test", errors);

    Token t1 = lexer.next_token();
    EXPECT_EQ(t1.kind, TokenKind::Identifier);
    EXPECT_EQ(t1.lexeme, "foo");

    Token t2 = lexer.next_token();
    EXPECT_EQ(t2.kind, TokenKind::Identifier);
    EXPECT_EQ(t2.lexeme, "bar_baz");

    Token t3 = lexer.next_token();
    EXPECT_EQ(t3.kind, TokenKind::Identifier);
    EXPECT_EQ(t3.lexeme, "_private");
}

TEST(LexerTest, Numbers) {
    ErrorReporter errors;
    Lexer lexer("42 3.14 100 0.5", "test", errors);

    Token t1 = lexer.next_token();
    EXPECT_EQ(t1.kind, TokenKind::IntLit);
    EXPECT_EQ(t1.lexeme, "42");

    Token t2 = lexer.next_token();
    EXPECT_EQ(t2.kind, TokenKind::FloatLit);
    EXPECT_EQ(t2.lexeme, "3.14");

    Token t3 = lexer.next_token();
    EXPECT_EQ(t3.kind, TokenKind::IntLit);
    EXPECT_EQ(t3.lexeme, "100");

    Token t4 = lexer.next_token();
    EXPECT_EQ(t4.kind, TokenKind::FloatLit);
    EXPECT_EQ(t4.lexeme, "0.5");
}

TEST(LexerTest, Strings) {
    ErrorReporter errors;
    Lexer lexer("\"hello\" \"world\"", "test", errors);

    Token t1 = lexer.next_token();
    EXPECT_EQ(t1.kind, TokenKind::StringLit);
    EXPECT_EQ(t1.lexeme, "\"hello\"");

    Token t2 = lexer.next_token();
    EXPECT_EQ(t2.kind, TokenKind::StringLit);
    EXPECT_EQ(t2.lexeme, "\"world\"");
}

TEST(LexerTest, Comments) {
    ErrorReporter errors;
    Lexer lexer("foo // this is a comment\nbar", "test", errors);

    Token t1 = lexer.next_token();
    EXPECT_EQ(t1.kind, TokenKind::Identifier);
    EXPECT_EQ(t1.lexeme, "foo");

    Token t2 = lexer.next_token();
    EXPECT_EQ(t2.kind, TokenKind::Identifier);
    EXPECT_EQ(t2.lexeme, "bar");
}

TEST(LexerTest, FullStatement) {
    ErrorReporter errors;
    Lexer lexer("let x = 42;", "test", errors);

    EXPECT_EQ(lexer.next_token().kind, TokenKind::Let);

    Token id = lexer.next_token();
    EXPECT_EQ(id.kind, TokenKind::Identifier);
    EXPECT_EQ(id.lexeme, "x");

    EXPECT_EQ(lexer.next_token().kind, TokenKind::Equal);

    Token num = lexer.next_token();
    EXPECT_EQ(num.kind, TokenKind::IntLit);
    EXPECT_EQ(num.lexeme, "42");

    EXPECT_EQ(lexer.next_token().kind, TokenKind::Semicolon);
    EXPECT_EQ(lexer.next_token().kind, TokenKind::Eof);

    EXPECT_FALSE(errors.has_errors());
}

TEST(LexerTest, PeekToken) {
    ErrorReporter errors;
    Lexer lexer("foo bar", "test", errors);

    Token peeked = lexer.peek_token();
    EXPECT_EQ(peeked.kind, TokenKind::Identifier);
    EXPECT_EQ(peeked.lexeme, "foo");

    Token next = lexer.next_token();
    EXPECT_EQ(next.lexeme, "foo");

    EXPECT_EQ(lexer.next_token().lexeme, "bar");
}
