#include <iostream>
#include <stdexcept>
#include <format>
#include <vector>
#include <memory>

struct CmdArguments {
  double accuracy;
  size_t grid_size;
  size_t iterations_count;
};

class Application {
 public:
  explicit Application(CmdArguments arguments) : arguments_(arguments) {
    if (arguments_.accuracy < 1e-6)
      throw std::invalid_argument("Accuracy should be more than 10^-6");
    if (static_cast<double>(arguments_.iterations_count) > 1e6)
      throw std::invalid_argument("Iterations count should be less than 10^6");
    if (arguments_.grid_size > 1024)
      throw std::invalid_argument("Grid size count should be less than 1024");
  }

  std::unique_ptr<double[]> Run() {
    auto grid = CreateStartGrid().release();
    auto grid_after_step = CreateStartGrid().release();

#pragma acc data copy(grid[:arguments_.grid_size * arguments_.grid_size], grid_after_step[:arguments_.grid_size * arguments_.grid_size])
    {
      for (size_t i = 0; i < arguments_.iterations_count; ++i) {
        auto diff = RunOneIter(grid, grid_after_step);

        std::swap(grid, grid_after_step);

        if (diff < arguments_.accuracy)
          break;
      }
    }

    return std::unique_ptr<double[]>(grid);
  }

  ~Application() = default;

 private:
  CmdArguments arguments_;

  inline size_t idx(size_t str, size_t col) const {
    return str * arguments_.grid_size + col;
  }

  std::unique_ptr<double[]> CreateStartGrid() const {
    auto grid = std::make_unique<double[]>(arguments_.grid_size * arguments_.grid_size);
    auto lastIndex = arguments_.grid_size - 1;

    grid[idx(0, 0)] = 10;
    grid[idx(0, lastIndex)] = 20;
    grid[idx(lastIndex, 0)] = 30;
    grid[idx(lastIndex, lastIndex)] = 20;

    double diff_top = (grid[idx(0, lastIndex)] - grid[idx(0, 0)]) /
        static_cast<double>(lastIndex);
    double diff_bottom = (grid[idx(lastIndex, lastIndex)] - grid[idx(lastIndex, 0)]) /
        static_cast<double>(lastIndex);
    double diff_left = (grid[idx(lastIndex, 0)] - grid[idx(0, 0)]) /
        static_cast<double>(lastIndex);
    double diff_right = (grid[idx(lastIndex, lastIndex)] - grid[idx(0, lastIndex)]) /
        static_cast<double>(lastIndex);
    for (size_t i = 1; i < lastIndex; ++i) {
      grid[idx(0, i)] = grid[idx(0, i - 1)] + diff_top;
      grid[idx(lastIndex, i)] = grid[idx(lastIndex, i - 1)] + diff_bottom;
      grid[idx(i, 0)] = grid[idx(i - 1, 0)] + diff_left;
      grid[idx(i, lastIndex)] = grid[idx(i - 1, lastIndex)] + diff_right;
    }

    return grid;
  }

  double RunOneIter(double* grid, double* grid_after_step) const {
    double max_diff = 0.;

#pragma acc parallel loop reduction(max:max_diff) present(grid, grid_after_step)
    for (size_t i = 1; i < arguments_.grid_size - 1; ++i) {
#pragma acc loop
      for (size_t j = 1; j < arguments_.grid_size - 1; ++j) {
        grid_after_step[idx(i, j)] = (grid[idx(i, j)] +
            grid[idx(i - 1, j)] +
            grid[idx(i + 1, j)] +
            grid[idx(i, j - 1)] +
            grid[idx(i, j + 1)]
        ) / 5.;

        max_diff = std::max(max_diff, grid_after_step[idx(i, j)] - grid[idx(i, j)]);
      }
    }

    return max_diff;
  }
};

CmdArguments ParseArguments(int argc, char* argv[]) {
  if (argc != 4)
    throw std::invalid_argument("Accuracy, grid_size and iterations_count parameters expected");

  return {
      .accuracy = std::stod(argv[1]),
      .grid_size = std::stoul(argv[2]),
      .iterations_count = std::stoul(argv[3])
  };
}

int main(int argc, char* argv[]) {
  try {
    auto arguments = ParseArguments(argc, argv);

    Application application(arguments);

    auto result = application.Run();

    for (size_t i = 0; i < arguments.grid_size; ++i) {
      for (size_t j = 0; j < arguments.grid_size; ++j)
        std::cout << result[i * arguments.grid_size + j] << " ";
      std::cout << std::endl;
    }

    return 0;
  }
  catch (std::exception& exception) {
    std::cerr << "Exception: " << exception.what() << std::endl;
  }
  catch (...) {
    std::cerr << "Unknown exception" << std::endl;
  }
}