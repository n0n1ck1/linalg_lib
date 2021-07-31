#pragma once

#include "matrix.h"

template<typename T>
Matrix<T> operator+(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
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
Matrix<T> operator-(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
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
        res(i / width, i%width) = matrix1(i / width, i%width) - matrix2(i / width, i%width);
      }
    }, k);
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
  Matrix<T> res(length, width);
  size_t n_threads = 2;
  size_t to_look_at = (length*width) / n_threads + ((length*width) % n_threads == 0 ? 0 : 1);
  std::vector<std::thread> threads;
  for (size_t k = 0; k < n_threads; ++k) {
    threads.emplace_back([&](size_t id) {
      for (size_t i = to_look_at * id; i < length*width && i < to_look_at*(id + 1); ++i) {
        res(i / width, i%width) = matrix1(i / width, i%width) * matrix2(i / width, i%width);
      }
    }, k);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}

template<typename T>
Matrix<T> operator*(const T& scale, const Matrix<T>& matrix) {
  size_t width = matrix.GetWidth();
  size_t length = matrix.GetLength();
  Matrix<T> res(length, width);
  size_t n_threads = 2;
  size_t to_look_at = (length*width) / n_threads + ((length*width) % n_threads == 0 ? 0 : 1);
  std::vector<std::thread> threads;
  for (size_t k = 0; k < n_threads; ++k) {
    threads.emplace_back([&](size_t id) {
      for (size_t i = to_look_at * id; i < length*width && i < to_look_at*(id + 1); ++i) {
        res(i / width, i % width) = matrix(i / width, i % width) * scale;
      }
    }, k);
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
  Matrix<T> res(length, width);
  size_t n_threads = 2;
  size_t to_look_at = (length*width) / n_threads + ((length*width) % n_threads == 0 ? 0 : 1);
  std::vector<std::thread> threads;
  for (size_t k = 0; k < n_threads; ++k) {
    threads.emplace_back([&](size_t id) {
      for (size_t i = to_look_at * id; i < length*width && i < to_look_at*(id + 1); ++i) {
        res(i / width, i%width) = matrix1(i / width, i%width) / matrix2(i / width, i%width);
      }
    }, k);
  }
  for (auto& t : threads) {
    t.join();
  }
  return res;
}


template<typename T>
Matrix<T> dot(const Matrix<T>& left, const Matrix<T>& right) {
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
    threads.emplace_back([&](const size_t& id) {
      for (size_t i = id * width / n_threads; i < (id + 1) * width / n_threads; ++i) {
        for (size_t j = 0; j < length; ++j) {
          for (size_t p = 0; p < count_iter; ++p) {
            res(j, i) += left(j, p) * right(p, i);
          }
        }
      }
    }, k);
  }
  for (auto& t : threads) {
      t.join();
  }
  return res;
}


