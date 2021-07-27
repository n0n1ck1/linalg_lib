#pragma once

#include<algorithm>
#include<atomic>
#include<exception>
#include<iostream>
#include<mutex>
#include<string>
#include<thread>
#include<utility>
#include<vector>

template <typename T>
class Matrix {
public:
  Matrix(const std::vector<std::vector<T>>& matrix) {
    if (matrix.empty()) {
      width_ = 0;
      length_ = 0;
      matrix_ = std::vector<T>();
    }
    else {
      width_ = matrix[0].size();
      length_ = matrix.size();
      matrix_.resize(width_*length_);
      for (size_t i = 0; i < length_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
          matrix_[i*width_ + j] = matrix[i][j];
        }
      }
    }
  }
  Matrix(std::vector<std::vector<T>>&& matrix) {
    if (matrix.empty()) {
      width_ = 0;
      length_ = 0;
      matrix_ = std::vector<T>();
    }
    else {
      width_ = matrix[0].size();
      length_ = matrix.size();
      matrix_.resize(width_*length_);
      for (size_t i = 0; i < length_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
          matrix_[i*width_ + j] = matrix[i][j];
        }
      }
    }
  }
  Matrix(const size_t& h, const size_t& w) { //ошибка: неоднозначно задается Matrix<double>({{0},{0}}) - не распознает, это конструктор от vector<vector> или от двух size_t
    T default_value = T();
    width_ = w;
    length_ = h;
    matrix_ = std::vector<T>(h * w, default_value);
  }
  Matrix(const Matrix& other) {
    matrix_ = other.matrix_;
    width_ = other.width_;
    length_ = other.length_;
  }
  Matrix(Matrix&& other) {
    matrix_ = std::move(other.matrix_);
    width_ = other.width_;
    length_ = other.length_;
  }

  size_t GetWidth() const {
    return width_;
  }
  size_t GetLength() const {
    return length_;
  }
  std::pair<size_t, size_t> GetShape() const {
    return std::make_pair(length_, width_);
  }
    
  T operator()(const size_t& row, const size_t& column) const {
    return matrix_[width_ * row + column];
  }
  T& operator()(const size_t& row, const size_t& column) {
    return matrix_[width_ * row + column];
  }
  Matrix get_row(const size_t& row) const {
    Matrix matrix(1, width_);
    for (size_t i = 0; i < width_; ++i) {
      matrix(0, i) = matrix_[row*width_ + i];
    }
    return matrix;
  }
  Matrix get_column(const size_t& column) const {
    Matrix matrix(length_, 1);
    for (size_t i = 0; i < length_; ++i) {
      matrix(i, 0) = matrix_[column + i*width_];
    }
    return matrix;
  }
  Matrix get_submatrix(const size_t& start_row, const size_t& end_row, const size_t& start_column, const size_t& end_column) const { // можно распараллелить
    Matrix matrix(end_row-start_row+1, end_column-start_column+1);
    for (size_t i = start_row; i < end_row + 1; ++i) {
      for (size_t j = start_column; j < end_column + 1; ++j) {
        matrix(i - start_row, j - start_column) = matrix_[i*width_ + j];
      }
    }
    return matrix;
  }

  Matrix& operator=(const Matrix& other) {
    matrix_ = other.matrix_;
    width_ = other.width_;
    length_ = other.length_;
    return *this;
  }
  Matrix& operator=(Matrix&& other) noexcept {
    matrix_ = std::move(other.matrix_);
    width_ = other.width_;
    length_ = other.length_;
    return *this;
  }

  bool operator==(const Matrix& other) const {
    return width_ == other.width_ && matrix_ == other.matrix_;
  }
  bool operator!=(const Matrix& other) const {
    return !(width_ == other.width_ && matrix_ == other.matrix_);
  }
    
  Matrix& operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
  }
  Matrix& operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
  }
  Matrix& operator*=(const Matrix& other) {
    *this = (*this) * other;
    return *this;
  }
  Matrix& operator/=(const Matrix& other) {
    *this = (*this) / other;
    return *this;
  }
  Matrix& dot(const Matrix& other) {
    *this = dot((*this), other);
    return *this;
  }
    
  Matrix& concatenate(const Matrix& other, size_t axis = 0) { // можно распараллелить
    if (axis == 0) {
      if (!(width_ == other.width_)) {
        throw std::length_error("Different shapes.");
      }
      std::vector<T> new_matrix(length_*width_ + other.length_*other.width_);
      for (size_t i = 0; i < length_*width_; ++i) {
        new_matrix[i] = matrix_[i];
      }
      for (size_t i = 0; i < other.length_*other.width_; ++i) {
        new_matrix[i + length_ * width_] = other.matrix_[i];
      }
      matrix_ = new_matrix;
      length_ += other.length_;
    }
    else {
      if (!(length_ == other.length_)) {
        throw std::length_error("Different shapes.");
      }
      std::vector<T> new_matrix(length_*width_ + other.length_*other.width_);
      for (size_t i = 0; i < length_*width_; ++i) {
        new_matrix[(i / width_)*(width_ + other.width_) + i % width_] = matrix_[i];
      }
      for (size_t i = 0; i < other.length_*other.width_; ++i) {
        new_matrix[(i / other.width_)*(width_ + other.width_) + width_ + i % other.width_] = other.matrix_[i];
      }
      matrix_ = new_matrix;
      width_ += other.width_;
    }
    return *this;
  }
    
  bool empty() const {
    return matrix_.empty();
  }

  Matrix& row_addition(size_t i, size_t j, T k) {
    for (size_t x = 0; x < width_; ++x) {
      matrix_[i*width_ + x] += k * matrix_[j*width_ + x];
    }
    return *this;
  }
  Matrix& row_multiplication(size_t i, T k) {
    for (size_t x = 0; x < width_; ++x) {
      matrix_[i*width_ + x] *= k;
    }
    return *this;
  }
  Matrix& row_switching(size_t i, size_t j) {
    for (size_t x = 0; x < width_; ++x) {
      std::swap(matrix_[i*width_ + x], matrix_[j*width_ + x]);
    }
    return *this;
  }

  Matrix& column_addition(size_t i, size_t j, T k) {
    for (size_t x = 0; x < length_; ++x) {
      matrix_[i + x * width_] += k * matrix_[j + x * width_];
    }
    return *this;
  }
  Matrix& column_multiplication(size_t i, T k) {
    for (size_t x = 0; x < length_; ++x) {
      matrix_[i + x * width_] *= k;
    }
    return *this;
  }
  Matrix& column_switching(size_t i, size_t j) {
    for (size_t x = 0; x < length_; ++x) {
      std::swap(matrix_[i + x * width_], matrix_[j + x * width_]);
    }
    return *this;
  }

private:
  std::vector<T> matrix_;
  size_t width_;
  size_t length_;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix) {
  if (matrix.empty()) {
    out << "Not a matrix\n";
    return out;
  }
  for (size_t i = 0; i < matrix.GetLength(); ++i) {
    for (size_t j = 0; j < matrix.GetWidth(); ++j) {
      if (j != 0) {
        out << "\t";
      }
      out << matrix(i, j);
    }
    out << "\n";
  }
  return out;
}

