#pragma once

#include<vector>
#include<thread>
#include<iostream>
#include<exception>

template <typename T>
class Matrix {
public:
    Matrix (const std::vector<std::vector<T>>& matrix) {
        width_ = matrix[0].size();
        length_ = matrix.size();
        for (const auto& row : matrix) {
            for (const auto& x : row) {
                matrix_.push_back(x);
            }
        }
    }
    Matrix (const size_t& h, const size_t& w) {
        std::vector<T> v(h * w);
        matrix_ = v;
        length_ = h;
        width_ = w;
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
    T operator()(const size_t& row, const size_t& column) const {
        return matrix_[width_ * row + column];
    }
    T& operator()(const size_t& row, const size_t& column) {
        return matrix_[width_ * row + column];
    }
    Matrix& operator=(const Matrix& other) {
        matrix_ = other.matrix_;
        width_ = other.width_;
        length_ = other.length_;
        return *this;
    }
    Matrix& operator=(Matrix&& other) noexcept {
        matrix_ = other.matrix_;
        width_ = other.width_;
        length_ = other.length_;
        return *this;
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
                    res(i, j) += matrix_(i, j) + other(i, j);
                });
            }
        }
        for (auto& t : threads) {
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

template<typename T>
Matrix<T> operator*(Matrix<T> left, Matrix<T> right) {
    Matrix<T> res(left.GetLength(), right.GetWidth());
    std::vector<std::thread> threads;
    for (size_t i = 0; i < left.GetLength(); ++i) {
        for (size_t j = 0; j < right.GetWidth(); ++j) {
            threads.emplace_back([&] {
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
