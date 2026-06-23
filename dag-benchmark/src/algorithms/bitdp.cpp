#include "./bitdp.hpp"
#include "../util.hpp"
#include <algorithm>
#include <cstdint>
#include <vector>

std::vector<std::vector<int>> bitdp_dag(std::vector<std::vector<std::vector<int>>> &vs, int n,
                                        double p, double q) {

  if (n > 28) return std::vector<std::vector<int>>(n + 1);

  Edges regrets = calculate_regrets(vs, n, p, q);
  std::vector<std::vector<std::pair<int, double>>> r(n + 1);
  for (auto t : regrets) {
    auto [x, c, cst] = t.second;
    r[c].push_back({x, cst});
  }

  std::vector<double> dp(1 << n, 0);
  std::vector<uint32_t> previous(1 << n, 0);
  for (uint32_t b = 0; b < (uint32_t)(1 << n); ++b) {
    for (int x = 0; x < n; ++x) {
      if ((b & (1 << x)) == 0) continue;

      double alt = dp[b ^ (1 << x)];
      for (auto t : r[x + 1]) {
        auto [rx, cst] = t;
        if (b & (1 << (rx - 1))) alt += cst;
      }
      dp[b] = std::max(dp[b], alt);
      if (dp[b] == alt) previous[b] = (b ^ (1 << x));
    }
  }

  std::vector<std::vector<int>> v(n + 1);
  uint32_t b = (1 << n) - 1;
  while (b > 0) {
    uint32_t pb = previous[b];
    int x = __builtin_ctz(pb ^ b);

    for (auto t : r[x + 1]) {
      if (b & (1 << (t.first - 1))) v[t.first].push_back(x + 1);
    }
    b = pb;
  }
  return v;
}