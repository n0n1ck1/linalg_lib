#pragma once

#include "matrix.h"

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
