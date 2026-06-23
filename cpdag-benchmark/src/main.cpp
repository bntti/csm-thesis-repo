#include "./algorithms/bb.hpp"
#include "./algorithms/greedy.hpp"
#include "./algorithms/lb.hpp"
#include "./algorithms/median.hpp"
#include "./algorithms/threshold.hpp"
#include "./config.hpp"
#include "./convert.hpp"
#include "./read-dag-file.hpp"
#include "./util.hpp"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

using Dag = std::vector<std::vector<int>>;

std::string to_string(Alg alg) {
  switch (alg) {
  case bb:
    return "branch-and-bound";
  case lower_bound:
    return "lower-bound";
  case median:
    return "median";
  case threshold:
    return "threshold";
  case dag_greedy:
    return "DAG-greedy";
  }
  assert(false);
}

int main(int argc, char **argv) {
  std::vector<Dag> dags = dag_file(argc, argv);
  int n = dags[0].size() - 1;
  std::vector<std::vector<std::vector<int>>> cpdags;
  for (auto v : dags) cpdags.push_back(convert(v, n));

  std::cout << "Alg                 Score           Time (ms)       Edges" << std::endl;
  std::cout << std::fixed << std::setprecision(3);
  for (Alg alg : algs) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    Dag dag;
    switch (alg) {
    case bb:
      dag = bb_solve(dags, cpdags, n);
      break;
    case lower_bound:
      dag = lb_dag(cpdags, n);
      break;
    case median:
      dag = median_solve(dags, cpdags, n);
      break;
    case threshold:
      dag = threshold_dag(dags, n);
      break;
    case dag_greedy:
      dag = greedy_dag(dags, n);
      break;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if (alg != lower_bound) assert(!cycle(dag, n));
    int num_edges{0};
    for (int x = 1; x <= n; ++x) num_edges += dag[x].size();

    Dag cpdag = alg != lower_bound ? convert(dag, n) : dag;
    int edit_dist = edit_distance(cpdags, n, cpdag);
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    std::cout << std::left << std::setw(20) << to_string(alg);
    std::cout << std::left << std::setw(16) << edit_dist;
    std::cout << std::left << std::setw(16) << ms;
    std::cout << std::left << std::setw(16) << num_edges << std::endl;
  }
}
