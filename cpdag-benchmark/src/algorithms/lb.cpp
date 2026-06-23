#include "./lb.hpp"
#include "../util.hpp"
#include <algorithm>
#include <assert.h>
#include <vector>

using Dag = std::vector<std::vector<int>>;

static std::vector<std::pair<int, int>> get_improves(std::vector<Dag> &cpdags, int n) {
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

  std::vector<std::pair<int, int>> improve;

  int dag_count = cpdags.size();
  for (int x = 1; x <= n; ++x) {
    for (int c = x + 1; c <= n; ++c) {
      int same = directed[x][c];
      int rev = directed[c][x];
      int undir = undirected[c][x];
      int none = dag_count - same - rev - undir;

      int no_edge = same + rev + undir;
      int same_edge = rev + undir + none;
      int rev_edge = same + undir + none;
      int undir_edge = same + rev + none;
      if (same_edge < no_edge && same_edge <= rev_edge && same_edge <= undir_edge) {
        improve.push_back({x, c});
      } else if (rev_edge < no_edge && rev_edge <= undir_edge) {
        improve.push_back({c, x});
      } else if (undir_edge < no_edge) {
        improve.push_back({x, c});
        improve.push_back({c, x});
      }
    }
  }

  return improve;
}

Dag lb_dag(std::vector<Dag> &cpdags, int n) {
  std::vector<std::pair<int, int>> improves = get_improves(cpdags, n);

  Dag v(n + 1);
  for (auto [x, c] : improves) v[x].push_back(c);

  return v;
}