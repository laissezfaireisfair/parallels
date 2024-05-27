#pragma once
#include <memory>
#include "CmdArguments.h"
#include "Grid.h"

namespace parallels {
using std::unique_ptr;

class Application {
 public:
  explicit Application(CmdArguments arguments);

  Grid Run();

  ~Application();

 private:
  class Impl;
  unique_ptr<Impl> pimpl_;
};
}