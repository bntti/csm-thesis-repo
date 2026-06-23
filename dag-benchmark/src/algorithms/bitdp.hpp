#pragma once
#include <vector>

std::vector<std::vector<int>> bitdp_dag(std::vector<std::vector<std::vector<int>>> &vs, int n,
                                        double p, double q);