/*
  Link:         http://adventofcode.com/2024/day/2
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2024/AdventOfCode_02_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/02/2024
*/

#ifndef _2024_ADVENTOFCODE_02_H_
#define _2024_ADVENTOFCODE_02_H_

#include <algorithm>
#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include "../util/util.h"

namespace aoc2024_02 {
  using namespace std;

  std::vector<int> GetLevelsFromInput(const std::string& input) {
    std::vector<int> levels;
    for (int i = 0; i < input.size(); ++i) {
      levels.push_back(util::getNumberRef(input, i));
    }
    return levels;
  }

  bool IsValid(const std::vector<int> &levels, int skip_level = 0, int tolerance = 0) {
    int delta = 0;
    for (int i = 1; i < levels.size(); ++i) {
      int d = levels[i] - levels[i-1];
      if (d == 0) {
        cout << "delta of 0. ";
        if (tolerance-- == 0) {
          return false;  // Mixing ascending and descending
        }
      }
      if (delta > 0 && d < 0 || delta < 0 && d > 0) {
        cout << "mixing ascending and descending. ";
        if (tolerance-- == 0) {
          return false;  // Mixing ascending and descending
        }
      }
      if (abs(d) > 3) {
        cout << "Step " << abs(d) << " is > 3. ";
        if (tolerance-- == 0) {
          return false;  // step is > 3.
        }
      }
      delta = d;
    }
    return true;
  }

  int IsValid(const std::string& input, int part = 1, int tolerance = 0) {
    cout << input << " is: ";
    std::vector<int> levels = GetLevelsFromInput(input);
    bool valid = IsValid(levels, tolerance);
    for (int i = 0; !valid && part == 2 && i < levels.size(); ++i) {
      std::vector<int> levels_skip;
      levels_skip.reserve(levels.size());
      for (int j = 0; j < i; ++ j) {
        levels_skip.push_back(levels.at(j));
      }
      for (int j = i+1; j < levels.size(); ++ j) {
        levels_skip.push_back(levels.at(j));
      }
      valid = IsValid(levels_skip, tolerance);
      if (valid) {
        break;
      }
    }
    cout << (valid ? "Valid" : "Invalid") << endl;
    return valid ? 1 : 0;
  }

  void solve(int part = 1) {
    std::string input;
    int result = 0;
    while (std::getline(cin, input)) {
      result += (part == 1 ? IsValid(input) : IsValid(input, part));
    }
    cout << "Solution: " << result << endl;
  }
};
#endif /* _2024_ADVENTOFCODE_01_H_ */
