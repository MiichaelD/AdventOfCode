/*
  Link:         http://adventofcode.com/2023/day/13
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/13/2023
*/

#ifndef _2023_ADVENTOFCODE_13_H_
#define _2023_ADVENTOFCODE_13_H_

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

namespace aoc2023_13 {
using namespace std;

class Challenge {
  public:
  vector<string> read;

  void print() const {
    util::printVector(read, true);
  }

  static bool IsMirrorBetweenCols(
    const Challenge& challenge, int c_a, int c_b, int& smudge) {
    // cout << "Searching between cols: " << c_a << " and " << c_b << endl;
    if (c_a < 0 || c_b >= challenge.read.back().size()) {
      return true; // we matched all the way an edge.
    }
    for (int r = 0; r < challenge.read.size(); ++r) {
      if (challenge.read[r][c_a] != challenge.read[r][c_b]) {
        // cout << "\tFailed at row: " << r << endl;
        // we found a mismatch
        if (--smudge < 0) {
          // cout << "\t\t No smudge left" << endl;
          return false;
        }
      }
    }
    // cout << "\t\tThis column is promising" << endl;
    return IsMirrorBetweenCols(challenge, c_a - 1, c_b + 1, smudge);
  }

  static bool IsMirrorBetweenRows(
    const Challenge& challenge, int r_a, int r_b, int& smudge) {
    // cout << "Searching between rows: " << r_a << " and " << r_b << endl;
    if (r_a < 0 || r_b >= challenge.read.size()) {
      return true; // we matched all the way an edge.
    }
    for (int c = 0; c < challenge.read.back().size(); ++c) {
      if (challenge.read[r_a][c] != challenge.read[r_b][c]) {
        // cout << "\tFailed at col: " << c << endl;
        // we found a mismatch
        if (--smudge < 0) {
          // cout << "\t\t No smudge left" << endl;
          return false;
        }
      }
    }
    // cout << "\t\tThis row is promising" << endl;
    return IsMirrorBetweenRows(challenge, r_a - 1, r_b + 1, smudge);
  }

  pair<int,int> FindReflectionColRow(int part = 1) const {
    pair<int,int> result;
    for (int c = 0; c < read.front().size() - 1 ; ++c) {
      int smudge = (part == 1) ? 0 : 1;
      if (IsMirrorBetweenCols(*this, c, c + 1, smudge) && !smudge) {
        cout << "Found a mirror on Col: " << c + 1 << endl;
        result.first = c + 1;
        break;  // I think there's only one mirror.
      }
    }
    for (int r = 0; r < read.size() - 1; ++r) {
      int smudge = (part == 1) ? 0 : 1;
      if (IsMirrorBetweenRows(*this, r, r + 1, smudge) && !smudge) {
        cout << "Found a mirror on Row: " << r + 1 << endl;
        result.second = r + 1;
        break;  // I think there's only one mirror.
      }
    }
    return result;
  }
};

class PuzzleInput {
public:
  vector<Challenge> challenges;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    string aux;
    while(!cin.eof()) {
      input.challenges.emplace_back();
      while(!cin.eof()) {
        getline(cin, aux);
        if (aux == "") break;
        input.challenges.back().read.emplace_back(std::move(aux));
      }
    }
    return input;
  }
  void print() const {
    for (const auto& challenge : challenges) {
      challenge.print();
      cout << endl;
    }
  }
 
  int64_t CountWaysToSolve(int part = 1) {
    int64_t solutions = 0;
    int64_t multiplier = 100;
    for (const auto& challenge : challenges) {
      cout << "Looking for Reflection for part " << part << endl;
      challenge.print();
      auto result = challenge.FindReflectionColRow(part);
      solutions += result.first;
      solutions += (multiplier * result.second);
    }
    return solutions;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  auto solutions =  input.CountWaysToSolve(part);
  cout << "Possible Solutions: " << solutions << endl;
}

};  // aoc2023_13

#endif /* _2023_ADVENTOFCODE_13_H_ */
