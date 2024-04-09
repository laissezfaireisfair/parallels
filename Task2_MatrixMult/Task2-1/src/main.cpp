#include <iostream>
#include <fstream>

#include "matrix.h"

using parallels::Matrix;
using std::endl;
using std::ofstream;
using std::ostream;

const size_t kMockVectorSize = 20'000;

Matrix GetMockLeftMatrix() {
  auto matrix = Matrix(1, kMockVectorSize);

  for (size_t i = 0; i < kMockVectorSize; ++i)
    matrix(0, i) = static_cast<double>(i);

  return matrix;
}

Matrix GetMockRightMatrix() {
  auto matrix = Matrix(kMockVectorSize, kMockVectorSize);

  for (size_t i = 0; i < kMockVectorSize; ++i)
    matrix(i, i) = static_cast<double>(i);

  return matrix;
}

void PrintMatrix(Matrix const& matrix, ostream& stream) {
  stream << matrix.Rows() << "x" << matrix.Columns() << endl;

  for (int i = 0; i < matrix.Rows(); ++i) {
    for (int j = 0; j < matrix.Columns(); ++j)
      stream << matrix(i, j) << " ";
    stream << endl;
  }
}

int main() {
  auto matrix1 = GetMockLeftMatrix();
  auto matrix2 = GetMockRightMatrix();

  auto stream = ofstream("output.txt");

  auto multResult = matrix1 * matrix2;
  PrintMatrix(multResult, stream);

  stream.close();

  return 0;
}
