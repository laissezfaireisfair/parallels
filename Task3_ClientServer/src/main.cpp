#include <functional>
#include <iostream>
#include <thread>
#include <future>
#include <queue>

int f(int arg) {
  return arg * arg;
}

std::queue<std::packaged_task<int()>> tasks_;
std::mutex mu;

void computeTasksJob(std::stop_token const& token) {
  while (!token.stop_requested()) {
    if (!tasks_.empty()) {
      std::scoped_lock lock(mu);
      auto task = std::move(tasks_.front());
      task();
      tasks_.pop();
      continue;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

void clientJob() {
  auto futures = std::vector<std::future<int>>();

  for (int i = 0; i < 5; ++i) {
    auto task = std::packaged_task([i] { return f(i); });
    futures.push_back(task.get_future());
    {
      std::scoped_lock lock(mu);
      tasks_.push(std::move(task));
    }
  }

  for (auto& future : futures) {
    std::cout << future.get() << std::endl;
  }
}

int main() {
  auto compute_tasks_thread = std::jthread(computeTasksJob);

  auto client_thread = std::jthread(clientJob);

  client_thread.join();

  compute_tasks_thread.request_stop();

  return 0;
}
