#include <benchmark/benchmark.h>
#include <string>

// Placeholder until lexer is implemented
static void BM_LexerPlaceholder(benchmark::State& state) {
    std::string source = "int x = 42;";
    for (auto _ : state) {
        benchmark::DoNotOptimize(source.size());
    }
}
BENCHMARK(BM_LexerPlaceholder);
