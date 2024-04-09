#include <vector>
#include <stdexcept>
#include <thread>

#include "appSettings.h"
#include "matrix.h"

namespace parallels {
using std::make_unique;
using std::vector;
using std::invalid_argument;
using std::thread;
using std::min;

class Matrix::Impl {
 public:
  Impl(size_t rows, size_t columns) : body(rows * columns, 0.),
                                      _rows(rows),
                                      _columns(columns) {}

  size_t Rows() const { return _rows; }

  size_t Columns() const { return _columns; }

  double& at(size_t row, size_t column) { return body.at(row * _columns + column); }

  unique_ptr<Impl> Mult(Matrix const& other) {
    if (Columns() != other.Rows())
      throw invalid_argument("Cannot multiply matrices with such shapes");

    auto result = make_unique<Impl>(Rows(), other.Columns());


    size_t const columnsPerTask = other.Columns() / AppSettings::max_threads_count_
        + (other.Columns() % AppSettings::max_threads_count_ ? 1 : 0);
    auto taskByBorders
        = [this, &other, &result](size_t fromColumn, size_t toColumn) {
          for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = fromColumn; j < toColumn; ++j) {
              double sum = 0;
              for (size_t k = 0; k < Columns(); ++k)
                sum += at(i, k) * other(k, j);
              result->at(i, j) = sum;
            }
          }
        };

    auto threads = vector<thread>();
    for (size_t i = 0; i < other.Columns(); i += columnsPerTask)
      threads.emplace_back(taskByBorders, i, min(other.Columns(), i + columnsPerTask));

    for (auto& thread : threads)
      thread.join();

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

Matrix::Matrix(unique_ptr<Impl> pimpl) : _pimpl(std::move(pimpl)) {}

Matrix::~Matrix() = default;
}