#include "./convert.hpp"
#include "./util.hpp"
#include <cassert>
#include <vector>

enum Direction { directed, undirected };
using Dag = std::vector<std::vector<int>>;
using DirDag = std::vector<std::vector<std::pair<int, Direction>>>;

bool connected(DirDag &v, int a, int b) {
  return vcount(v[a], {b, directed}) || vcount(v[a], {b, undirected}) ||
         vcount(v[b], {a, directed});
}

void direct_edges(DirDag &v, DirDag &r, std::vector<std::pair<int, int>> &direct, bool &change) {
  if (!direct.empty()) change = 1;
  for (auto [a, b] : direct) {
    int va = v[a].size();
    int ra = r[a].size();
    int vb = v[b].size();
    int rb = r[b].size();
    verase(v[a], {b, undirected});
    verase(r[a], {b, undirected});
    verase(v[b], {a, undirected});
    verase(r[b], {a, undirected});
    assert((int)v[a].size() + 1 == va);
    assert((int)r[a].size() + 1 == ra);
    assert((int)v[b].size() + 1 == vb);
    assert((int)r[b].size() + 1 == rb);

    v[a].push_back({b, directed});
    r[b].push_back({a, directed});
  }
  direct.clear();
}

/// Find all edges that are in unshielded colliders
std::vector<std::pair<int, int>> gen_pattern(Dag &v, Dag &r, int n) {
  std::vector<std::pair<int, int>> direct;

  for (int x = 1; x <= n; ++x) sort(r[x].begin(), r[x].end());
  for (int x = 1; x <= n; ++x) sort(v[x].begin(), v[x].end());

  // x = the node that is pointed to
  for (int c = 1; c <= n; ++c) {
    if (r[c].size() < 2) continue;

    for (auto x : r[c]) {
      int common = calc_common(v[x], r[c]) + calc_common(r[x], r[c]);
      if (common < (int)r[c].size() - 1) direct.push_back({x, c});
    }
  }

  return direct;
}

void orient(DirDag &v, DirDag &r, int n) {
  bool change = true;
  while (change) {
    change = false;
    std::vector<std::pair<int, int>> direct;

    // R1
    /* a
     * |
     * v
     * x--c
     */
    for (int x = 1; x <= n; ++x) {
      if (v[x].size() == 0 || r[x].size() == 0) continue;

      for (auto [b, bdir] : v[x]) {
        if (bdir != undirected) continue;

        for (auto [a, adir] : r[x]) {
          if (adir != directed) continue;
          assert(a != b);

          if (!connected(v, a, b)) {
            direct.push_back({x, b});
            break;
          }
        }
      }

      direct_edges(v, r, direct, change);
    }

    // R2
    /*  x
     *  |\
     *  v \
     *  a->b
     */
    for (int x = 1; x <= n; ++x) {
      if (v[x].size() < 2) continue;

      for (auto [b, bdir] : v[x]) {
        if (bdir != undirected) continue;

        for (auto [a, adir] : v[x]) {
          if (adir != directed) continue;
          assert(a != b);

          if (vcount(v[a], {b, directed})) {
            direct.push_back({x, b});
            break;
          }
        }
      }

      direct_edges(v, r, direct, change);
    }

    // R3
    /*    b
     *  / | \
     * a  |  c
     *  \ | /
     *   v|v
     *    x
     */
    for (int x = 1; x <= n; ++x) {
      if (r[x].size() < 3) continue;

      for (auto [b, bdir] : r[x]) {
        if (bdir != undirected) continue;
        bool found = 0;

        for (auto [a, adir] : r[x]) {
          if (adir != directed) continue;
          assert(a != b);

          for (auto [c, cdir] : r[x]) {
            if (cdir != directed) continue;
            if (a == c) continue;
            assert(c != b);

            if (connected(v, a, c)) continue;
            if (vcount(v[b], {a, undirected}) && vcount(v[b], {c, undirected})) {
              direct.push_back({b, x});
              found = 1;
              break;
            }
          }
          if (found) break;
        }
      }

      direct_edges(v, r, direct, change);
    }
  }
}

std::vector<std::vector<int>> convert(std::vector<std::vector<int>> &in_v, int n) {
  // Create copy and reverse copy of in_v
  Dag dv(n + 1);
  Dag dr(n + 1);
  for (int x = 1; x <= n; ++x) {
    for (int c : in_v[x]) {
      dv[x].push_back(c);
      dr[c].push_back(x);
    }
  }

  // Calculate all edges that should be oriented
  auto direct = gen_pattern(dv, dr, n);

  // Create CPDAG and reverse version of it
  DirDag v(n + 1);
  DirDag r(n + 1);
  for (int x = 1; x <= n; ++x) {
    for (int c : in_v[x]) {
      v[x].push_back({c, undirected});
      r[x].push_back({c, undirected});
      v[c].push_back({x, undirected});
      r[c].push_back({x, undirected});
    }
  }

  // Direct the edges in all unshielded colliders
  bool _;
  direct_edges(v, r, direct, _);

  // Use orientation rules
  orient(v, r, n);

  // Convert into normal graph format
  std::vector<std::vector<int>> out_v(n + 1);
  for (int x = 1; x <= n; ++x) {
    for (auto p : v[x]) {
      out_v[x].push_back(p.first);
    }
  }
  return out_v;
}
