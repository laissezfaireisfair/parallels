#pragma once

#include <memory>

namespace parallels {
using std::unique_ptr;
using std::size_t;

class Matrix {
 public:
  Matrix(size_t rows, size_t columns);

  Matrix(Matrix&& other) noexcept;

  Matrix(Matrix& other) = delete;

  Matrix& operator=(Matrix&& other) noexcept;

  Matrix& operator=(Matrix& other) = delete;

  size_t Rows() const;

  size_t Columns() const;

  double& operator()(size_t row, size_t column);

  double operator()(size_t row, size_t column) const;

  Matrix operator*(Matrix const& other) const;

  Matrix operator-(Matrix const& other) const;

  Matrix operator*(double coef) const;

  double Norm() const;

  ~Matrix();

  static size_t thread_limit_;

 private:
  class Impl;
  unique_ptr<Impl> _pimpl;

  explicit Matrix(unique_ptr<Impl> pimpl);
};
}