#pragma once
#include <random>
#include <vector>

extern std::mt19937 rng;

int rand_int(int a, int b);

bool roll(double p);

void remove_edge(std::vector<std::vector<int>> &v, int x, int c);

bool cycle(std::vector<std::vector<int>> &v, int n);

bool node_cycle(std::vector<std::vector<int>> &v, int n, int x);

bool connected(std::vector<std::vector<int>> &v, int x, int c);
