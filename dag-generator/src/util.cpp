#include "util.hpp"
#include "config.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

std::mt19937 rng(SEED);
std::uniform_real_distribution<double> uniform(0, 1);

int rand_int(int a, int b) { return a + rng() % (b - a + 1); }

bool roll(double p) { return uniform(rng) < p; }

void remove_edge(std::vector<std::vector<int>> &v, int x, int c) {
  v[x].erase(std::remove(v[x].begin(), v[x].end(), c), v[x].end());
}

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
