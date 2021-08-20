#include <benchmark/benchmark.h>
#include "../matrix/functions.h"
#include "../matrix/sequential_functions.h"

static void BM_Multiplication(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(dot(matrix, matrix));
  }
}
BENCHMARK(BM_Multiplication);

static void BM_SequentialMultiplication(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_dot(matrix, matrix));
  }
}

BENCHMARK(BM_SequentialMultiplication);
// Run the benchmark

static void BM_Determinant(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(det(matrix));
  }
}

BENCHMARK(BM_Determinant);
// Run the benchmark

static void BM_SequentialDeterminant(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_det(matrix));
  }
}

BENCHMARK(BM_SequentialDeterminant);
// Run the benchmark

static void BM_Inverse(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(inverse(matrix));
  }
}

BENCHMARK(BM_Inverse);
// Run the benchmark

static void BM_SequentialInverse(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_inverse(matrix));
  }
}

BENCHMARK(BM_SequentialInverse);
// Run the benchmark

static void BM_SLE(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  Matrix<double> result(100, 1);
  for (size_t i = 0; i < 100; ++i) {
    result(i,0) = 1.;
  }
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(sle_solution(matrix, result));
  }
}

BENCHMARK(BM_SLE);
// Run the benchmark

static void BM_FastSLE(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  Matrix<double> result(100, 1);
  for (size_t i = 0; i < 100; ++i) {
    result(i,0) = 1.;
  }
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(fast_sle_solution(matrix, result));
  }
}

BENCHMARK(BM_FastSLE);
// Run the benchmark

static void BM_SequentialSLE(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  Matrix<double> result(100, 1);
  for (size_t i = 0; i < 100; ++i) {
    result(i,0) = 1.;
  }
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_sle_solution(matrix, result));
  }
}

BENCHMARK(BM_SequentialSLE);
// Run the benchmark

static void BM_Rank(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(rank(matrix));
  }
}

BENCHMARK(BM_Rank);
// Run the benchmark

static void BM_FastRank(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(fast_rank(matrix));
  }
}

BENCHMARK(BM_FastRank);
// Run the benchmark

static void BM_SequentialRank(benchmark::State& state) {
  Matrix<double> matrix = diag(1.0, 100);
  // TODO: replace diag(1, 100) with a randomly generated matrix
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_rank(matrix));
  }
}

BENCHMARK(BM_SequentialRank);
// Run the benchmark


static void BM_TransposeSquare(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(transposed(matrix));
  }
}

BENCHMARK(BM_TransposeSquare);

static void BM_SequentialTransposeSquare(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 100);
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_transposed(matrix));
  }
}

BENCHMARK(BM_SequentialTransposeSquare);

static void BM_TransposeRectangle(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 200);
  for (auto _ : state) {
    benchmark::DoNotOptimize(transposed(matrix));
  }
}

BENCHMARK(BM_TransposeRectangle);

static void BM_SequentialTransposeRectangle(benchmark::State& state) {
  Matrix<double> matrix = random_matrix(100, 200);
  for (auto _ : state) {
    benchmark::DoNotOptimize(seq_transposed(matrix));
  }
}

BENCHMARK(BM_SequentialTransposeRectangle);

BENCHMARK_MAIN();
