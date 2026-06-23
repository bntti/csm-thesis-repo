#pragma once
#include <chrono>

const int SEED = std::chrono::steady_clock::now().time_since_epoch().count();
const int N = 100;