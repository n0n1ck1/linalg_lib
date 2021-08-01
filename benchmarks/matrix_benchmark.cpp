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

static void BM_Determinant(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(det(matrix));
  }
}

BENCHMARK(BM_Determinant);
// Run the benchmark

static void BM_SequentialDeterminant(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_det(matrix));
  }
}

BENCHMARK(BM_SequentialDeterminant);
// Run the benchmark

static void BM_Inverse(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(inverse(matrix));
  }
}

BENCHMARK(BM_Inverse);
// Run the benchmark

static void BM_SequentialInverse(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_inverse(matrix));
  }
}

BENCHMARK(BM_SequentialInverse);
// Run the benchmark

// TODO: add rank function

BENCHMARK_MAIN();
