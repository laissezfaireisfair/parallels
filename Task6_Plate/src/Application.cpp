#include <stdexcept>
#include "Application.h"

namespace parallels {
using std::invalid_argument;
using std::make_unique;
using std::swap;
using std::max;

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

    for (size_t i = 0; i < arguments_.iterations_count; ++i) {
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
    auto grid = Grid(arguments_.grid_size);
    auto lastIndex = grid.Size() - 1;

    grid(0, 0) = 10;
    grid(0, lastIndex) = 20;
    grid(lastIndex, 0) = 30;
    grid(lastIndex, lastIndex) = 20;

    double diff_top = (grid(0, lastIndex) - grid(0, 0)) /
        static_cast<double>(lastIndex);
    double diff_bottom = (grid(lastIndex, lastIndex) - grid(lastIndex, 0)) /
        static_cast<double>(lastIndex);
    double diff_left = (grid(lastIndex, 0) - grid(0, 0)) /
        static_cast<double>(lastIndex);
    double diff_right = (grid(lastIndex, lastIndex) - grid(0, lastIndex)) /
        static_cast<double>(lastIndex);
    for (size_t i = 1; i < lastIndex; ++i) {
      grid(0, i) = grid(0, i - 1) + diff_top;
      grid(lastIndex, i) = grid(lastIndex, i - 1) + diff_bottom;
      grid(i, 0) = grid(i - 1, 0) + diff_left;
      grid(i, lastIndex) = grid(i - 1, lastIndex) + diff_right;
    }

    return grid;
  }

  double RunOneIter(Grid const& grid, Grid& grid_after_step) const {
    double max_diff = 0.;

    #pragma acc parallel loop reduction(max:max_diff)
    for (size_t i = 1; i < arguments_.grid_size - 1; ++i) {
      #pragma acc loop
      for (size_t j = 1; j < arguments_.grid_size - 1; ++j) {
        grid_after_step(i, j) = (grid(i, j) +
            grid(i - 1, j) +
            grid(i + 1, j) +
            grid(i, j - 1) +
            grid(i, j + 1)
        ) / 5.;

        max_diff = max(max_diff, grid_after_step(i, j) - grid(i, j));
      }
    }

    return max_diff;
  }
};

Application::Application(CmdArguments arguments) : pimpl_(make_unique<Impl>(arguments)) {}

Grid Application::Run() {
  return pimpl_->Run();
}

Application::~Application() = default;
}