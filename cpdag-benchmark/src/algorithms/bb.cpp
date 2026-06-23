#include "./bb.hpp"
#include "../convert.hpp"
#include "../util.hpp"
#include "./greedy.hpp"
#include "./median.hpp"
#include "./threshold.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

using Dag = std::vector<std::vector<int>>;

struct edit_dist_vars {
  std::vector<std::vector<int>> directed;
  std::vector<std::vector<int>> undirected;
};

static int edit_distance(std::vector<Dag> &cpdags, int n, Dag &dag_v, edit_dist_vars &vars) {
  Dag cpdag = convert(dag_v, n);
  int dag_count = cpdags.size();
  int r_total = 0;
  int p_total = 0;
  int p2_total = 0;
  int q_total = 0;
  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      int same = vars.directed[x][c];
      int rev = vars.directed[c][x];
      int undir = vars.undirected[c][x];
      int none = dag_count - same - rev - undir;
      if (connected(cpdag, x, c) && connected(cpdag, c, x)) {
        p_total += none;
        p2_total += same + rev;
      } else if (connected(cpdag, x, c)) {
        r_total += rev;
        p_total += none;
        p2_total += undir;
      } else if (connected(cpdag, c, x)) {
        r_total += same;
        p_total += none;
        p2_total += undir;
      } else {
        p_total += (same + rev + undir);
        q_total += none;
      }
    }
  }

  return r_total + p_total * 1 + p2_total * 1 + q_total * 0;
}

static inline int get_min_cost(std::vector<Dag> &cpdags, int x, int c, edit_dist_vars &vars) {
  int same = vars.directed[x][c];
  int rev = vars.directed[c][x];
  int undir = vars.undirected[c][x];
  int none = cpdags.size() - same - rev - undir;
  return std::min(std::min(rev + undir + none, same + undir + none),
                  std::min(same + rev + none, same + rev + undir));
}

static inline int get_min_cost_edge(std::vector<Dag> &cpdags, int x, int c, edit_dist_vars &vars) {
  int same = vars.directed[x][c];
  int rev = vars.directed[c][x];
  int undir = vars.undirected[c][x];
  int none = cpdags.size() - same - rev - undir;
  return std::min(rev + undir + none, same + rev + none);
}

static inline int get_cost_no_edge(std::vector<Dag> &cpdags, int x, int c, edit_dist_vars &vars) {
  int same = vars.directed[x][c];
  int rev = vars.directed[c][x];
  int undir = vars.undirected[c][x];
  return same + rev + undir;
}

static long long num = 0;
static std::vector<std::vector<int>> best_v;
static int best_cost = 1e9;
static void solve_rec(std::vector<Dag> &cpdags, Dag &v, std::vector<int> &p, int n, int x, int c,
                      edit_dist_vars &vars, int lb) {
  if (lb >= best_cost) return;
  if (c > n) {
    ++x;
    c = x + 1;
  }

  if (x >= n) {
    ++num;
    int cost = edit_distance(cpdags, n, v, vars);
    assert(lb <= cost);
    if (cost < best_cost) {
      best_v = v;
      best_cost = cost;
    }
    return;
  }

  // Try to add x -> c
  v[x].push_back(c);
  p[c]++;
  int diff = get_min_cost_edge(cpdags, x, c, vars) - get_min_cost(cpdags, x, c, vars);
  if (p[x] == 0 || v[c].size() == 0 || !node_cycle(v, n, x))
    solve_rec(cpdags, v, p, n, x, c + 1, vars, lb + diff);
  p[c]--;
  v[x].pop_back();

  // Try to add c -> x
  v[c].push_back(x);
  p[x]++;
  diff = get_min_cost_edge(cpdags, c, x, vars) - get_min_cost(cpdags, c, x, vars);
  if (p[c] == 0 || v[x].size() == 0 || !node_cycle(v, n, c))
    solve_rec(cpdags, v, p, n, x, c + 1, vars, lb + diff);
  p[x]--;
  v[c].pop_back();

  // No edge added between x and c
  diff = get_cost_no_edge(cpdags, x, c, vars) - get_min_cost(cpdags, x, c, vars);
  solve_rec(cpdags, v, p, n, x, c + 1, vars, lb + diff);
}

Dag bb_solve(std::vector<Dag> &dags, std::vector<Dag> &cpdags, int n) {
  // Precalculate arrays for edit distance calculation
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
  edit_dist_vars vars = {directed, undirected};

  // Return empty if computing would take too long
  if (n > 20) return Dag(n + 1);

  int lb = 0;
  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      lb += get_min_cost(cpdags, x, c, vars);
    }
  }

  // Set best cost to some known good solution
  Dag greedy = greedy_dag(dags, n);
  int greedy_cost = edit_distance(cpdags, n, greedy, vars);
  Dag median = median_solve(dags, cpdags, n);
  int median_cost = edit_distance(cpdags, n, median, vars);
  Dag threshold = threshold_dag(dags, n);
  int threshold_cost = edit_distance(cpdags, n, threshold, vars);

  if (greedy_cost < median_cost && greedy_cost < threshold_cost) {
    best_cost = greedy_cost;
    best_v = greedy;
  } else if (median_cost < threshold_cost) {
    best_cost = median_cost;
    best_v = median;
  } else {
    best_cost = threshold_cost;
    best_v = threshold;
  }

  std::vector<std::vector<int>> v(n + 1);
  std::vector<int> pa(n + 1, 0);
  solve_rec(cpdags, v, pa, n, 1, 2, vars, lb);
  return best_v;
}