#pragma once

#include <queue>
#include <future>
#include <thread>
#include <unordered_map>

namespace parallels {
using std::size_t;
using std::queue;
using std::packaged_task;
using std::jthread;
using std::bind;
using std::pair;
using std::unordered_map;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::stop_token;
using std::mutex;

template<class T>
class Server {
 public:
  void Start() {
    is_active_ = true;

    while (is_active_) {
      if (!tasks_and_ids_.empty()) {
        auto pair = std::move(tasks_and_ids_.front());
        pair.second()();
        results_by_task_id_.emplace(pair.first(), pair.second().get_future().get());
        tasks_and_ids_.pop();
      }
      sleep_for(milliseconds(50));
    }
  }

  void Stop() {
    is_active_ = false;
  }

  size_t AddTask(packaged_task<T()> task) {
    tasks_and_ids_.emplace(std::move(task), ++id_counter_);
    return id_counter_;
  }

  T RequestResult(size_t task_id) {
    if (results_by_task_id_.contains(task_id))
      return results_by_task_id_[task_id];

    sleep_for(milliseconds(50));
  }

 private:
  queue<pair<size_t, packaged_task<T()>>> tasks_and_ids_;

  unordered_map<size_t, T> results_by_task_id_;

  size_t id_counter_ = 0;

  mutex mu;

  void processTasks
};
}