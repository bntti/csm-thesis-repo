#pragma once

enum Alg {
  bitdp,
  greedy,
  lb,
  median,
};

constexpr Alg algs[] = {lb, bitdp, greedy, median};
constexpr double P = 0.5;
constexpr double Q = 0.85 * P;
