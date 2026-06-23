#pragma once

enum Alg {
  bb,
  lower_bound,
  median,
  threshold,
  dag_greedy,
};

constexpr Alg algs[] = {bb, lower_bound, median, threshold, dag_greedy};
