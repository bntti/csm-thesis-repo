#pragma once
#include <algorithm>
#include <vector>

template <class T> int inline vcount(std::vector<T> &v, T x) {
  return std::count(v.begin(), v.end(), x);
}

template <class T> void inline verase(std::vector<T> &v, T x) {
  v.erase(std::remove(v.begin(), v.end(), x), v.end());
}

bool cycle(std::vector<std::vector<int>> &v, int n);

bool node_cycle(std::vector<std::vector<int>> &v, int n, int x);

bool connected(std::vector<std::vector<int>> &v, int x, int c);

/// Calculates number of common elements, O(|a|+|b|), assumes sorted
template <class T> int calc_common(std::vector<T> a, std::vector<T> b) {
  int ai = 0, bi = 0;
  int res = 0;
  while (true) {
    if (ai >= (int)a.size() || bi >= (int)b.size()) break;
    while (ai < (int)a.size() && a[ai] < b[bi]) ++ai;
    while (bi < (int)b.size() && b[bi] < a[ai]) ++bi;
    if (ai >= (int)a.size() || bi >= (int)b.size()) break;
    if (a[ai] == b[bi]) {
      ++res;
      ++ai;
      ++bi;
    }
  }
  return res;
}

using Edges = std::vector<std::pair<int, std::tuple<int, int, int>>>;
Edges calculate_regrets(std::vector<std::vector<std::vector<int>>> &vs, int n);

int edit_distance(std::vector<std::vector<std::vector<int>>> &cpdags, int n,
                  std::vector<std::vector<int>> v);
