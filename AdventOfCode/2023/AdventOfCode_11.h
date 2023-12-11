/*
  Link:         http://adventofcode.com/2023/day/11
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/11/2023
*/

#ifndef _2023_ADVENTOFCODE_11_H_
#define _2023_ADVENTOFCODE_11_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <cstring>  // strcmp()
#include <ctype.h>  // char related functions
#include <climits>  // INT_MAX and such numeric limits
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/util.h"

namespace aoc2023_11 {
using namespace std;

constexpr char kGalaxy = '#';

struct PuzzleInput {
  vector<string> map;
  vector<int64_t> rows_no_gal;
  vector<int64_t> cols_no_gal;
  unordered_set<pair<int64_t,int64_t>, util::pair_hash> galaxies;
  unordered_set<pair<int64_t,int64_t>, util::pair_hash> galaxies_expanded;
  vector<pair<int64_t,int64_t>> galaxies_vec;
  vector<bool> col_has_gal;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    while(!cin.eof()) {
      input.map.emplace_back();
      getline(cin, input.map.back());
      bool row_has_galaxy = false;
      if (input.col_has_gal.size() == 0){
        for (int c = 0; c < input.map.back().size(); ++c) {
          input.col_has_gal.emplace_back(false);
        }
      }
      for (int c = 0; c < input.map.back().size(); ++c) {
        if (input.map.back()[c] == kGalaxy) {
          row_has_galaxy = true;
          input.galaxies_vec.emplace_back(input.map.size()-1, c);
          input.galaxies.insert(input.galaxies_vec.back());
          input.col_has_gal[c] = true;
        }
      }
      if (!row_has_galaxy) {
        input.rows_no_gal.push_back(input.map.size()-1);
      }
    }
    for (int c = 0; c < input.col_has_gal.size(); ++c) {
      if (input.col_has_gal[c]) {
        continue;
      }
      input.cols_no_gal.push_back(c);
    }
    cout << "Cols w/o Galaxies:";
    util::printVector(input.cols_no_gal);
    cout << endl << "Rows w/o Galaxies:";
    util::printVector(input.rows_no_gal);
    cout << endl << input.galaxies.size() << " Galaxies found at:" << endl;
    for (const auto& entry : input.galaxies) {
      util::printPair(entry, true);
    }
    return input;
  }
};

void Expand(PuzzleInput& input, int increment = 1) {
  cout << "Expanding Rows" << endl;
  for (int r = input.rows_no_gal.size() - 1; r >= 0; --r) {
    for (auto& entry : input.galaxies_vec) {
      if (entry.first > input.rows_no_gal[r]) {
        entry.first += increment;
      }
    }
  }
  cout << "Expanding Cols" << endl;
  for (int c = input.cols_no_gal.size() - 1; c >= 0; --c) {
    for (auto& entry : input.galaxies_vec) {
      if (entry.second > input.cols_no_gal[c]) {
        entry.second += increment;
      }
    }
  }

  cout << "New galaxies expanded: " << endl;
  for (const auto& entry : input.galaxies_vec) {
    input.galaxies_expanded.insert(entry);
    util::printPair(entry, true);
  }
}

int64_t Distance(const PuzzleInput& input) {
  int64_t result = 0;
  for (int i = 0 ; i < input.galaxies_vec.size(); ++i) {
    const auto& a = input.galaxies_vec[i];
    cout << endl << "Distance between "; util::printPair(a, true);
    for (int j = i + 1; j < input.galaxies_vec.size(); ++j) {
      const auto& b = input.galaxies_vec[j];
      int64_t distance = abs(a.first - b.first) + abs(a.second - b.second);;
      cout << "\t"; util::printPair(b); cout << "\tis " << distance << endl;
      result += distance;
    }
  }
  return result;
}

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  Expand(input, part == 1 ? 1 : 100);
  int64_t result = Distance(input);
  cout << "Solution: " << result << endl;
}

};  // aoc2023_11

#endif /* _2023_ADVENTOFCODE_11_H_ */
