#include "util/timeout_guard.h"
#include <gtest/gtest.h>

#include "../matrix/functions.h"

TEST(Matrix, Generation) {
    TimeoutGuard guard(1s);
    Matrix<int> matrix_1(3, 4);
    std::vector<std::vector<int>> v;
    for (int i = 0; i < 3; ++i) {
      std::vector<int> row(4);
      v.push_back(row);
    }
    Matrix<int> matrix_2(v);
    ASSERT_EQ(matrix_1, matrix_2);
}

TEST(Matrix, SimpleMultiplication) {
  TimeoutGuard guard(1s);
  std::vector<std::vector<int>> left = {{1, 2, 3}, {4, 5, 6}};
  Matrix<int> matrix_left(left);
  std::vector<std::vector<int>> right = {{2, 3, 4, 5}, {4, 3, 2, 1}, {4, 5, 6, 7}};
  Matrix<int> matrix_right(right);
  std::vector<std::vector<int>> expected = {{22, 24, 26, 28}, {52, 57, 62, 67}};
  Matrix<int> matrix_expected(expected);
  ASSERT_EQ(dot(matrix_left, matrix_right), matrix_expected);
}

TEST(Matrix, BigMultiplication) {
  Matrix<int> eye = diag(1, 100);
  Matrix<int> doubled_eye = 2 * eye;
  Matrix<int> tripled_eye = 3 * eye;
  Matrix<int> expected = 6 * eye;
  ASSERT_EQ(doubled_eye * tripled_eye, expected);
}

TEST(Matrix, Addition) {
  Matrix<int> matrix_1({{1, 2, 3}, {3, 2, 1}});
  Matrix<int> matrix_2({{3, 2, 1}, {1, 2, 3}});
  Matrix<int> expected({{4, 4, 4}, {4, 4, 4}});
  ASSERT_EQ(matrix_1 + matrix_2, expected);
}

TEST(Matrix, SimpleDeterminant) {
  Matrix<double> matrix({{1, 2, 3, 4},
                        {4, 3, 2, 1},
                        {2, 3, 4, 6},
                        {4, 2, 1, 3}});
  ASSERT_EQ(det(matrix), 5);
}

TEST(Matrix, BigDeterminant) {
  Matrix<double> matrix = diag(1.0, 1000);
  ASSERT_EQ(det(matrix), 1);
}

TEST(Matrix, SimpleInverse) {
  Matrix<double> matrix({{1.0, 2.0, 3.0, 4.0},
                         {4.0, 3.0, 2.0, 1.0},
                         {2.0, 3.0, 4.0, 6.0},
                         {4.0, 2.0, 1.0, 3.0}});
  Matrix<double> expected({{3.8, -0.2, -3.0, 1.0},
                           {-9.2, 0.8, 7.0, -2.0},
                           {6.8, -0.2, -5.0, 1.0},
                           {-1.2, -0.2, 1.0, 0.0}});
  ASSERT_EQ(inverse(matrix), expected);
}

TEST(Matrix, BigInverse) {
  Matrix<double> matrix = diag(2.0, 500);
  Matrix<double> expected = diag(0.5, 500);
  ASSERT_EQ(inverse(matrix), expected);
}

