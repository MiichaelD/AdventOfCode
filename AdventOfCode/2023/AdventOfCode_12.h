/*
  Link:         http://adventofcode.com/2023/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/12/2023
*/

#ifndef _2023_ADVENTOFCODE_12_H_
#define _2023_ADVENTOFCODE_12_H_

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

namespace aoc2023_12 {
using namespace std;

struct PuzzleInput {
  struct Challenge {
    string read;
    vector<int> reality;
    void print() const {
      cout << "Read: " << read << ", for ";
      util::printVector(reality); cout << endl;
    }
    int64_t CountWaysToSolve() const {
      int64_t solutions = 0;
      cout << "\t";
      
      return solutions;
    }
  };
  vector<Challenge> challenges;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    while(!cin.eof()) {
      input.challenges.emplace_back();
      cin >> input.challenges.back().read;
      string thing_to_parse;
      cin >> thing_to_parse;
      for (int i = 0; i < thing_to_parse.size(); ++i) {
         input.challenges.back().reality.emplace_back(
          util::getNumberRef(thing_to_parse, i));
      }
    }
    input.print();
    return input;
  }
  void print() const {
    for (const auto& challenge : challenges) {
      challenge.print();
    }
  }
  int64_t CountWaysToSolve() {
    int64_t solutions = 0;
    for (const auto& challenge : challenges) {
      cout << "Case: ";
      challenge.print();
      solutions += challenge.CountWaysToSolve();
    }
    return solutions;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  auto solutions =  input.CountWaysToSolve();
  cout << "Possible Solutions: " << solutions << endl;
}

};  // aoc2023_12

#endif /* _2023_ADVENTOFCODE_12_H_ */