template<typename T>
Matrix<T> operator^(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  return dot(matrix1, matrix2);
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


template<typename T>
T det(Matrix<T> matrix) {
    size_t width = matrix.GetWidth();
    size_t length = matrix.GetLength();
    if (width != length) {
        throw "The matrix isn't a square";
    }
    T res = static_cast<T>(1);
    size_t n_threads = 2;
    for (size_t i = 0; i < width - 1; ++i) {
        std::vector<std::thread> threads;
        threads.reserve(n_threads);
        if (matrix(i, i) == static_cast<T>(0)) {
            bool has_non_zero = false;
            size_t index_non_zero;
            for (size_t k = 0; k < n_threads; ++k) {
                threads.emplace_back([&](const size_t& id){
                    // divide the rows in range [i, width] evenly between all threads
                    for (size_t j = i + id * (width - i ) / n_threads; j < i + (id + 1) * (width - i) / n_threads; ++j) {
                        if (matrix(j, i) != 0) {
                            has_non_zero = true;
                            index_non_zero = j;
                            return;
                        }
                    }
                }, k);
            }
            for (auto& t : threads) {
                t.join();
            }
            if (!has_non_zero) {
                return static_cast<T>(0);
            }
            matrix.row_switching(i, index_non_zero);
            res *= static_cast<T>(-1);
        }
        threads.clear();
        // the same as above, except the first thread mustn't have the i-th row in it
        threads.emplace_back([&] {
            for (size_t j = i + 1; j < i + (width - i) / n_threads; ++j) {
                matrix.row_addition(j, i, static_cast<T>(-1) * matrix(j, i) / matrix(i, i));
            }
        });
        for (size_t k = 1; k < n_threads; ++k) {
            threads.emplace_back([&](const size_t& id) {
                for (size_t j = std::max(i + id * (width - i) / n_threads, i + 1); j < i + (id + 1) * (width - i) / n_threads; ++j) {
                    matrix.row_addition(j, i, static_cast<T>(-1) * matrix(j, i) / matrix(i, i));
                }
            }, k);
        }
        for (auto& t : threads) {
            t.join();
        }
    }
    for (size_t i = 0; i < width; ++i) {
        res *= matrix(i, i);
    }
    return res;
}


template<typename T>
Matrix<T> inverse(const Matrix<T>& matrix) {
  if (matrix.GetWidth() != matrix.GetLength()) {
    throw "The matrix isn't a square";
  }
  size_t width = matrix.GetWidth();
  Matrix<T> sle = concatenate(matrix, diag(1.0, width), 1);
  size_t n_threads = 2;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < width - 1; ++i) {
    if (sle(i, i) == static_cast<T>(0)) {
      bool has_non_zero = false;
      size_t index_non_zero;
      threads.clear();
      for (size_t k = 0; k < n_threads; ++k) {
        threads.emplace_back([&](const size_t& id) {
          // divide the rows in range [i, width] evenly between all threads
          for (size_t j = i + id * (width - i ) / n_threads; j < i + (id + 1) * (width - i) / n_threads; ++j) {
            if (sle(j, i) != 0) {
              has_non_zero = true;
              index_non_zero = j;
              return;
            }
          }
        }, k);
      }
      for (auto& t : threads) {
        t.join();
      }
      if (!has_non_zero) {
        throw "Determinant equals 0, inverse matrix doesn't exist";
      }
      sle.row_switching(i, index_non_zero);
    }
    threads.clear();
    // the same as above, except the first thread mustn't have the i-th row in it
    threads.emplace_back([&] {
      for (size_t j = i + 1; j < i + (width - i) / n_threads; ++j) {
        sle.row_addition(j, i, static_cast<T>(-1) * sle(j, i) / sle(i, i));
      }
    });
    for (size_t k = 1; k < n_threads; ++k) {
      threads.emplace_back([&](const size_t& id) {
        for (size_t j = std::max(i + id * (width - i) / n_threads, i + 1); j < i + (id + 1) * (width - i) / n_threads; ++j) {
          sle.row_addition(j, i, static_cast<T>(-1) * sle(j, i) / sle(i, i));
        }
      }, k);
    }
    for (auto& t : threads) {
      t.join();
    }
  }
  if (sle(width - 1, width - 1) == 0) {
    throw "Determinant equals 0, inverse matrix doesn't exist";
  }
  for (long long int i = width - 1; i >= 0; --i) {
    sle.row_multiplication(i, 1 / sle(i, i));
    threads.clear();
    for (size_t k = 0; k < n_threads; ++k) {
      threads.emplace_back([&](const size_t& id) {
        for (size_t j = id * i / n_threads; j < (id + 1) * i / n_threads; ++j) {
          sle.row_addition(j, i, -sle(j, i));
        }
      }, k);
    }
    for (auto& t : threads) {
      t.join();
    }
  }
  return sle.get_submatrix(0, width - 1, width, 2 * width - 1);
}


