#include <iostream>
#include <stdexcept>
#include <format>
#include <vector>

struct CmdArguments {
  double accuracy;
  size_t grid_size;
  size_t iterations_count;
};

class Grid {
 public:
  explicit Grid(size_t size) : size_(size), body_(size * size) {}

  size_t Size() const {
    return size_;
  }

  double& operator()(size_t row, size_t column) {
    return body_.at(row * size_ + column);
  }

  double const& operator()(size_t row, size_t column) const {
    return body_.at(row * size_ + column);
  }

 private:
  size_t size_;
  std::vector<double> body_;
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

  Grid Run() {
    auto grid = CreateStartGrid();
    auto grid_after_step = grid;
#pragma acc enter data copyin(grid, grid_after_step)

    for (size_t i = 0; i < arguments_.iterations_count; ++i) {
      auto diff = RunOneIter(grid, grid_after_step);

      std::swap(grid, grid_after_step);

      if (diff < arguments_.accuracy)
        break;
    }
#pragma acc exit data delete(grid, grid_after_step)
    return grid;
  }

  ~Application() = default;

 private:
  CmdArguments arguments_;

  Grid CreateStartGrid() const {
    auto grid = Grid(arguments_.grid_size);
    auto lastIndex = grid.Size() - 1;

    grid(0, 0) = 10;
    grid(0, lastIndex) = 20;
    grid(lastIndex, 0) = 30;
    grid(lastIndex, lastIndex) = 20;

    double diff_top = (grid(0, lastIndex) - grid(0, 0)) /
        static_cast<double>(lastIndex);
    double diff_bottom = (grid(lastIndex, lastIndex) - grid(lastIndex, 0)) /
        static_cast<double>(lastIndex);
    double diff_left = (grid(lastIndex, 0) - grid(0, 0)) /
        static_cast<double>(lastIndex);
    double diff_right = (grid(lastIndex, lastIndex) - grid(0, lastIndex)) /
        static_cast<double>(lastIndex);
    for (size_t i = 1; i < lastIndex; ++i) {
      grid(0, i) = grid(0, i - 1) + diff_top;
      grid(lastIndex, i) = grid(lastIndex, i - 1) + diff_bottom;
      grid(i, 0) = grid(i - 1, 0) + diff_left;
      grid(i, lastIndex) = grid(i - 1, lastIndex) + diff_right;
    }

    return grid;
  }

  double RunOneIter(const Grid& grid, Grid& grid_after_step) const {
    double max_diff = 0.;

#pragma acc parallel loop reduction(max:max_diff) present(grid, grid_after_step)
    for (size_t i = 1; i < arguments_.grid_size - 1; ++i) {
#pragma acc loop
      for (size_t j = 1; j < arguments_.grid_size - 1; ++j) {
        grid_after_step(i, j) = (grid(i, j) +
            grid(i - 1, j) +
            grid(i + 1, j) +
            grid(i, j - 1) +
            grid(i, j + 1)
        ) / 5.;

        max_diff = std::max(max_diff, grid_after_step(i, j) - grid(i, j));
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
        std::cout << result(i, j) << " ";
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