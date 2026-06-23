#include "./threshold.hpp"
#include "../util.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

std::vector<std::vector<int>> threshold_dag(std::vector<std::vector<std::vector<int>>> &vs, int n) {
  // Calculate edge occurrences
  std::vector<std::vector<int>> edge_count(n + 1);
  for (int i = 1; i <= n; ++i) {
    edge_count[i].resize(n + 1);
    std::fill(edge_count[i].begin(), edge_count[i].end(), 0);
  }

  std::vector<std::vector<int>> edge_union(n + 1);
  for (auto v : vs) {
    for (int x = 1; x <= n; ++x) {
      for (int c : v[x]) {
        if (edge_count[x][c] == 0) edge_union[x].push_back(c);
        edge_count[x][c]++;
      }
    }
  }

  // Sort all edges by occurrence count
  std::vector<std::tuple<int, int, int>> edges;
  for (int x = 1; x <= n; ++x) {
    for (int c : edge_union[x]) edges.push_back({edge_count[x][c], x, c});
  }
  std::sort(edges.rbegin(), edges.rend());

  // Binary search threshold count for edges
  int a = 1;
  int b = vs.size();
  while (a < b) {
    int c = (a + b) / 2;
    if (c == b) --c;

    std::vector<std::vector<int>> v(n + 1);
    for (auto [num, e1, e2] : edges) {
      if (num < c) break;
      v[e1].push_back(e2);
    }

    if (cycle(v, n)) a = c + 1;
    else b = c;
  }

  std::vector<std::vector<int>> v(n + 1);
  for (auto [num, e1, e2] : edges) {
    if (num < a) break;
    v[e1].push_back(e2);
  }

  return v;
}