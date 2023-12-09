/*
  Link:         http://adventofcode.com/2023/day/09
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/09/2023
*/

#ifndef _2023_ADVENTOFCODE_09_H_
#define _2023_ADVENTOFCODE_09_H_

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

namespace aoc2023_09 {
using namespace std;

vector<vector<int64_t>> getInput() {
  string input;
  vector<vector<int64_t>> histories;
  while(!cin.eof()) {
    getline(cin, input);
    histories.emplace_back();
    for (int i = 0; i < input.size(); ++i) {
      histories.back().push_back(util::getNumberRef(input, i));
    }
  }
  return histories;
}

int64_t interpolate(vector<int64_t>& history, int depth = 1) {
  int64_t solution = 0l;
  vector<int64_t> sub_history;
  bool all_zeroes = true;
  for (int i = 0; i < depth; ++i) {
    cout << "\t"; 
  }
  for (int i = 1; i < history.size(); ++i) {
    int64_t delta = history[i] - history[i - 1];
    if (delta != 0) { all_zeroes = false; }
    sub_history.push_back(delta);
  }
  util::printVector(sub_history, false); cout << endl << "\t"; 
  if (all_zeroes) {
    for (int i = 0; i < depth; ++i) {
      cout << "\t"; 
    }
    cout << "\treturning 0" << endl;
    history.emplace_back(0);
    return 0;
  } 
  for (int i = 0; i < depth; ++i) {
    cout << "\t"; 
  }
  int64_t last_item =  sub_history.back();
  auto result =  interpolate(sub_history, depth + 1);
  solution = last_item + result;
  history.push_back(solution);
  for (int i = 0; i < depth; ++i) {
    cout << "\t"; 
  }
  cout << "\tlast step returned: " << solution << endl; 
  return solution;
}

void solve1() {
  vector<vector<int64_t>> histories = getInput();
  int64_t solution = 0;
  for (auto& h : histories) {
    cout << "History: "; util::printVector(h, false);
    cout << endl << "\t";
    auto step_solution =  h.back() + interpolate(h);
    solution += step_solution;
    cout << "Solution:  " << step_solution << endl;
  }
  cout << "Complete solution: " <<solution << endl;
}

void solve2() {
  string input;
  cin >> input;
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } else {
    solve2();
  }
}

};  // aoc2023_09

#endif /* _2023_ADVENTOFCODE_09_H_ */
