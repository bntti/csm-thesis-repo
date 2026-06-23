#pragma once
#include <vector>

using Dag = std::vector<std::vector<int>>;

Dag median_solve(std::vector<Dag> &vs, std::vector<Dag> &cpdags, int n);
