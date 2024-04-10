#include <iostream>
#include <cmath>

using std::invalid_argument;
using std::stoi;
using std::exception;
using std::cout;
using std::endl;
using std::size_t;
using std::numbers::pi;
using std::sin;

static size_t maxThreadsCount;
const size_t kResolution = 40'000'000;

double integrate(double (* func)(double), double from, double to) {
  const double step = (to - from) / kResolution;
  double sum = 0.;

#pragma omp parallel for num_threads(maxThreadsCount)
  for (size_t i = 0; i < kResolution; ++i) {
    const double left = static_cast<double>(i) * step;
    const double right = left + step;
    double const segment = (right - left) / 6. * (func(left) + 4 * func((left + right) / 2.) + func(right));
#pragma omp atomic
    sum += segment;
  }

  return sum;
}

int main(int argc, char* argv[]) {
  try {
    if (argc != 2)
      throw invalid_argument("Expected max thread count as argument");
    maxThreadsCount = stoi(argv[1]);

    auto integrableFunction = [](double x) {
      return sin(x);
    };
    cout << integrate(integrableFunction, 0., pi) << endl;
  }
  catch (exception& exception) {
    cout << exception.what() << endl;
  }
  catch (...) {
    cout << "Unknown exception" << endl;
  }

  return 0;
}
