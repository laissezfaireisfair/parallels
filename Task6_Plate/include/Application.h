#pragma once
#include <memory>
#include "CmdArguments.h"
#include "SquareMatrix.h"

namespace parallels {
using std::unique_ptr;

class Application {
 public:
  explicit Application(CmdArguments arguments);

  SquareMatrix Run();

  ~Application();

 private:
  class Impl;
  unique_ptr<Impl> pimpl_;
};
}