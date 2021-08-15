#pragma once

#include "matrix.h"
#include "functions.h" // for diag and concatenate

template<typename T>
Matrix<T> seq_dot(const Matrix<T>& left, const Matrix<T>& right) {
  if (!(left.GetWidth() == right.GetLength())) {
    throw std::length_error("Left width (" + std::to_string(left.GetWidth()) + ") and right length (" + std::to_string(right.GetLength()) + ") are not equal.");
  }
  size_t width = right.GetWidth();
  size_t length = left.GetLength();
  size_t count_iter = left.GetWidth();
  Matrix<T> res(length, width);
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < length; ++j) {
      for (size_t p = 0; p < count_iter; ++p) {
        res(j, i) += left(j, p) * right(p, i);
      }
    }
  }
  return res;
}

template<typename T>
T seq_det(Matrix<T> matrix) {
  size_t width = matrix.GetWidth();
  size_t length = matrix.GetLength();
  if (width != length) {
      throw "The matrix isn't a square";
      return static_cast<T>(0);
  }
  T res = static_cast<T>(1);
  for (size_t i = 0; i < width - 1; ++i) {
      if (matrix(i, i) == static_cast<T>(0)) {
          bool has_non_zero = false;
          size_t index_non_zero;
          for (size_t j = i; j < width; ++j) {
              if (matrix(j, i) != 0) {
                  has_non_zero = true;
                  index_non_zero = j;
              }
          }
          if (!has_non_zero) {
              return static_cast<T>(0);
          }
          matrix.row_switching(i, index_non_zero);
          res *= static_cast<T>(-1);
      }
      for (size_t j = i + 1; j < width; ++j) {
          matrix.row_addition(j, i, static_cast<T>(-1) * matrix(j, i) / matrix(i, i));
      }
  }
  for (size_t i = 0; i < width; ++i) {
      res *= matrix(i, i);
  }
  return res;
}

template<typename T>
Matrix<T> seq_inverse(const Matrix<T>& matrix) {
  if (matrix.GetWidth() != matrix.GetLength()) {
    throw "The matrix ins't a square";
    return Matrix<T>();
  }
  size_t width = matrix.GetWidth();
  Matrix<T> sle = concatenate(matrix, diag(static_cast<T>(1.0), width), 1);
  for (size_t i = 0; i < width - 1; ++i) {
    if (sle(i, i) == static_cast<T>(0)) {
      bool has_non_zero = false;
      size_t index_non_zero;
      for (size_t j = i; j < width; ++j) {
        if (sle(j, i) != 0) {
          has_non_zero = true;
          index_non_zero = j;
        }
      }
      if (!has_non_zero) {
        throw "Determinant equals 0, inverse matrix doesn't exist";
        return Matrix<T>();
      }
      sle.row_switching(i, index_non_zero);
    }
    for (size_t j = i + 1; j < width; ++j) {
      sle.row_addition(j, i, static_cast<T>(-1) * sle(j, i) / sle(i, i));
    }
  }
  if (sle(width - 1, width - 1) == 0) {
    throw "Determinant equals 0, inverse matrix doesn't exist";
    return Matrix<T>();
  }
  for (long long int i = width - 1; i >= 0; --i) {
    sle.row_multiplication(i, 1 / sle(i, i));
    for (long long int j = 0; j < i; ++j) {
      sle.row_addition(j, i, -sle(j, i));
    }
  }
  return sle.get_submatrix(0, width - 1, width, 2 * width - 1);
}

template <typename T>
Matrix<T> seq_sle_solution(const Matrix<T> &left_part, const Matrix<T> right_part) {
  auto [left_length, left_width] = left_part.GetShape();
  auto [right_length, right_width] = right_part.GetShape();
  if (left_length != right_length) {
    throw std::length_error("Shapes do not match.");
  }
  Matrix<T> sle_matrix = concatenate(left_part, right_part, 1);
  //size_t length = left_width;
  //size_t width = right_width;

  //straight gauss
  for (size_t i = 0; i < left_width; ++i) {
    size_t first_not_zero = i;
    while (first_not_zero < left_length && sle_matrix(first_not_zero, i) == 0) {
      ++first_not_zero;
    }
    if (first_not_zero == left_length) {
      return Matrix<T>(0, 0);
    }
    while (first_not_zero != i) {
      sle_matrix.row_switching(first_not_zero, first_not_zero - 1);
      --first_not_zero;
    }

    for (size_t j = first_not_zero + 1; j < left_length; ++j) {
      sle_matrix.row_addition(j, first_not_zero, -sle_matrix(j, i) / sle_matrix(first_not_zero, i));
    }

    sle_matrix.row_multiplication(first_not_zero, 1 / sle_matrix(first_not_zero, i));
  }
  if (left_length > left_width) {
    bool do_not_have_solution = false;
    for (size_t i = left_width; i < left_length; ++i) {
      for (size_t j = 0; j < left_width + right_width; ++j) {
        if (sle_matrix(i, j) != 0) {
          do_not_have_solution = true;
          break;
        }
      }
      if (do_not_have_solution) {
        break;
      }
    }
    if (do_not_have_solution) {
      return Matrix<T>(0, 0);
    }
  }

  //reversed gauss
  for (int i = left_width - 1; i != -1; --i) {
    for (int j = i - 1; j != -1; --j) {
      sle_matrix.row_addition(j, i, -sle_matrix(j, i));
    }
  }

  return sle_matrix.get_submatrix(0, left_width - 1, left_width, left_width + right_width - 1);
}

template<typename T>
Matrix<T> seq_add(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  Matrix<T> res(length, width);
  size_t n_threads = 2;
  size_t to_look_at = (length*width) / n_threads + ((length*width) % n_threads == 0 ? 0 : 1);
  std::vector<std::thread> threads;
  for (size_t k = 0; k < n_threads; ++k) {
    threads.emplace_back([&](size_t id) {
      for (size_t i = to_look_at * id; i < length*width && i < to_look_at*(id + 1); ++i) {
        res(i / width, i%width) = matrix1(i / width, i%width) + matrix2(i / width, i%width);
      }
    }, k);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}

template<typename T>
Matrix<T> seq_sub(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  Matrix<T> res(length, width);
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < length; ++j) {
      res(i, j) = matrix1(i, j) - matrix2(i, j);
    }
  }
  return res;
}

template<typename T>
Matrix<T> seq_mult(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  Matrix<T> res(length, width);
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < length; ++j) {
      res(i, j) = matrix1(i, j) * matrix2(i, j);
    }
  }
  return res;
}

template<typename T>
Matrix<T> seq_div(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  Matrix<T> res(length, width);
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < length; ++j) {
      res(i, j) = matrix1(i, j) / matrix2(i, j);
    }
  }
  return res;
}

template<typename T>
Matrix<T> seq_scale(T scale, const Matrix<T>& matrix) {
  size_t width = matrix.GetWidth();
  size_t length = matrix.GetLength();
  Matrix<T> res(length, width);
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < length; ++j) {
      res(i, j) = scale * matrix(i, j);
    }
  }
  return res;
}

template<typename T>
Matrix<T> seq_transposed(const Matrix<T>& matrix) {
  auto [length, width] = matrix.GetShape();
  Matrix<T> res(width, length);
  for (size_t i = 0; i < length; ++i) {
    for (size_t j = 0; j < width; ++j) {
      res(i, j) = matrix(j, i);
    }
  }
}