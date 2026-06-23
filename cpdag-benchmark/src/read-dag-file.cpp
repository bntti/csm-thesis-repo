#include "./read-dag-file.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Dag = std::vector<std::vector<int>>;

std::vector<Dag> dag_file(int argc, char **argv) {
  assert(argc >= 2);
  std::string filename = argv[1];
  std::ifstream dag_file(filename);

  if (!dag_file.is_open()) {
    std::cout << "No such file " << filename << std::endl;
    exit(1);
  }

  int num_dags;
  dag_file >> num_dags;

  std::vector<Dag> dags;
  for (int d = 0; d < num_dags; ++d) {
    int n, m;
    dag_file >> n >> m;

    Dag dag(n + 1);
    for (int i = 0; i < m; ++i) {
      int a, b;
      dag_file >> a >> b;
      dag[a].push_back(b);
    }

    dags.push_back(dag);
  }

  return dags;
}