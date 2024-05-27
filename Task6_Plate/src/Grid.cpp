#include "Grid.h"

namespace parallels {
Grid::Grid(size_t size) : size_(size), body_(size * size) {}

size_t Grid::Size() const {
  return size_;
}

double& Grid::operator()(size_t row, size_t column) {
  return body_.at(row * size_ + column);
}

double const& Grid::operator()(size_t row, size_t column) const {
  return body_.at(row * size_ + column);
}
}
