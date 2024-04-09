#include <iostream>

#include "matrix.h"

using parallels::Matrix;
using std::stoul;
using std::invalid_argument;
using std::cout;
using std::endl;
using std::exception;

Matrix GetMockMatrix(size_t equations_count) {
  auto matrix = Matrix(equations_count, equations_count);

  for (size_t i = 0; i < equations_count; ++i)
    for (size_t j = 0; j < equations_count; ++j)
      matrix(i, j) = i == j ? 2. : 1.;

  return matrix;
}

Matrix GetMockRightVector(size_t equations_count) {
  auto matrix = Matrix(equations_count, 1);

  for (size_t i = 0; i < equations_count; ++i)
    matrix(i, 0) = static_cast<double>(equations_count + 1);

  return matrix;
}

Matrix SolveSystem(Matrix const& matrix, Matrix const& right_vector) {
  const double kAccuracy = 1e-10;
  const double tau = 2e-4;

  auto solution = Matrix(right_vector.Rows(), 1);

  while (true) {
    auto tauMultiplicator = matrix * solution - right_vector;
    if (tauMultiplicator.Norm() / right_vector.Norm() < kAccuracy)
      break;

    solution = solution - tauMultiplicator * tau;
  }

  return solution;
}

int main(int argc, char* argv[]) {
  const size_t kEquationsCount = 5'000;

  try {
    if (argc != 2)
      throw invalid_argument("Thread limit expected");
    Matrix::thread_limit_ = stoul(argv[1]);

    auto matrix = GetMockMatrix(kEquationsCount);
    auto right_vector = GetMockRightVector(kEquationsCount);

    auto solution = SolveSystem(matrix, right_vector);
  }
  catch (exception& exception) {
    cout << exception.what() << endl;
  }
  catch (...) {
    cout << "Unknown exception" << endl;
  }

  return 0;
}
