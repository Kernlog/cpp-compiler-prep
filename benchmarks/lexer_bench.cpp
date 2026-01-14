#include <benchmark/benchmark.h>
#include "lexer/lexer.hpp"

using namespace compiler;

static const char* SAMPLE_CODE = R"(
fn fibonacci(n) {
    if n <= 1 {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

fn main() {
    let result = fibonacci(10);
    let x = 42;
    let y = 3.14;
    let flag = true;

    if x > 0 && flag {
        result = result + 1;
    }

    while x > 0 {
        x = x - 1;
    }

    return result;
}
)";

static void BM_LexerTokenize(benchmark::State& state) {
    for (auto _ : state) {
        ErrorReporter errors;
        Lexer lexer(SAMPLE_CODE, "bench", errors);

        while (lexer.next_token().kind != TokenKind::Eof) {
        }
    }
}
BENCHMARK(BM_LexerTokenize);

static void BM_LexerTokenizeLarge(benchmark::State& state) {
    std::string large_input;
    for (int i = 0; i < 100; i++) {
        large_input += SAMPLE_CODE;
    }

    for (auto _ : state) {
        ErrorReporter errors;
        Lexer lexer(large_input, "bench", errors);

        while (lexer.next_token().kind != TokenKind::Eof) {
        }
    }

    state.SetBytesProcessed(state.iterations() * large_input.size());
}
BENCHMARK(BM_LexerTokenizeLarge);
