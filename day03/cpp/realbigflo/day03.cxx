#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

constexpr int STAR1_TRAVERSE_RIGHT = 3;
constexpr int STAR1_TRAVERSE_DOWN = 1;

constexpr int STAR2_TRAVERSE_RIGHT = 1;
constexpr int STAR2_TRAVERSE_DOWN = 1;

constexpr int STAR3_TRAVERSE_RIGHT = 5;
constexpr int STAR3_TRAVERSE_DOWN = 1;

constexpr int STAR4_TRAVERSE_RIGHT = 7;
constexpr int STAR4_TRAVERSE_DOWN = 1;

constexpr int STAR5_TRAVERSE_RIGHT = 1;
constexpr int STAR5_TRAVERSE_DOWN = 2;

std::unique_ptr<std::vector<std::unique_ptr<std::vector<bool>>>>
get_tree_map() {
  int row = 0;
  std::fstream in_file;
  std::string line;
  auto treemap =
      std::make_unique<std::vector<std::unique_ptr<std::vector<bool>>>>();
  in_file.open("input.txt", ::std::ios_base::in);
  while (getline(in_file, line)) {
    auto tree_row = std::make_unique<std::vector<bool>>();
    for (const auto &letter : line) {
      tree_row->push_back(letter == '#');
    }
    treemap->push_back(std::move(tree_row));
    ++row;
  }

  return treemap;
}

std::unique_ptr<std::vector<std::unique_ptr<std::vector<bool>>>>
traverse_tree_map_star1(
    std::unique_ptr<std::vector<std::unique_ptr<std::vector<bool>>>> treemap,
    int traverse_right, int traverse_down, int &hits) {
  for (int next_y = 0, next_x = 0; next_y < treemap->size();
       next_y += traverse_down,
           next_x = (next_x + traverse_right) % (treemap->at(0)->size())) {
    if (treemap->at(next_y)->at(next_x)) {
      ++hits;
    }
  }
  return treemap;
}

int main() {
  auto tree_map = get_tree_map();
  std::array<int, 5> tree_hits = {0};

  tree_map = traverse_tree_map_star1(std::move(tree_map), STAR1_TRAVERSE_RIGHT,
                                     STAR1_TRAVERSE_DOWN, tree_hits[0]);
  tree_map = traverse_tree_map_star1(std::move(tree_map), STAR2_TRAVERSE_RIGHT,
                                     STAR2_TRAVERSE_DOWN, tree_hits[1]);
  tree_map = traverse_tree_map_star1(std::move(tree_map), STAR3_TRAVERSE_RIGHT,
                                     STAR3_TRAVERSE_DOWN, tree_hits[2]);
  tree_map = traverse_tree_map_star1(std::move(tree_map), STAR4_TRAVERSE_RIGHT,
                                     STAR4_TRAVERSE_DOWN, tree_hits[3]);
  tree_map = traverse_tree_map_star1(std::move(tree_map), STAR5_TRAVERSE_RIGHT,
                                     STAR5_TRAVERSE_DOWN, tree_hits[4]);
  std::cout << "Tree hits star 1: " << tree_hits[0] << "\n";
  std::cout << "Tree hits star 2: "
            << std::accumulate(tree_hits.cbegin(), tree_hits.cend(), 1,
                               std::multiplies<int>())
            << "\n";
  return 0;
}