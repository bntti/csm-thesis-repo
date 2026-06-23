#include "./greedy.hpp"
#include "../util.hpp"
#include <algorithm>
#include <vector>

std::vector<std::vector<int>> greedy_dag(std::vector<std::vector<std::vector<int>>> &vs, int n,
                                         double p, double q) {

  Edges regrets = calculate_regrets(vs, n, p, q);
  std::sort(regrets.rbegin(), regrets.rend());

  std::vector<std::vector<int>> v(n + 1);
  for (auto t : regrets) {
    auto [x, c, cst] = t.second;
    v[x].push_back(c);
    if (node_cycle(v, n, x)) v[x].pop_back();
  }

  return v;
}