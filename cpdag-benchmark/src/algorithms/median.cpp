#include "./median.hpp"
#include "../util.hpp"
#include <vector>

using Dag = std::vector<std::vector<int>>;

Dag median_solve(std::vector<Dag> &vs, std::vector<Dag> &cpdags, int n) {
  std::vector<std::vector<int>> directed(n + 1);
  std::vector<std::vector<int>> undirected(n + 1);
  for (int i = 1; i <= n; ++i) {
    directed[i].resize(n + 1);
    undirected[i].resize(n + 1);
    std::fill(directed[i].begin(), directed[i].end(), 0);
    std::fill(undirected[i].begin(), undirected[i].end(), 0);
  }

  int total_edges = 0;
  for (auto v : cpdags) {
    for (int x = 1; x <= n; ++x) {
      for (int c : v[x]) {
        if (connected(v, c, x)) {
          ++undirected[x][c];
          if (x < c) total_edges += 1;
        } else {
          ++directed[x][c];
          total_edges += 1;
        }
      }
    }
  }

  int best_v = -1;
  int min_cost = 1e9;
  for (int i = 0; i < (int)cpdags.size(); ++i) {
    Dag &cpdag = cpdags[i];
    int dag_count = cpdags.size();
    int cost = 0;
    int edges_used = 0;
    for (int x = 1; x <= n; ++x) {
      for (int c : cpdag[x]) {
        int same = directed[x][c];
        int rev = directed[c][x];
        int undir = undirected[x][c];
        int none = dag_count - same - rev - undir;
        if (connected(cpdag, c, x)) {
          if (c < x) continue;
          edges_used += same + rev + undir;
          cost += none + same + rev;
        } else {
          cost += rev + undir + none;
          edges_used += same + rev + undir;
        }
      }
    }
    int t_missing = total_edges - edges_used; // Number of unmatched edges
    cost += t_missing;

    if (cost < min_cost) {
      min_cost = cost;
      best_v = i;
    }
  }

  return vs[best_v];
}