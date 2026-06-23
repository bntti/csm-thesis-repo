#include "./lb.hpp"
#include "../util.hpp"
#include <algorithm>
#include <vector>

std::vector<std::vector<int>> lb_dag(std::vector<std::vector<std::vector<int>>> &vs, int n,
                                     double p, double q) {

  Edges regrets = calculate_regrets(vs, n, p, q);
  std::sort(regrets.rbegin(), regrets.rend());

  std::vector<std::vector<int>> v(n + 1);
  for (auto t : regrets) {
    auto [x, c, cst] = t.second;
    // Avoid adding both edges. Sorted by regret so is fine.
    if (!connected(v, c, x)) v[x].push_back(c);
  }

  return v;
}