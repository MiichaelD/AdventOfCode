/*
  Link:         http://adventofcode.com/2023/day/14
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/14/2023
*/

#ifndef _2023_ADVENTOFCODE_14_H_
#define _2023_ADVENTOFCODE_14_H_

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

namespace aoc2023_14 {
using namespace std;

constexpr char kMovableRock = 'O';
constexpr char kEmptySpace = '.';
constexpr char kWall = '#';
constexpr int64_t kTargetCycles = 1000000000;

class PuzzleInput {
  public:
  vector<string> map_str;
  vector<vector<char>> map;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    while(!cin.eof()) {
      getline(cin, input.map_str.emplace_back());
      input.map.emplace_back();
      for (char c: input.map_str.back()) {
        input.map.back().emplace_back(c);
      }
    }
    return input;
  }

  bool operator==(const PuzzleInput& other) const {
    for (int r = 0; r < map.size(); ++r) {
      for (int c = 0; c < map.back().size(); ++c) {
        if (map[r][c] != other.map[r][c]) {
          return false;
        }
      }
    }
  return true;
}

  void print() const {
    for (const auto& line : map) {
      util::printVector(line);
      cout << endl;
    }
    cout << endl;
  }

  void ShakeNorth() {
    // cout << endl << "Shaking North" << endl << endl;
    for (int c = 0; c < map.back().size(); ++c) {
      int empty_spaces = 0;
      int first_empty_row = -1;
      for (int r = 0; r < map.size(); ++r) {
        switch(map[r][c]) {
          case kMovableRock: {
            if (!empty_spaces) {
              break;
            }
            map[first_empty_row++][c] = kMovableRock;
            map[r][c] = kEmptySpace;
          }
          break;
          case kEmptySpace: 
            if (empty_spaces++ == 0) {
              first_empty_row = r;
            }
            break;
          case kWall: 
            first_empty_row = -1;
            empty_spaces = 0;
            break;
        }
      }
    }
  }

  int64_t GetValue() const {
    int64_t result = 0;
    for (int r = 0; r < map.size(); ++r) {
      int64_t row_result = 0;
      for (int c = 0; c < map.back().size(); ++c) {
        if (map[r][c] == 'O') {
          row_result += map.size() - r;
        }
      }
      // cout << "\tresult in row " << r << ", is: " << row_result << endl;
      result += row_result;
    }
    return result;
  }

  void ShakeForCycle(int cycles = 1) {
    for (int c = 0; c < cycles; ++c) {
      for (int i = 0; i < 4; ++i) {
        ShakeNorth();
        RotateClockwise();
      }
    }
  }

  void RotateClockwise() {
    int rowLimit = map.size() / 2;
    int colLimit = map.back().size();
    int dim = map.size() - 1;
    for (int r = 0 ; r < rowLimit; ++r) {
      --colLimit;
      for (int c = r; c < colLimit; ++c){
        char aux = map[r][c]; // copy top-left to aux
        map[r][c] = map[dim-c][r]; // copy bottom-left to top-left
        map[dim-c][r] = map[dim-r][dim-c]; // copy bottom-right to bottom-left 
        map[dim-r][dim-c] = map[c][dim-r]; // copy top-right to bottom right
        map[c][dim-r] = aux; // copy aux to top-right
      }
      // cout << "At Step: " << (r+1) << endl;
      // printMatrix(matrix);
    }
  }
};

int64_t FindValueAtCycle(PuzzleInput& input, int64_t cycles = kTargetCycles) {
  PuzzleInput input_2 = input;
  int64_t c1 = 0;
  input_2.ShakeForCycle();
  int64_t c2 = 1;
  while (c1 < cycles) {
    if (input == input_2) {
      cout << "Found matching inputs at cycle: " << c1 << endl;
      // Calculating how many cycles are missing
      int64_t pending = cycles - c1;
      int64_t distance = c2 - c1;
      int64_t addition = pending / distance * distance;
      cout << "Pending: " << pending << ". Distance: " << distance << endl; 
      cout << "Adding " << c1 << " += " << addition << endl; 
      c1 += addition;
      cout << "This match repeats at cycle: " << c1 << endl;
      break;
    }
    input.ShakeForCycle();
    ++c1;
    input_2.ShakeForCycle(2);
    c2 += 2;
  }
  // Processing pending cycles
  while (c1 < cycles) {
    input.ShakeForCycle();
    ++c1;
    cout << "Extra cycle: " << c1 << endl;
  }
  input.print();
  return input.GetValue();
}

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  input.print();
  int64_t result = 0;
  if (part == 1) {
    input.ShakeNorth();
    result = input.GetValue();
    input.print();
  } else {
    // input.ShakeForCycle(1000000000);
    result = FindValueAtCycle(input);
  }
  cout << "Result: " << result << endl;
}

};  // aoc2023_14

#endif /* _2023_ADVENTOFCODE_14_H_ */
