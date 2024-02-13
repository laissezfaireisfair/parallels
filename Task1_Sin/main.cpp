#include <iostream>
#include <cmath>

#ifdef APPROX
using fPoint = float;
#else
using fPoint = double;
#endif

int main() {
  constexpr int kResolution = 1e7;

  auto* sin_values = new fPoint[kResolution];
  double period = 2. * std::numbers::pi;
  double step = period / kResolution;

  fPoint sum = 0;
  for (int i = 0; i < kResolution; ++i) {
    sin_values[i] = (fPoint) sin(step * i);
    sum += sin_values[i];
  }

  std::cout << sum << std::endl;

  delete[] sin_values;
  return 0;
}
