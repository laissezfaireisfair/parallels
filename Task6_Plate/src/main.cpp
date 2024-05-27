#include <iostream>
#include <stdexcept>
#include "Application.h"

using std::stod;
using std::cerr;
using std::endl;
using std::exception;
using std::invalid_argument;
using parallels::CmdArguments;
using parallels::Application;

CmdArguments ParseArguments(int argc, char* argv[]) {
  if (argc != 4)
    throw invalid_argument("Accuracy, grid_size and iterations_count parameters expected");

  return {
      .accuracy = stod(argv[1]),
      .grid_size = stod(argv[2]),
      .iterations_count = stod(argv[3])
  };
}

int main(int argc, char* argv[]) {
  try {
    auto arguments = ParseArguments(argc, argv);

    Application application(arguments);

    application.Run();

    return 0;
  }
  catch (exception& exception) {
    cerr << "Exception: " << exception.what() << endl;
  }
  catch (...) {
    cerr << "Unknown exception" << endl;
  }
}