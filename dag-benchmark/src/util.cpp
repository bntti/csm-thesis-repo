#include "./util.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

bool cycle_check(std::vector<std::vector<int>> &v, std::vector<uint8_t> &visited, int x) {
  if (visited[x] == 1) return true;
  if (visited[x] == 2) return false;
  visited[x] = 1;
  for (int c : v[x]) {
    if (cycle_check(v, visited, c)) return true;
  }
  visited[x] = 2;
  return false;
}

bool cycle(std::vector<std::vector<int>> &v, int n) {
  std::vector<uint8_t> visited(n + 1, 0);
  for (int x = 1; x <= n; ++x) {
    if (cycle_check(v, visited, x)) return true;
  }
  return false;
}

bool node_cycle(std::vector<std::vector<int>> &v, int n, int x) {
  std::vector<uint8_t> visited(n + 1, 0);
  return cycle_check(v, visited, x);
}

bool connected(std::vector<std::vector<int>> &v, int x, int c) {
  for (int cc : v[x]) {
    if (cc == c) return true;
  }
  return false;
}

Edges calculate_regrets(std::vector<std::vector<std::vector<int>>> &vs, int n, double p, double q) {
  std::vector<std::vector<int>> edge_union(n + 1);
  for (auto v : vs) {
    for (int x = 1; x <= n; ++x) {
      for (int c : v[x]) {
        if (!connected(edge_union, x, c)) edge_union[x].push_back(c);
      }
    }
  }

  std::vector<std::vector<int>> e(n + 1, std::vector<int>(n + 1, 0));
  for (auto v : vs) {
    for (int x = 1; x <= n; ++x) {
      for (int c : v[x]) e[x][c]++;
    }
  }

  int dag_count = vs.size();
  Edges regrets;
  for (int x = 1; x <= n; ++x) {
    for (int c : edge_union[x]) {
      double same = e[x][c];
      double rev = e[c][x];
      double none = dag_count - same - rev;
      double b = (same + rev) * p + none * q;
      double w = rev + none * p;
      double regret = b - w;
      if (regret > 0) regrets.push_back({regret, {x, c, regret}});
    }
  }

  std::sort(regrets.begin(), regrets.end());
  return regrets;
}

double edit_distance(std::vector<std::vector<std::vector<int>>> &vs, int n, double p, double q,
                     std::vector<std::vector<int>> d) {
  std::vector<std::vector<int>> e(n + 1, std::vector<int>(n + 1, 0));
  for (auto v : vs) {
    for (int x = 1; x <= n; ++x) {
      for (int c : v[x]) e[x][c]++;
    }
  }

  int dag_count = vs.size();
  int r_total = 0;
  int p_total = 0;
  int q_total = 0;
  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      int same = e[x][c];
      int rev = e[c][x];
      int none = dag_count - same - rev;
      if (connected(d, x, c)) {
        r_total += rev;
        p_total += none;
      } else if (connected(d, c, x)) {
        r_total += same;
        p_total += none;
      } else {
        p_total += same + rev;
        q_total += none;
      }
    }
  }

  return r_total + p_total * p + q_total * q;
}
