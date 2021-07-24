#pragma once

#include "matrix.h"

template<typename T>
Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> res(left.GetLength(), right.GetWidth());
    std::vector<std::thread> threads;
    for (size_t i = 0; i < left.GetLength(); ++i) {
        for (size_t j = 0; j < right.GetWidth(); ++j) {
            threads.emplace_back([i, j, left, right, &res] {
                for (size_t k = 0; k < left.GetWidth(); ++k) {
                    res(i, j) += left(i, k) * right(k, j);
                }
            });
        }
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
