#pragma once
#include <vector>

namespace parallels {
using std::vector;

class SquareMatrix {
 public:
  explicit SquareMatrix(size_t size);

  size_t Size() const;

  double& operator()(size_t row, size_t column);

  double const& operator()(size_t row, size_t column) const;

 private:
  size_t size_;
  vector<double> body_;
};
}