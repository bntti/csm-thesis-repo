#pragma once
#include <vector>

using Dag = std::vector<std::vector<int>>;

Dag bb_solve(std::vector<Dag> &dags, std::vector<Dag> &cpdags, int n);