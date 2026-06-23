#include "./median-dag.hpp"
#include <vector>

std::vector<std::vector<int>> median_dag(std::vector<std::vector<std::vector<int>>> &vs, int n,
                                         double p, double q) {

  std::vector<std::vector<int>> directed(n + 1);
  for (int i = 1; i <= n; ++i) {
    directed[i].resize(n + 1);
    std::fill(directed[i].begin(), directed[i].end(), 0);
  }

  int total_edges = 0;
  for (auto v : vs) {
    for (int x = 1; x <= n; ++x) {
      total_edges += v[x].size();
      for (int c : v[x]) {
        ++directed[x][c];
      }
    }
  }

  int best_v = -1;
  double min_cost = 1e9;
  for (int i = 0; i < (int)vs.size(); ++i) {
    auto &v = vs[i];
    int dag_count = vs.size();
    int r_total = 0;
    int p_total = 0;
    int edges_used = 0;
    int v_edges = 0;
    for (int x = 1; x <= n; ++x) {
      v_edges += v[x].size();
      for (int c : v[x]) {
        int same = directed[x][c];
        int rev = directed[c][x];
        int none = dag_count - same - rev;
        r_total += rev;
        p_total += none;
        edges_used += same + rev;
      }
    }
    int t_pairs = n * (n - 1) / 2 - v_edges;  // Number of uncomputed node pairs
    int t_missing = total_edges - edges_used; // Number of unmatched edges
    p_total += t_missing;
    int q_total = (dag_count * t_pairs - t_missing);

    double cost = r_total + p_total * p + q_total * q;
    if (cost < min_cost) {
      min_cost = cost;
      best_v = i;
    }
  }

  return vs[best_v];
}