#include <iostream>
#include "Server.h"

using parallels::Server;
using std::packaged_task;
using std::cout;
using std::endl;

int main() {
  auto server = Server<int>();
  server.Start();

  auto task = packaged_task<int()>([]() { return 1000 * 1000; });
  auto task_id = server.AddTask(std::move(task));

  auto result = server.RequestResult(task_id);
  cout << result << endl;

  server.Stop();

  return 0;
}
