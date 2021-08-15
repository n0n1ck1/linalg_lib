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
#include<random>
#include<iomanip>

template <typename T>
class Matrix {
public:
  Matrix() {
    width_ = 0;
    length_ = 0;
  }
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

  explicit Matrix(const size_t& h, const size_t& w) {
    T default_value = T();
    width_ = w;
    length_ = h;
    matrix_ = std::vector<T>(h * w, default_value);
  }

  Matrix(const size_t& n) : Matrix(n, n) {}

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

  Matrix get_submatrix(const size_t& start_row, const size_t& end_row, const size_t& start_column, const size_t& end_column) const {
    if (!(width_ > end_column && length_ > end_row)) {
      throw std::out_of_range("Specified submatrix doesn't exist.");
    }

    Matrix matrix(end_row - start_row + 1, end_column - start_column + 1);

    size_t n_threads = 2;
    size_t new_width = matrix.width_;
    size_t new_length = matrix.length_;
    std::vector<std::thread> threads;
    for (size_t k = 0; k < n_threads; ++k) {
      threads.emplace_back([&] (const size_t& id) {
        for (size_t i = id * new_length / n_threads; i < (id + 1) * new_length / n_threads; ++i) {
          for (size_t j = 0; j < new_width; ++j) {
            matrix(i, j) = matrix_[(i + start_row) * width_ + (j + start_column)];
          }
        }
      }, k);
    }
    for (auto& t : threads) {
      t.join();
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
    if (width_ != other.width_ || length_ != other.length_)
      return false;
    bool almost_equal = true;
    double eps = 1e-4;
    for (size_t i = 0; i < length_ && almost_equal; ++i) {
      for (size_t j = 0; j < width_; ++j) {
        if (double(1)*std::abs(matrix_[i*width_ + j] - other.matrix_[i*width_ + j]) / std::abs(other.matrix_[i*width_ + j]) > eps) {
          almost_equal = false;
          break;
        }
      }
    }
    return almost_equal;
  }

  bool operator!=(const Matrix& other) const {
    return !(*this == other);
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

  Matrix& operator*=(const T& scale) {
    *this = scale * (*this);
    return *this;
  }

  Matrix& operator/=(const Matrix& other) {
    *this = (*this) / other;
    return *this;
  }

  Matrix& operator^=(const Matrix& other) {
    *this = (*this) ^ other;
    return *this;
  }

  Matrix& dot(const Matrix& other) {
    *this = (*this) ^ other;
    return *this;
  }

  Matrix& concatenate(const Matrix& other, size_t axis = 0) {
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



  void transpose() {
    size_t n_threads = 2;
    std::vector<std::thread> threads;

    if (length_ == width_) {  // интуитивный алгоритм для квадратных матриц
      for (size_t k = 0; k < n_threads; ++k) {
        std::vector<std::atomic<bool>> unused(matrix_.size());
        for (size_t i = 0; i < matrix_.size(); ++i) {
          unused[i].store(true);
        }
        threads.emplace_back([&] (size_t id) {
          for (size_t i = id * length_ / n_threads; i < (id + 1) * length_ / n_threads; ++i) {
            for (size_t j = 0; j < width_; ++j) {
//              #1
              if (i < j) {
                std::swap(matrix_[i * width_ + j], matrix_[j * length_ + i]);
              }

//              #2 - сравню по эффективности, когда добавлю бенчмарки
//              if (unused[j * length_ + i].exchange(false) && unused[i * width_ + j].exchange(false)) {
//                std::swap(matrix_[i * width_ + j], matrix_[j * length_ + i]);
//              }
            }
          }
        }, k);
      }
      for (auto& t: threads) {
        t.join();
      }
    } else if (length_ > 1 && width_ > 1) {  // эффективно для прямоугольных матриц
      n_threads = 2;  // я придумал только для 2-х тредов, потом, может, подумаю, как сделать для n
      for (size_t k = 0; k < n_threads; ++k) {
        threads.emplace_back([&] (size_t id) {
          size_t start = id + 1;
          size_t i = start;
          do {
            i = (i / width_) + (i % width_) * length_;
            std::swap(matrix_[start], matrix_[i]);
          } while (i != start);
        }, k);
      }
      for (auto& t: threads) {
        t.join();
      }
      std::swap(length_, width_);
    }


//    Matrix<T> res(width_, length_);
//    size_t n_threads = 2;
//    std::vector<std::thread> threads;
//    for (size_t k = 0; k < n_threads; ++k) {
//      threads.emplace_back([&] (size_t id) {
//        for (size_t i = id * width_ / n_threads; i < (id + 1) * width_ / n_threads; ++i) {
//          for (size_t j = 0; j < length_; ++j) {
//            res(i, j) = matrix_[j * width_ + i];
//          }
//        }
//      }, k);
//    }
//    for (auto& t: threads) {
//      t.join();
//    }
//    *this = res;
  }


  void fill_random(const T& range_low, const T& range_high) {
    size_t n_threads = 2;
    std::vector<std::thread> threads;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(range_low, range_high);

    for (size_t k = 0; k < n_threads; ++k) {
      threads.emplace_back([&] (size_t id) {
        for (size_t i = id * length_ / n_threads; i < (id + 1) * length_ / n_threads; ++i) {
          for (size_t j = 0; j < width_; ++j) {
            matrix_[i * width_ + j] = distrib(gen);
          }
        }
      }, k);
    }
    for (auto& t: threads) {
      t.join();
    }
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
      out << std::fixed << std::setprecision(5) << matrix(i, j);
    }
    out << "\n";
  }
  return out;
}
