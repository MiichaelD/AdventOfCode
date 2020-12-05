/*
  Link:         http://adventofcode.com/2020/day/X
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/06/2020
*/

#ifndef _2020_ADVENTOFCODE_06_H_
#define _2020_ADVENTOFCODE_06_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
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

namespace aoc2020_06 {  
  using namespace std;

  void solve1() {   
    string input;
    cin >> input;
    
  }

  void solve2() {    
    
  }

  void solve(int part = 1) {
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};

#endif /* _2020_ADVENTOFCODE_06_H_ */