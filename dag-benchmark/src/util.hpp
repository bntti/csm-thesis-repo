#pragma once
#include <set>
#include <vector>

bool cycle(std::vector<std::vector<int>> &v, int n);

bool node_cycle(std::vector<std::vector<int>> &v, int n, int x);

bool connected(std::vector<std::vector<int>> &v, int x, int c);

// TODO: Check if faster to not use set and use vec.count
template <class T> void multi_erase(std::vector<T> &v, std::vector<T> erase) {
  std::set<T> s(erase.begin(), erase.end());
  for (int i = 0; i < (int)v.size(); ++i) {
    if (!s.count(v[i])) continue;
    swap(v[i], v.back());
    v.pop_back();
    --i;
  }
}

using Edges = std::vector<std::pair<double, std::tuple<int, int, double>>>;
Edges calculate_regrets(std::vector<std::vector<std::vector<int>>> &vs, int n, double p, double q);

double edit_distance(std::vector<std::vector<std::vector<int>>> &vs, int n, double p, double q,
                     std::vector<std::vector<int>> v);
