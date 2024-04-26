#include <fstream>
#include <cmath>
#include <random>
#include "Server.h"

using parallels::Server;
using std::vector;
using std::string;
using std::size_t;
using std::packaged_task;
using std::jthread;
using std::sin;
using std::sqrt;
using std::pow;
using std::ofstream;
using std::endl;
using std::pair;
using std::to_string;
using std::uniform_real_distribution;
using std::default_random_engine;
using std::numbers::pi;

double GetRandArgument(double from, double to) {
  static int seed = 1;
  seed++;
  uniform_real_distribution<double> distribution(from, to);
  default_random_engine random_engine(seed);
  double random_argument = distribution(random_engine);
  return random_argument;
}

pair<string, packaged_task<double()>> GetSinTask() {
  auto argument = GetRandArgument(-pi, pi);
  auto expression = "sin " + to_string(argument);
  auto task = packaged_task<double()>([argument]() { return sin(argument); });
  return {std::move(expression), std::move(task)};
}

pair<string, packaged_task<double()>> GetSqrtTask() {
  auto argument = GetRandArgument(0., 1000.);
  auto expression = "sqrt " + to_string(argument);
  auto task = packaged_task<double()>([argument]() { return sqrt(argument); });
  return {std::move(expression), std::move(task)};
}

pair<string, packaged_task<double()>> GetPowTask() {
  double base = GetRandArgument(1., 10.);
  double power = GetRandArgument(0., 100.);
  auto expression = "pow " + to_string(base) + " " + to_string(power);
  auto task = packaged_task<double()>([base, power]() { return pow(base, power); });
  return {std::move(expression), std::move(task)};
}

void AbstractClientJob(Server<double>& server,
                       const string& foutName,
                       pair<string, packaged_task<double()>> (* get_task)()) {
  const int kCreatedTasksCount = 10000;

  auto task_ids_and_expressions = vector<pair<size_t, string>>();
  for (int i = 0; i < kCreatedTasksCount; ++i) {
    auto [expression, task] = get_task();
    auto task_id = server.AddTask(std::move(task));
    task_ids_and_expressions.emplace_back(task_id, std::move(expression));
  }

  ofstream fout(foutName);
  for (auto& [task_id, expression] : task_ids_and_expressions) {
    auto result = server.RequestResult(task_id);
    fout << expression << ": " << result << endl;
  }
  fout.close();
}

void SinClientJob(Server<double>& server) {
  const string filename = "sins.txt";
  AbstractClientJob(server, filename, GetSinTask);
}

void SqrtClientJob(Server<double>& server) {
  const string filename = "sqrts.txt";
  AbstractClientJob(server, filename, GetSqrtTask);
}

void PowClientJob(Server<double>& server) {
  const string filename = "pows.txt";
  AbstractClientJob(server, filename, GetPowTask);
}

int main() {
  auto server = Server<double>();
  server.Start();

  jthread sin_client_thread([&server] { return SinClientJob(server); });
  jthread sqrt_client_thread([&server] { return SqrtClientJob(server); });
  jthread pow_client_thread([&server] { return PowClientJob(server); });

  sin_client_thread.join();
  sqrt_client_thread.join();
  pow_client_thread.join();

  server.Stop();

  return 0;
}
