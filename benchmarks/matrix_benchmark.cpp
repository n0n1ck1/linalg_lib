#include <benchmark/benchmark.h>
#include "../matrix/functions.h"
#include "../matrix/sequential_functions.h"

static void BM_Multiplication(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(dot(matrix, matrix));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Multiplication);

static void BM_SequentialMultiplication(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_dot(matrix, matrix));
  }
}

BENCHMARK(BM_SequentialMultiplication);
// Run the benchmark
BENCHMARK_MAIN();
