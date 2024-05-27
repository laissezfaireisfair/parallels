#include <stdexcept>
#include "Application.h"

namespace parallels {
using std::invalid_argument;
using std::make_unique;
using std::swap;

class Application::Impl {
 public:
  explicit Impl(CmdArguments arguments) : arguments_(arguments) {
    if (arguments_.accuracy <= 1e-6)
      throw invalid_argument("Accuracy should be more than 10^-6");
    if (static_cast<double>(arguments_.iterations_count) >= 1e6)
      throw invalid_argument("Iterations count should be less than 10^6");
    if (arguments_.grid_size >= 1024)
      throw invalid_argument("Grid size count should be less than 1024");
  }

  Grid Run() {
    auto grid = CreateStartGrid();
    auto grid_after_step = grid;

    for (int i = 0; i < arguments_.iterations_count; ++i) {
      auto diff = RunOneIter(grid, grid_after_step);

      swap(grid, grid_after_step);

      if (diff < arguments_.accuracy)
        break;
    }

    return grid;
  }

 private:
  CmdArguments arguments_;

  Grid CreateStartGrid() const {
    auto matrix = Grid(arguments_.grid_size);
    auto lastIndex = matrix.Size() - 1;

    matrix(0, 0) = 10;
    matrix(0, lastIndex) = 20;
    matrix(lastIndex, 0) = 30;
    matrix(lastIndex, lastIndex) = 20;

    double diff_top = matrix(0, lastIndex) - matrix(0, 0) /
        static_cast<double>(lastIndex);
    double diff_bottom = matrix(lastIndex, lastIndex) - matrix(lastIndex, 0) /
        static_cast<double>(lastIndex);
    double diff_left = matrix(lastIndex, 0) - matrix(0, 0) /
        static_cast<double>(lastIndex);
    double diff_right = matrix(lastIndex, lastIndex) - matrix(0, lastIndex) /
        static_cast<double>(lastIndex);
    for (int i = 1; i <= lastIndex; ++i) {
      matrix(0, i) = matrix(0, i - 1) + diff_top;
      matrix(lastIndex, i) = matrix(lastIndex, i - 1) + diff_bottom;
      matrix(i, 0) = matrix(i - 1, 0) + diff_left;
      matrix(i, lastIndex) = matrix(i - 1, lastIndex) + diff_right;
    }

    return matrix;
  }

  double RunOneIter(Grid const& grid, Grid& grid_after_step) const {
    for (int i = 1; i < arguments_.grid_size - 1; ++i) {
      for (int j = 1; j < arguments_.grid_size - 1; ++j)
        grid_after_step(i, j) = (grid(i, j) +
            grid(i - 1, j) +
            grid(i + 1, j) +
            grid(i, j - 1) +
            grid(i, j + 1)
        ) / 5.;
    }
    return arguments_.accuracy / 2.;
  }
};

Application::Application(CmdArguments arguments) : pimpl_(make_unique<Impl>(arguments)) {}

Grid Application::Run() {
  return pimpl_->Run();
}

Application::~Application() = default;
}