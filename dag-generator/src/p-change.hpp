#pragma once
#include <vector>

using Dag = std::vector<std::vector<int>>;

Dag p_create(int n, double p_edge);
Dag p_change_alg(Dag &original, int n, double p_edge, double p_remove, int n_swaps);
