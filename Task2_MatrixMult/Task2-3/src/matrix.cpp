#include <vector>
#include <stdexcept>
#include <cmath>

#include "matrix.h"

namespace parallels {
using std::make_unique;
using std::vector;
using std::invalid_argument;
using std::min;
using std::sqrt;

size_t Matrix::thread_limit_;

class Matrix::Impl {
 public:
  Impl(size_t rows, size_t columns) : body(rows * columns, 0.),
                                      _rows(rows),
                                      _columns(columns) {}

  size_t Rows() const { return _rows; }

  size_t Columns() const { return _columns; }

  const double& at(size_t row, size_t column) const { return body.at(row * _columns + column); }
  double& at(size_t row, size_t column) { return body.at(row * _columns + column); }

  unique_ptr<Impl> Mult(Matrix const& other) const {
    if (Columns() != other.Rows())
      throw invalid_argument("Cannot multiply matrices with such shapes");

    auto result = make_unique<Impl>(Rows(), other.Columns());

#pragma omp parallel for num_threads(thread_limit_)
    for (size_t i = 0; i < Rows(); ++i) {
      for (size_t j = 0; j < other.Columns(); ++j) {
        double sum = 0;
        for (size_t k = 0; k < Columns(); ++k)
          sum += at(i, k) * other(k, j);
        result->at(i, j) = sum;
      }
    }

    return result;
  }

  unique_ptr<Impl> Sub(Matrix const& other) const {
    if (Rows() != other.Rows() || Columns() != other.Columns())
      throw invalid_argument("Cannot subtract matrices with such shapes");

    auto result = make_unique<Impl>(Rows(), Columns());

    for (size_t i = 0; i < body.size(); ++i)
      result->body[i] = body[i] - other._pimpl->body[i];

    return result;
  }

  double Norm() const {
    double sum = 0.;
#pragma omp parallel for num_threads(thread_limit_) reduction(+:sum)
    for (auto cell : body)
      sum += cell;
    return sqrt(sum);
  }

  unique_ptr<Impl> Mult(double coef) const {
    auto result = make_unique<Impl>(Rows(), Columns());

#pragma omp parallel for num_threads(thread_limit_)
    for (size_t i = 0; i < body.size(); ++i)
      result->body[i] = body[i] * coef;

    return result;
  }

  ~Impl() = default;

 private:
  vector<double> body;
  size_t _rows;
  size_t _columns;
};

Matrix::Matrix(size_t rows, size_t columns) : _pimpl(make_unique<Impl>(rows, columns)) {}

Matrix::Matrix(Matrix&& other) noexcept = default;

Matrix& Matrix::operator=(Matrix&& other) noexcept = default;

size_t Matrix::Rows() const { return _pimpl->Rows(); }

size_t Matrix::Columns() const { return _pimpl->Columns(); }

double& Matrix::operator()(size_t row, size_t column) { return _pimpl->at(row, column); }

double Matrix::operator()(size_t row, size_t column) const { return _pimpl->at(row, column); }

Matrix Matrix::operator*(Matrix const& other) const { return Matrix(std::move(_pimpl->Mult(other))); }

Matrix Matrix::operator-(Matrix const& other) const { return Matrix(std::move(_pimpl->Sub(other))); }

Matrix Matrix::operator*(double coef) const { return Matrix(std::move(_pimpl->Mult(coef))); }

double Matrix::Norm() const { return _pimpl->Norm(); }

Matrix::Matrix(unique_ptr<Impl> pimpl) : _pimpl(std::move(pimpl)) {}

Matrix::~Matrix() = default;
}