#include <iostream>
#include <cmath>

#ifdef APPROX
using fPoint = float;
fPoint calcSin(fPoint arg) {
  return sinf(arg);
}
#else
using fPoint = double;
fPoint calcSin(fPoint arg) {
  return sin(arg);
}
#endif

using std::cout;
using std::endl;

int main() {
  constexpr int resolution = 10e7;
  
  auto* sinValues = new fPoint[resolution];
  fPoint period = 2. * std::numbers::pi;
  fPoint step = period / resolution;

  for (int i = 0; i < resolution; ++i)
    sinValues[i] = calcSin(step * i);

  fPoint sum = 0;
  for (int i = 0; i < resolution; ++i)
    sum += sinValues[i];

  delete[] sinValues;

  cout << sum << endl;

  return 0;
}
