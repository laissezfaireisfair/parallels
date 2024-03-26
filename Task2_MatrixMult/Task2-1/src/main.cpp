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
      matrix(i, j) = i == j ? 1 : 0;

  return matrix;
}

int main(int argc, char* argv[]) {
  // TODO: Read matrices from specified files

  auto matrix1 = GetMockLeftMatrix();
  auto matrix2 = GetMockRightMatrix();

  auto multResult = matrix1 * matrix2;

  cout << multResult.Rows() << "x" << multResult.Columns() << endl;
  for (int i = 0; i < multResult.Rows(); ++i) {
    for (int j = 0; j < multResult.Columns(); ++j)
      cout << multResult(i, j);
    cout << endl;
  }

  // TODO: Write result to specified file

  return 0;
}
