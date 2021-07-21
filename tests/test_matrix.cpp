#include "util/timeout_guard.h"
#include <gtest/gtest.h>

#include "../matrix/matrix.h"

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
  std::vector<std::vector<int>> left = {{1, 2, 3}, {4, 5, 6}};
  Matrix<int> matrix_left(left);
  std::vector<std::vector<int>> right = {{2, 3, 4, 5}, {4, 3, 2, 1}};
  right.push_back({4,5,6,7});
  Matrix<int> matrix_right(right);
  std::vector<std::vector<int>> expected = {{22, 24, 26, 28}, {44, 49, 54, 59}};
  //std::cout<< matrix_left << "\n"<< matrix_right;
  Matrix<int> matrix_expected(expected);
//  std::cout <<"\n"<< matrix_expected <<"\n";
  //std::cout << matrix_left * matrix_right <<"\n";
  ASSERT_EQ(matrix_left * matrix_right, matrix_expected);
}