template<typename T>
Matrix<T> transposed(const Matrix<T> &matrix) {
  Matrix<T> res = matrix;
  res.transpose();
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


template<typename T>
Matrix<T> parallel_sle_solution(const Matrix<T>& left_part, const Matrix<T> right_part) {
  int left_length = left_part.GetLength();
  int left_width = left_part.GetWidth();
  int right_length = right_part.GetLength();
  int right_width = right_part.GetWidth();
  if (left_length != right_length) {
    throw std::length_error("Shapes do not match.");
  }
  Matrix<T> sle_matrix = concatenate(left_part, right_part, 1);
  int n_threads = 2;
  std::vector<std::thread> threads;
  threads.reserve(n_threads);
  // size_t length = left_width;
  // size_t width = right_width;
  // straight gauss
  for (int i = 0; i < int(left_width); ++i) {
    int to_look_at = (left_length - 1 - i) / n_threads + ((left_length - 1 - i) % n_threads == 0 ? 0 : 1);
    if (sle_matrix(i, i) == 0) {
      std::atomic<int> first_not_zero{ i };
      std::atomic<bool> has_not_zero{ false };
      for (int k = 0; k < n_threads; ++k) {
        threads.emplace_back([&](int id) {
          for (int j = i + 1 + id * to_look_at; j < int(left_length) && j < i + 1 + (id + 1)*to_look_at && !has_not_zero.load(); ++j) {
            if (sle_matrix(j, i) != 0) {
              has_not_zero.store(true);
              first_not_zero.store(j);
              break;
            }
          }
        }, k);
      }
      for (auto& t : threads) {
        t.join();
      }
      threads.clear();
      if (!has_not_zero.load()) {
        return Matrix<T>(0, 0);// inf or no solution
      }
      sle_matrix.row_switching(i, first_not_zero.load());
    }
    for (int k = 0; k < n_threads; ++k) {
      threads.emplace_back([&](int id) {
        for (int j = i + 1 + id * to_look_at; j < int(left_length) && j < i + 1 + (id + 1)*to_look_at; ++j) {
          sle_matrix.row_addition(j, i, -sle_matrix(j, i) / sle_matrix(i, i));
        }
      }, k);
    }
    for (auto& t : threads) {
      t.join();
    }
    threads.clear();
    sle_matrix.row_multiplication(i, 1 / sle_matrix(i, i));
  }
  if (left_length > left_width) {
    int to_look_at = (left_length - left_width) / n_threads + ((left_length - left_width) % n_threads == 0 ? 0 : 1);
    std::atomic<bool> do_not_have_solution{ false };
    for (int k = 0; k < n_threads; ++k) {
      threads.emplace_back([&](int id) {
        for (int i = int(left_width) + id * to_look_at; i < int(left_length) && i < int(left_width) + (id + 1)*to_look_at && !do_not_have_solution.load(); ++i) {
          for (int j = 0; j < int(left_width + right_width) && !do_not_have_solution.load(); ++j) {
            if (sle_matrix(i, j) != 0) {
              do_not_have_solution.store(true);
              break;
            }
          }
        }
      }, k);
    }
    for (auto& t : threads) {
      t.join();
    }
    threads.clear();
    if (do_not_have_solution.load()) {
      return Matrix<T>(0, 0);// no solution
    }
  }
  // reversed gauss
  for (int i = left_width - 1; i != -1; --i) {
    int to_look_at = i / n_threads + (i % n_threads == 0 ? 0 : 1);
    for (int k = 0; k < n_threads; ++k) {
      threads.emplace_back([&](int id) {
        for (int j = id * to_look_at; j < i && j < (id + 1)*to_look_at; ++j) {
          sle_matrix.row_addition(j, i, -sle_matrix(j, i));
        }
      }, k);
    }
    for (auto& t : threads) {
      t.join();
    }
    threads.clear();
  }
  return sle_matrix.get_submatrix(0, left_width - 1, left_width, left_width + right_width - 1);
}


template<typename T>
Matrix<T> parallel_sle_solution_per_rows(const Matrix<T>& left_part, const Matrix<T> right_part) {
  int left_length = left_part.GetLength();
  int left_width = left_part.GetWidth();
  int right_length = right_part.GetLength();
  int right_width = right_part.GetWidth();
  if (left_length != right_length) {
    throw std::length_error("Shapes do not match.");
  }
  int length = left_width;
  int width = right_width;
  Matrix<T> sle_matrix = concatenate(left_part, right_part, 1);
  Matrix<T> answer(length, width);
  std::vector<std::atomic<int>> sequence(left_width);
  for (size_t i = 0; i < size_t(left_width); ++i) {
    sequence[i].store(-1);
  }
  std::vector<std::atomic<int>> phase(3);
  for (size_t i = 0; i < size_t(3); ++i) {
    phase[i].store(0);
  }
  std::vector<std::atomic<int>> waiting(left_width);
  for (size_t i = 0; i < size_t(left_width); ++i) {
    waiting[i].store(0);
  }
  std::atomic<bool> do_not_have_solution{ false };
  std::atomic<bool> inf_solution{ false };
  std::vector<std::thread> threads;
  if (left_length < left_width) {
    inf_solution.store(true);
  }
  else {
    threads.reserve(left_length);
    for (int i = 0; i < left_length; ++i) {
      threads.emplace_back([&](int id) {
        int pos = 0;
        int needed_val = -1;
        while (pos < left_width) {
          if (std::abs(sle_matrix(id, pos)) > 1e-10 && sequence[pos].compare_exchange_weak(needed_val, id)) {
            break;
          }
          waiting[pos].fetch_add(1);
          while (phase[0].load() < pos + 1) {
            std::this_thread::yield();
            if (phase[0].load() + waiting[pos].load() == left_length && phase[0].load() < pos + 1) {
              inf_solution.store(true);// inf or no solution
              break;
            }
          }
          if (inf_solution.load()) {
            break;
          }
          sle_matrix.row_addition(id, sequence[pos].load(), -sle_matrix(id, pos) / sle_matrix(sequence[pos].load(), pos));
          ++pos;
          needed_val = -1;
        }
        phase[0].fetch_add(1);
        while (phase[0].load() != left_length) {
          std::this_thread::yield();
        }
        if (!inf_solution.load() && !do_not_have_solution.load()) {
          if (pos == left_width) {
            for (int j = 0; j < left_width + right_width && !inf_solution.load() && !do_not_have_solution.load(); ++j) {
              if (std::abs(sle_matrix(id, j)) > 1e-10) {
                do_not_have_solution.store(true);// no solution
                break;
              }
            }
          }
          else {
            sle_matrix.row_multiplication(id, 1 / sle_matrix(id, pos));
          }
        }
        phase[1].fetch_add(1);
        while (phase[1].load() != left_length) {
          std::this_thread::yield();
        }
        if (!inf_solution.load() && !do_not_have_solution.load() && pos < left_width) {
          for (int j = left_width - 1; j > pos; --j) {
            while (phase[2].load() < left_width - j) {
              std::this_thread::yield();
            }
            sle_matrix.row_addition(id, sequence[j].load(), -sle_matrix(id, j));
          }
          phase[2].fetch_add(1);
          for (int j = 0; j < width; ++j) {
            answer(pos, j) = sle_matrix(id, left_width + j);
          }
        }
      }, i);
    }
    for (auto& t : threads) {
      t.join();
    }
  }
  if (inf_solution.load() || do_not_have_solution.load()) {
    return Matrix<T>(0, 0);
  }
  return answer;
}
