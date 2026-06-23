#include "./config.hpp"
#include "./p-change.hpp"
#include <cassert>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Dag = std::vector<std::vector<int>>;

void create_instance(int num_dags, std::string &output_folder, int index) {
  std::cout << "Generating " << num_dags << " dags" << std::endl;

  double p_edge = 1.5 / (N - 1);
  Dag original = p_create(N, p_edge);
  std::vector<Dag> dags;
  for (int i = 0; i < num_dags; ++i) dags.push_back(p_change_alg(original, N, p_edge, 0.7, 0));

  std::ofstream myfile;
  std::string output_file{
      std::format("{}/generated-{}-{:02}-{}.dags", output_folder, N, num_dags, index)};
  myfile.open(output_file);
  myfile << dags.size() << '\n';

  for (Dag d : dags) {
    std::vector<std::pair<int, int>> edges{};
    for (int x = 1; x <= N; ++x) {
      for (int c : d[x]) edges.push_back({x, c});
    }

    myfile << N << " " << edges.size() << '\n';
    for (auto [x, c] : edges) myfile << x << " " << c << '\n';
  }

  myfile.close();
}

int main() {
  std::cout << "Generating dags..." << std::endl;

  std::string output_folder{std::format("output/generated-{0}", N)};
  std::filesystem::create_directories(output_folder);

  for (int num_dags = 2; num_dags <= 25; ++num_dags) {
    for (int i = 0; i < 4; ++i) create_instance(num_dags, output_folder, i);
  }
}
