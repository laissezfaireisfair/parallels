#pragma once

#include <queue>
#include <future>
#include <thread>
#include <unordered_map>
#include <optional>
#include <functional>

namespace parallels {
using std::bind;
using std::placeholders::_1;
using std::nullopt;
using std::packaged_task;
using std::scoped_lock;
using std::logic_error;
using std::future;
using std::optional;
using std::jthread;
using std::queue;
using std::unordered_map;
using std::size_t;
using std::mutex;
using std::stop_token;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

template<class T>
class Server {
 public:
  void Start() {
    if (computingThread)
      return;

    computingThread.emplace(bind(&Server::computeTasksJob, this, _1));
  }

  void Stop() {
    if (!computingThread)
      return;

    computingThread->request_stop();
    computingThread = nullopt;
  }

  size_t AddTask(packaged_task<T()> task) {
    scoped_lock lock(tasks_mutex_);

    auto new_id = id_counter_++;
    {
      scoped_lock futures_lock(futures_mutex_);
      future_by_id[new_id] = task.get_future();
    }
    tasks_.push(std::move(task));

    return new_id;
  }

  T RequestResult(size_t task_id) {
    if (!future_by_id.contains(task_id))
      throw logic_error("No such result. May be already ejected");

    future<T> future;
    {
      scoped_lock futures_lock(futures_mutex_);
      future = std::move(future_by_id[task_id]);
      future_by_id.erase(task_id);
    }

    return future.get();
  }

 private:
  optional<jthread> computingThread;
  queue<packaged_task<T()>> tasks_;
  unordered_map<size_t, future<T>> future_by_id;
  size_t id_counter_ = 0;
  mutex tasks_mutex_;
  mutex futures_mutex_;

  void computeTasksJob(stop_token const& token) {
    while (!token.stop_requested()) {
      if (!tasks_.empty()) {
        scoped_lock lock(tasks_mutex_);
        auto task = std::move(tasks_.front());
        task();
        tasks_.pop();
        continue;
      }

      sleep_for(milliseconds(200));
    }
  }
};
}