#include "./p-change.hpp"
#include "./util.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

using Dag = std::vector<std::vector<int>>;

Dag p_create(int n, double p_edge) {
  Dag v(n + 1);

  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      if (roll(p_edge)) v[x].push_back(c);
    }
  }

  return v;
}

Dag p_change_alg(Dag &original, int n, double p_edge, double p_remove, int n_swaps) {
  assert(n_swaps == 0); // n_swaps not used in the thesis

  double total_possible = n * (n + 1) / 2.0;
  double num_added = total_possible * p_edge;
  double num_removed = num_added * p_remove;
  double p_add = num_removed * (1 / (total_possible - num_added));

  Dag v(n + 1);

  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      if (connected(original, x, c) && !roll(p_remove)) v[x].push_back(c);
    }
  }

  std::vector<std::pair<int, int>> node_pairs;
  for (int x = 1; x <= n; ++x) {
    for (int c = 1; c <= n; ++c) {
      if (x != c && !connected(v, x, c)) node_pairs.push_back({x, c});
    }
  }
  std::shuffle(node_pairs.begin(), node_pairs.end(), rng);

  for (auto [x, c] : node_pairs) {
    if (roll(p_add)) {
      v[x].push_back(c);
      if (node_cycle(v, n, x)) v[x].pop_back();
    }
  }

  return v;
}