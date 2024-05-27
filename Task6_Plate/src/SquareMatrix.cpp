#include "SquareMatrix.h"

namespace parallels {
SquareMatrix::SquareMatrix(size_t size) : size_(size), body_(size * size) {}

SquareMatrix::SquareMatrix(SquareMatrix&& other) noexcept = default;

size_t SquareMatrix::Size() const {
  return size_;
}

double& SquareMatrix::operator()(size_t row, size_t column) {
  return body_.at(row * size_ + column);
}
double const& SquareMatrix::operator()(size_t row, size_t column) const {
  return body_.at(row * size_ + column);
}

SquareMatrix& SquareMatrix::operator=(SquareMatrix&& other) noexcept = default;

SquareMatrix::~SquareMatrix() = default;
}
