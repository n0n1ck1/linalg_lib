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
    Matrix operator+(const Matrix& other) {
        Matrix res(length_, width_);
        for (size_t i = 0; i < width_ * length_; ++i) {
            res.matrix_[i] = matrix_[i] + other.matrix_[i];
        }
        return res;
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
