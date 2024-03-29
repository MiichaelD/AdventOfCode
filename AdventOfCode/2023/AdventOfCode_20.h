/*
  Link:         http://adventofcode.com/2023/day/20
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/20/2023
*/

#ifndef _2023_ADVENTOFCODE_20_H_
#define _2023_ADVENTOFCODE_20_H_

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

namespace aoc2023_20 {
using namespace std;

struct PuzzleInput {

  static PuzzleInput GetInput(int part) {
    PuzzleInput input;

    return input;
  }

};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput(part);
}

};  // aoc2023_20

#endif /* _2023_ADVENTOFCODE_20_H_ */
