#pragma once

#include<vector>
#include<thread>
#include<atomic>
#include<iostream>
#include<utility>

template <typename T>
class Matrix {
public:
  Matrix (const std::vector<std::vector<T>>& matrix) {
    matrix_ = matrix;
    width_ = matrix[0].size();
    length_ = matrix.size();
  }
  Matrix (const size_t& h, const size_t& w) {
    length_ = h;
    width_ = w;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < h; ++i) {
      threads.emplace_back([&]{
        std::vector<T> blank_row(w);
        matrix_.push_back(blank_row);
      });
    }
    for (auto& t : threads) {
      t.join();
    }
  }
  Matrix (const Matrix& other) {
    matrix_ = other.matrix_;
    width_ = other.width_;
    length_ = other.length_;
  }
  Matrix (const Matrix&& other) {
    matrix_ = other.matrix_;
    width_ = other.width_;
    length_ = other.length_;
  }
  size_t GetWidth() const {
    return width_;
  }
  size_t GetLength() const {
    return length_;
  }
  std::pair<size_t, size_t> size() const {
    std::pair<size_t, size_t> res = std::make_pair(length_, width_);
    return res;
  }
  std::vector<T> operator[](const size_t& row) const {
    return matrix_[row];
  }
  std::vector<T>& operator[](const size_t& row) {
    return matrix_[row];
  }
  Matrix& operator=(const Matrix& other) {
    matrix_ = other.matrix_;
    width_ = other.width_;
    length_ = other.length_;
    return this;
  }
  Matrix& operator=(const Matrix&& other) {
    matrix_ = other.matrix_;
    width_ = other.width_;
    length_ = other.length_;
    return this;
  }
  bool operator==(const Matrix& other) const {
    return matrix_ == other.matrix_;
  }
  Matrix& operator+(const Matrix& other) {
    std::vector<std::thread> threads;
    Matrix res(length_, width_);
    for (size_t i = 0; i < length_; ++i) {
      for (size_t j = 0; j < width_; ++j) {
        threads.emplace_back([&]{
          res[i][j] = matrix_[i][j] + other[i][j];
        });
      }
    }
    for (auto& t : threads) {
      t.join();
    }
  }
private:
  std::vector<std::vector<T>> matrix_;
  size_t width_;
  size_t length_;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix) {
    for (size_t i = 0; i < matrix.size().first; ++i) {
        for (size_t j = 0; j < matrix.size().second; ++j) {
            if (j != 0) {
                out << "\t";
            }
            out << matrix[i][j];
        }
        if (i != matrix.size().first - 1) {
            out << "\n";
        }
    }
    return out;
}
