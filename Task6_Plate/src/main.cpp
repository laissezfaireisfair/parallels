#include <iostream>
#include <stdexcept>
#include <format>
#include "Application.h"

using std::stod;
using std::stoul;
using std::cerr;
using std::cout;
using std::endl;
using std::format;
using std::exception;
using std::invalid_argument;
using parallels::CmdArguments;
using parallels::Application;

CmdArguments ParseArguments(int argc, char* argv[]) {
  if (argc != 4)
    throw invalid_argument("Accuracy, grid_size and iterations_count parameters expected");

  return {
      .accuracy = stod(argv[1]),
      .grid_size = stoul(argv[2]),
      .iterations_count = stoul(argv[3])
  };
}

int main(int argc, char* argv[]) {
  try {
    auto arguments = ParseArguments(argc, argv);

    Application application(arguments);

    auto result = application.Run();

    for (size_t i = 0; i < arguments.grid_size; ++i) {
      for (size_t j = 0; j < arguments.grid_size; ++j)
        cout << format("{:.4f}", result(i, j)) << " ";
      cout << endl;
    }

    return 0;
  }
  catch (exception& exception) {
    cerr << "Exception: " << exception.what() << endl;
  }
  catch (...) {
    cerr << "Unknown exception" << endl;
  }
}