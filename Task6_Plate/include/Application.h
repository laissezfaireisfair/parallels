#pragma once
#include "CmdArguments.h"
#include "Grid.h"

namespace parallels {

class Application {
 public:
  explicit Application(CmdArguments arguments);

  Grid Run();

  ~Application();

 private:
  CmdArguments arguments_;

  Grid CreateStartGrid() const;

  double RunOneIter(const Grid& grid, Grid& grid_after_step) const;
};
}