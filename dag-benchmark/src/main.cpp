#include "./algorithms/bitdp.hpp"
#include "./algorithms/greedy.hpp"
#include "./algorithms/lb.hpp"
#include "./algorithms/median-dag.hpp"
#include "./config.hpp"
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
  case bitdp:
    return "bitdp";
  case greedy:
    return "greedy";
  case lb:
    return "lower_bound";
  case median:
    return "median";
  }
  assert(false);
}

int main(int argc, char **argv) {
  std::vector<Dag> dags = dag_file(argc, argv);
  int n = dags[0].size() - 1;

  std::cout << "Alg             Score           Time (ms)       Edges" << std::endl;
  std::cout << std::fixed << std::setprecision(3);
  for (Alg alg : algs) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    Dag dag;
    switch (alg) {
    case bitdp:
      dag = bitdp_dag(dags, n, P, Q);
      break;
    case greedy:
      dag = greedy_dag(dags, n, P, Q);
      break;
    case lb:
      dag = lb_dag(dags, n, P, Q);
      break;
    case median:
      dag = median_dag(dags, n, P, Q);
      break;
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if (alg != lb) assert(!cycle(dag, n));
    int num_edges{0};
    for (int x = 1; x <= n; ++x) num_edges += dag[x].size();
    double edit_dist = edit_distance(dags, n, P, Q, dag);
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    std::cout << std::left << std::setw(16) << to_string(alg);
    std::cout << std::left << std::setw(16) << edit_dist;
    std::cout << std::left << std::setw(16) << ms;
    std::cout << std::left << std::setw(16) << num_edges << std::endl;
  }
}
