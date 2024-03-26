#include <vector>

#include "matrix.h"

namespace parallels {
using std::make_unique;
using std::vector;

class Matrix::Impl {
 public:
  Impl(size_t rows, size_t columns) : body(rows, vector(columns, 0.)) {}

  size_t Rows() const { return body.size(); }

  size_t Columns() const { return Rows() > 0 ? body[0].size() : 0; }

  double& at(size_t row, size_t column) { return body.at(row).at(column); }

  unique_ptr<Impl> Mult(Matrix const& other) {
    // TODO: Implement
  }

  ~Impl() = default;

 private:
  vector<vector<double>> body;
};

Matrix::Matrix(size_t rows, size_t columns) : _pimpl(make_unique<Impl>(rows, columns)) {}

Matrix::Matrix(Matrix&& other) noexcept = default;

Matrix& Matrix::operator=(Matrix&& other) noexcept = default;

size_t Matrix::Rows() const { return _pimpl->Rows(); }

size_t Matrix::Columns() const { return _pimpl->Columns(); }

double& Matrix::operator()(size_t row, size_t column) { return _pimpl->at(row, column); }

double Matrix::operator()(size_t row, size_t column) const { return _pimpl->at(row, column); }

Matrix Matrix::operator*(Matrix const& other) const { return Matrix(std::move(_pimpl->Mult(other))); }

Matrix::Matrix(unique_ptr<Impl> pimpl) : _pimpl(std::move(pimpl)) {}

Matrix::~Matrix() = default;
}