TEST(Matrix, ParallelSle_3_inf) {
  Matrix<double> matrix_1({{2, -3, 1}, {3, -5, 5}, {5, -8, 6}});
  Matrix<double> matrix_2({{2}, {3}, {5}});
  Matrix<double> expected(0, 0);
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_3_no_solution) {
  Matrix<double> matrix_1({{5, -6, 1}, {3, -5, -2}, {2, -1, 3}});
  Matrix<double> matrix_2({{4}, {3}, {5}});
  Matrix<double> expected(0, 0);
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_3) {
  Matrix<double> matrix_1({{2, 3, 5}, {3, 7, 4}, {1, 2, 2}});
  Matrix<double> matrix_2({{10}, {3}, {3}});
  Matrix<double> expected({{3}, {-2}, {2}});
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_4) {
  Matrix<double> matrix_1({ {3, -2, -5, 1}, {2, -3, 1, 5}, {1, 2, 0, -4}, {1, -1, -4, 9 } });
  Matrix<double> matrix_2({ {3}, {-3}, {-3}, {22} });
  Matrix<double> expected({ {-1}, {3}, {-2}, {2} });
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_5) {
  Matrix<double> matrix_1({ {1, 1, 4, 4, 9}, {2, 2, 17, 17, 82}, {2, 0, 3, -1, 4}, {0, 1, 4, 12, 27}, {1, 2, 2, 10, 0} });
  Matrix<double> matrix_2({ {-9}, {-146}, {-10}, {-26}, {37} });
  Matrix<double> expected({ {5}, {4}, {-3}, {3}, {-2} });
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_5_dif_shape) {
  Matrix<double> matrix_1({ {1, 1, 4, 4, 9}, {2, 2, 17, 17, 82}, {2, 0, 3, -1, 4}, {0, 1, 4, 12, 27}, {1, 2, 2, 10, 0}, {2, 2, 8, 8, 18} });
  Matrix<double> matrix_2({ {-9}, {-146}, {-10}, {-26}, {37}, {-18} });
  Matrix<double> expected({ {5}, {4}, {-3}, {3}, {-2} });
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_big) {
  int size = 100;
  double val = 2.;
  Matrix<double> big_matrix = diag(val, size);
  Matrix<double> big_matrix_res(size, 1);
  for (int i = 0; i < size; ++i) {
      big_matrix_res(i, 0) = val;
  }
  Matrix<double> expected(size, 1);
  for (int i = 0; i < size; ++i) {
      expected(i, 0) = 1.;
  }
  for (int i = size - 2; i > -1; --i) {
      big_matrix.row_addition(i, i + 1, 1);
      big_matrix_res.row_addition(i, i + 1, 1);
  }
  for (int i = 0; i < size / 2; ++i) {
      big_matrix.row_switching(i, size - 1 - i);
      big_matrix_res.row_switching(i, size - 1 - i);
  }
  Matrix<double> res = parallel_sle_solution(big_matrix, big_matrix_res);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_large) {
    int size = 1000;
  double val = 2.;
  Matrix<double> big_matrix = diag(val, size);
  Matrix<double> big_matrix_res(size, 1);
  for (int i = 0; i < size; ++i) {
      big_matrix_res(i, 0) = val;
  }
  Matrix<double> expected(size, 1);
  for (int i = 0; i < size; ++i) {
      expected(i, 0) = 1.;
  }
  for (int i = size - 2; i > -1; --i) {
      big_matrix.row_addition(i, i + 1, 1);
      big_matrix_res.row_addition(i, i + 1, 1);
  }
  for (int i = 0; i < size / 2; ++i) {
      big_matrix.row_switching(i, size - 1 - i);
      big_matrix_res.row_switching(i, size - 1 - i);
  }
  Matrix<double> res = parallel_sle_solution(big_matrix, big_matrix_res);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSle_zero) {
  Matrix<double> matrix_1({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
  Matrix<double> matrix_2({{0}, {0}, {0}});
  Matrix<double> expected(0, 0);
  Matrix<double> res = parallel_sle_solution(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_3_inf) {
  Matrix<double> matrix_1({{2, -3, 1}, {3, -5, 5}, {5, -8, 6}});
  Matrix<double> matrix_2({{2}, {3}, {5}});
  Matrix<double> expected(0, 0);
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_3_no_solution) {
  Matrix<double> matrix_1({{5, -6, 1}, {3, -5, -2}, {2, -1, 3}});
  Matrix<double> matrix_2({{4}, {3}, {5}});
  Matrix<double> expected(0, 0);
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_3) {
  Matrix<double> matrix_1({{2, 3, 5}, {3, 7, 4}, {1, 2, 2}});
  Matrix<double> matrix_2({{10}, {3}, {3}});
  Matrix<double> expected({{3}, {-2}, {2}});
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_4) {
  Matrix<double> matrix_1({ {3, -2, -5, 1}, {2, -3, 1, 5}, {1, 2, 0, -4}, {1, -1, -4, 9 } });
  Matrix<double> matrix_2({ {3}, {-3}, {-3}, {22} });
  Matrix<double> expected({ {-1}, {3}, {-2}, {2} });
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_5) {
  Matrix<double> matrix_1({ {1, 1, 4, 4, 9}, {2, 2, 17, 17, 82}, {2, 0, 3, -1, 4}, {0, 1, 4, 12, 27}, {1, 2, 2, 10, 0} });
  Matrix<double> matrix_2({ {-9}, {-146}, {-10}, {-26}, {37} });
  Matrix<double> expected({ {5}, {4}, {-3}, {3}, {-2} });
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_5_dif_shape) {
  Matrix<double> matrix_1({ {1, 1, 4, 4, 9}, {2, 2, 17, 17, 82}, {2, 0, 3, -1, 4}, {0, 1, 4, 12, 27}, {1, 2, 2, 10, 0}, {2, 2, 8, 8, 18} });
  Matrix<double> matrix_2({ {-9}, {-146}, {-10}, {-26}, {37}, {-18} });
  Matrix<double> expected({ {5}, {4}, {-3}, {3}, {-2} });
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_big) {
  int size = 100;
  double val = 2.;
  Matrix<double> big_matrix = diag(val, size);
  Matrix<double> big_matrix_res(size, 1);
  for (int i = 0; i < size; ++i) {
      big_matrix_res(i, 0) = val;
  }
  Matrix<double> expected(size, 1);
  for (int i = 0; i < size; ++i) {
      expected(i, 0) = 1.;
  }
  for (int i = size - 2; i > -1; --i) {
      big_matrix.row_addition(i, i + 1, 1);
      big_matrix_res.row_addition(i, i + 1, 1);
  }
  for (int i = 0; i < size / 2; ++i) {
      big_matrix.row_switching(i, size - 1 - i);
      big_matrix_res.row_switching(i, size - 1 - i);
  }
  Matrix<double> res = parallel_sle_solution_per_rows(big_matrix, big_matrix_res);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_large) {
  int size = 500;
  double val = 2.;
  Matrix<double> big_matrix = diag(val, size);
  Matrix<double> big_matrix_res(size, 1);
  for (int i = 0; i < size; ++i) {
      big_matrix_res(i, 0) = val;
  }
  Matrix<double> expected(size, 1);
  for (int i = 0; i < size; ++i) {
      expected(i, 0) = 1.;
  }
  for (int i = size - 2; i > -1; --i) {
      big_matrix.row_addition(i, i + 1, 1);
      big_matrix_res.row_addition(i, i + 1, 1);
  }
  for (int i = 0; i < size / 2; ++i) {
      big_matrix.row_switching(i, size - 1 - i);
      big_matrix_res.row_switching(i, size - 1 - i);
  }
  Matrix<double> res = parallel_sle_solution_per_rows(big_matrix, big_matrix_res);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelSlePerRows_zero) {
  Matrix<double> matrix_1({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
  Matrix<double> matrix_2({{0}, {0}, {0}});
  Matrix<double> expected(0, 0);
  Matrix<double> res = parallel_sle_solution_per_rows(matrix_1, matrix_2);
  ASSERT_EQ(expected, res);
}

TEST(Matrix, ParallelRank) {
  std::vector<size_t> expected({0, 0, 0, 1, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1});
  std::vector<size_t> result;
  result.reserve(expected.size());
  result.push_back(parallel_rank( Matrix<double>({ {0,0,0}, {0,0,0} })));
  result.push_back(parallel_rank( Matrix<double>({ {0,0,0}, {0,0,0}, {0,0,0} })));
  result.push_back(parallel_rank( Matrix<double>({ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} })));
  result.push_back(parallel_rank( Matrix<double>({ {0,0,0}, {0,0,0}, {0,0,1} })));
  result.push_back(parallel_rank( Matrix<double>({ {0,0,0}, {0,1,0}, {0,0,1} })));
  result.push_back(parallel_rank( Matrix<double>({ {1,0,0}, {0,1,0}, {0,0,1} })));
  result.push_back(parallel_rank( Matrix<double>({ {1,0,0}, {0,1,0}, {0,0,1}, {1,2,3} })));
  result.push_back(parallel_rank( Matrix<double>({ {1,2,3}, {0,1,0}, {0,0,1} })));
  result.push_back(parallel_rank( Matrix<double>({ {1,2,3}, {2,4,6}, {0,0,0} })));
  result.push_back(parallel_rank( Matrix<double>({ {1,2,3}, {0,1,0}, {1,3,3} })));
  result.push_back(parallel_rank( Matrix<double>({ {1,2,3}, {0,1,0} })));
  result.push_back(parallel_rank( Matrix<double>({ {2,4,2}, {0,0,0}, {0,0,0}, {0,2,0}, {-1,0,-1} })));
  result.push_back(parallel_rank( Matrix<double>({ {3,2,1}, {1,2,3} })));
  result.push_back(parallel_rank( Matrix<double>({ {4,4,4}, {4,4,4} })));
  ASSERT_EQ(expected, result);
}

TEST(Matrix, ParallelRank_big) {
  size_t size = 500;
  Matrix<double> matrix = diag(1., size);
  ASSERT_EQ(parallel_rank(matrix), size);
}

TEST(Matrix, ParallelRankPerRows) {
  std::vector<size_t> expected({0, 0, 0, 1, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1});
  std::vector<size_t> result;
  result.reserve(expected.size());
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {0,0,0}, {0,0,0} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {0,0,0}, {0,0,0}, {0,0,0} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {0,0,0}, {0,0,0}, {0,0,1} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {0,0,0}, {0,1,0}, {0,0,1} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {1,0,0}, {0,1,0}, {0,0,1} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {1,0,0}, {0,1,0}, {0,0,1}, {1,2,3} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {1,2,3}, {0,1,0}, {0,0,1} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {1,2,3}, {2,4,6}, {0,0,0} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {1,2,3}, {0,1,0}, {1,3,3} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {1,2,3}, {0,1,0} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {2,4,2}, {0,0,0}, {0,0,0}, {0,2,0}, {-1,0,-1} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {3,2,1}, {1,2,3} })));
  result.push_back(parallel_rank_per_rows( Matrix<double>({ {4,4,4}, {4,4,4} })));
  ASSERT_EQ(expected, result);
}

TEST(Matrix, ParallelRankPerRows_big) {
  size_t size = 500;
  Matrix<double> matrix = diag(1., size);
  ASSERT_EQ(parallel_rank_per_rows(matrix), size);
}
