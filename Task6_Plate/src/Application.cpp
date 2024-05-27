#include "Application.h"

namespace parallels {
using std::make_unique;

class Application::Impl {
 public:
  explicit Impl(CmdArguments arguments) : arguments_(arguments) {}

  void Run() {

  }

 private:
  CmdArguments arguments_;
};

Application::Application(CmdArguments arguments) : pimpl_(make_unique<Impl>(arguments)) {}

void Application::Run() {
  pimpl_->Run();
}

Application::~Application() = default;
}