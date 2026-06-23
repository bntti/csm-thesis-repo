#include "./util.hpp"
#include <algorithm>
#include <vector>

bool cycle_check(std::vector<std::vector<int>> &v, std::vector<int> &visited, int x) {
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
  std::vector<int> visited(n + 1, 0);
  for (int x = 1; x <= n; ++x) {
    if (cycle_check(v, visited, x)) return true;
  }
  return false;
}

bool node_cycle(std::vector<std::vector<int>> &v, int n, int x) {
  std::vector<int> visited(n + 1, 0);
  return cycle_check(v, visited, x);
}

bool connected(std::vector<std::vector<int>> &v, int x, int c) {
  for (int cc : v[x]) {
    if (cc == c) return true;
  }
  return false;
}

Edges calculate_regrets(std::vector<std::vector<std::vector<int>>> &vs, int n) {
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
      int same = e[x][c];
      int rev = e[c][x];
      int none = dag_count - same - rev;
      int b = (same + rev) * 1 + none * 0;
      int w = rev + none * 1;
      int regret = b - w;
      if (regret > 0) regrets.push_back({regret, {x, c, regret}});
    }
  }

  std::sort(regrets.begin(), regrets.end());
  return regrets;
}

int edit_distance(std::vector<std::vector<std::vector<int>>> &cpdags, int n,
                  std::vector<std::vector<int>> v) {
  std::vector<std::vector<int>> directed(n + 1);
  std::vector<std::vector<int>> undirected(n + 1);
  for (int i = 1; i <= n; ++i) {
    directed[i].resize(n + 1);
    undirected[i].resize(n + 1);
    std::fill(directed[i].begin(), directed[i].end(), 0);
    std::fill(undirected[i].begin(), undirected[i].end(), 0);
  }

  for (auto cpdag : cpdags) {
    for (int x = 1; x <= n; ++x) {
      for (int c : cpdag[x]) {
        if (connected(cpdag, c, x)) ++undirected[x][c];
        else ++directed[x][c];
      }
    }
  }

  int dag_count = cpdags.size();
  int r_total = 0;
  int p_total = 0;
  int p2_total = 0;
  int q_total = 0;
  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      int same = directed[x][c];
      int rev = directed[c][x];
      int undir = undirected[c][x];
      int none = dag_count - same - rev - undir;
      if (connected(v, x, c) && connected(v, c, x)) {
        p_total += none;
        p2_total += same + rev;
      } else if (connected(v, x, c)) {
        r_total += rev;
        p_total += none;
        p2_total += undir;
      } else if (connected(v, c, x)) {
        r_total += same;
        p_total += none;
        p2_total += undir;
      } else {
        p_total += (same + rev + undir);
        q_total += none;
      }
    }
  }

  return r_total + p_total + p2_total + (q_total * 0);
}
