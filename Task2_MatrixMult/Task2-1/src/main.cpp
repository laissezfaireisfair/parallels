#include <iostream>

#include "matrix.h"

using parallels::Matrix;
using std::cout;
using std::endl;

Matrix GetMockLeftMatrix() {
  auto matrix = Matrix(1, 5);

  for (int i = 0; i < 5; ++i)
    matrix(0, i) = i;

  return matrix;
}

Matrix GetMockRightMatrix() {
  auto matrix = Matrix(5, 5);

  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 5; ++j)
      matrix(i, j) = i == j ? i : 0;

  return matrix;
}

void PrintMatrix(Matrix const& matrix) {
  cout << matrix.Rows() << "x" << matrix.Columns() << endl;

  for (int i = 0; i < matrix.Rows(); ++i) {
    for (int j = 0; j < matrix.Columns(); ++j)
      cout << matrix(i, j) << " ";
    cout << endl;
  }
}

int main(int argc, char* argv[]) {
  // TODO: Read matrices from specified files

  auto matrix1 = GetMockLeftMatrix();
  PrintMatrix(matrix1);
  auto matrix2 = GetMockRightMatrix();
  PrintMatrix(matrix2);

  auto multResult = matrix1 * matrix2;

  PrintMatrix(multResult);

  // TODO: Write result to specified file

  return 0;
}
