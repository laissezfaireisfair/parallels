#include "Application.h"

namespace parallels {
using std::make_unique;
using std::swap;

class Application::Impl {
 public:
  explicit Impl(CmdArguments arguments) : arguments_(arguments) {}

  SquareMatrix Run() {
    auto grid = CreateStartGrid();
    auto grid_after_step = SquareMatrix(arguments_.grid_size);

    while (true) {
      auto diff = RunOneIter(grid, grid_after_step);
      if (diff < arguments_.accuracy)
        break;

      swap(grid, grid_after_step);
    }

    return grid_after_step;
  }

 private:
  CmdArguments arguments_;

  SquareMatrix CreateStartGrid() const {
    // TODO: Implement
    return SquareMatrix(arguments_.grid_size);
  }

  double RunOneIter(SquareMatrix const& grid, SquareMatrix& grid_after_step) const {
    // TODO: Implement
    return arguments_.accuracy / 2.;
  }
};

Application::Application(CmdArguments arguments) : pimpl_(make_unique<Impl>(arguments)) {}

SquareMatrix Application::Run() {
  return pimpl_->Run();
}

Application::~Application() = default;
}