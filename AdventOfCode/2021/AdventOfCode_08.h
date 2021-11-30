/*
  Link:         http://adventofcode.com/2020/day/08
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2020
*/

#ifndef _2021_ADVENTOFCODE_08_H_
#define _2021_ADVENTOFCODE_08_H_

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
#include "../util/robot_2020.h"

namespace aoc2021_08 {
using namespace std;


void solve(int part = 1) {
  aoc2020::Robot robot;
  robot.getCommands();
  if (part == 1) {
    robot.findLoop();
  } else {
    robot.findCorruptedCommand();
  }
  cout << "Accumulator: " << robot.accumulator << endl;
}

};  // aoc2021_08

#endif /* _2021_ADVENTOFCODE_08_H_ */
