#pragma once

#include "matrix.h"

template<typename T>
Matrix<T> operator+(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  std::vector<std::thread> threads;
  Matrix<T> res(length, width);
  for (size_t i = 0; i < length; ++i) {
    threads.emplace_back([&](size_t x) {
      for (size_t y = 0; y < width; ++y) {
        res(x, y) = matrix1(x, y) + matrix2(x, y);
      }
    }, i);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}

template<typename T>
Matrix<T> operator*(T scale, const Matrix<T>& matrix) {
  Matrix<T> res(matrix.GetLength(), matrix.GetWidth());
  for (size_t i = 0; i < matrix.GetLength(); ++i) {
    for (size_t j = 0; j < matrix.GetWidth(); ++j) {
      res(i, j) = scale * matrix(i, j);
    }
  }
  return res;
}

template<typename T>
Matrix<T> operator-(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  std::vector<std::thread> threads;
  Matrix<T> res(length, width);
  for (size_t i = 0; i < length; ++i) {
    threads.emplace_back([&](size_t x) {
      for (size_t y = 0; y < width; ++y) {
        res(x, y) = matrix1(x, y) - matrix2(x, y);
      }
    }, i);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}


template<typename T>
Matrix<T> operator*(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  std::vector<std::thread> threads;
  Matrix<T> res(length, width);
  for (size_t i = 0; i < length; ++i) {
    threads.emplace_back([&](size_t x) {
      for (size_t y = 0; y < width; ++y) {
        res(x, y) = matrix1(x, y) * matrix2(x, y);
      }
    }, i);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}


template<typename T>
Matrix<T> operator/(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (!(matrix1.GetLength() == matrix2.GetLength() && matrix1.GetWidth() == matrix2.GetWidth())) {
    throw std::length_error("Different shapes");
  }
  size_t width = matrix1.GetWidth();
  size_t length = matrix1.GetLength();
  std::vector<std::thread> threads;
  Matrix<T> res(length, width);
  for (size_t i = 0; i < length; ++i) {
    threads.emplace_back([&](size_t x) {
      for (size_t y = 0; y < width; ++y) {
        res(x, y) = matrix1(x, y) / matrix2(x, y);
      }
    }, i);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}


template<typename T>
Matrix<T> dot(const Matrix<T>& left, Matrix<T> right) {
  if (!(left.GetWidth() == right.GetLength())) {
    throw std::length_error("Left width (" + std::to_string(left.GetWidth()) + ") and right length (" + std::to_string(right.GetLength()) + ") are not equal.");
  }
  size_t n_threads = 2;
  size_t width = right.GetWidth();
  size_t length = left.GetLength();
  size_t count_iter = left.GetWidth();
  Matrix<T> res(length, width);
  std::vector<std::thread> threads;
  for (size_t k = 0; k < n_threads; ++k) {
    threads.emplace_back([k, &res, &left, &right, &count_iter, &width, &length, &n_threads] {
      for (size_t i = k * width / n_threads; i < (k + 1) * width / n_threads; ++i) {
        for (size_t j = 0; j < length; ++j) {
          for (size_t p = 0; p < count_iter; ++p) {
            res(j, i) += left(j, p) * right(p, i);
          }
        }
      }
    });
  }
  for (auto& t : threads) {
      t.join();
  }
  return res;
}

template<typename T>
Matrix<T> concatenate(const Matrix<T>& matrix1, const Matrix<T>& matrix2, size_t axis = 0) {
  Matrix<T> new_matrix = matrix1;
  new_matrix.concatenate(matrix2, axis);
  return new_matrix;
}


template<typename T>
Matrix<T> diag(const T& elem, const size_t& size) {
  Matrix<T> res(size, size);
  for (size_t i = 0; i < size; ++i) {
    res(i, i) = elem;
  }
  return res;
}

template<typename T>
Matrix<T> diag_from_vector(const std::vector<T> vector) {
  size_t size = vector.size();
  Matrix<T> res(size, size);
  for (size_t i = 0; i < size; ++i) {
    res(i, i) = vector[i];
  }
  return res;
}

template <typename T>
Matrix<T> sle_solution(const Matrix<T> &left_part, const Matrix<T> right_part) {
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
