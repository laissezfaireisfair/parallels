#pragma once
#include <memory>
#include "CmdArguments.h"

namespace parallels {
using std::unique_ptr;

class Application {
 public:
  explicit Application(CmdArguments arguments);

  void Run();

  ~Application();

 private:
  class Impl;
  unique_ptr<Impl> pimpl_;
};
}