#include "util/timeout_guard.h"
#include <gtest/gtest.h>

#include "../matrix/matrix.h"
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

TEST(Matrix, SLE1) {
  Matrix<double> matrix_1({{2, -4, 1, -5}, {4, -7, -1, -8}, {10, -18, 2, -23}, {2, -3, 1, -1}});
  Matrix<double> matrix_2({{2}, {-5}, {3}, {0}});
  Matrix<double> expected({{1}, {2}, {3}, {-1}});
  ASSERT_EQ(sle_solution(matrix_1, matrix_2), expected);
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
