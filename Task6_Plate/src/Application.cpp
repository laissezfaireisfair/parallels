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
    auto grid_after_step = Grid(arguments_.grid_size);

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

    matrix(0,0) = 10;
    matrix(0, lastIndex) = 20;
    matrix(lastIndex, 0) = 30;
    matrix(lastIndex, lastIndex) = 20;

    return matrix;
  }

  double RunOneIter(Grid const& grid, Grid& grid_after_step) const {
    // TODO: Implement
    return arguments_.accuracy / 2.;
  }
};

Application::Application(CmdArguments arguments) : pimpl_(make_unique<Impl>(arguments)) {}

Grid Application::Run() {
  return pimpl_->Run();
}

Application::~Application() = default;
}