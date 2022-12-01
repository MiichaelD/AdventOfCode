/*
  Link:         http://adventofcode.com/2020/day/17
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/17/2020
*/

#ifndef _2021_ADVENTOFCODE_17_H_
#define _2021_ADVENTOFCODE_17_H_

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

namespace aoc2021_17 {
using namespace std;
// TEST CASE
  // target area: x=20..30, y=-10..-5
  // pair<int,int> minPoint(20, -5);
  // pair<int,int> maxPoint(30, -10);
  //  // Solve xvel_min * (xvel_min + 1) / 2 > xmin
  // int xvelMin = 6;    // Test: 6 

// REAL CASE
  // target area: x=240..292, y=-90..-57
  pair<int,int> minPoint(240, -57);
  pair<int,int> maxPoint(292, -90); 
// Solve xvel_min * (xvel_min + 1) / 2 > xmin
  int xvelMin = 11;   // Real: 11

inline bool onX(int x) {
  return minPoint.first <= x && x <= maxPoint.first;
}

inline bool onY(int y) {
  return minPoint.second >= y && y >= maxPoint.second;
}


void solve(int part = 1) {
  // https://www.reddit.com/r/adventofcode/comments/ri9kdq/2021_day_17_solutions/
  vector<unordered_set<int>> validX, validY;
  for (int xVel = xvelMin; xVel <= maxPoint.first; ++xVel) {
    int step = 0;
    int x = 0;
    int xv = xVel;
    unordered_set<int> valid;
    while (step < (2 * abs(maxPoint.second) + 2) && x <= maxPoint.first) {
      x += xv;
      xv = max(xv - 1, 0);
      ++step;
      if (onX(x)) {
        valid.insert(step);
      }
    }
    validX.push_back(valid);
  }

  for (int yVel = maxPoint.second; yVel < (-maxPoint.second); ++yVel) {
    int step = 0;
    int y = 0;
    int yv = yVel;
    unordered_set<int> valid;
    while (y > maxPoint.second) {
      y += yv;
      --yv;
      ++step;
      if (onY(y)) {
        valid.insert(step);
      }
    }
    validY.push_back(valid);
  }


  size_t totalInitialVelocities = 0;
  for (const auto &validSetX : validX) {
    for (const auto &validSetY : validY) {
      for (int val : validSetX) {
        if (validSetY.contains(val)){
          ++totalInitialVelocities;
          break;
        }
      }
    }
  }
  int y = abs(maxPoint.second);
  cout << "Part 1: Max height: " << (y * (y - 1)) / 2 << endl; // 4005
  cout << "Part 2: Initial Velocities: " << totalInitialVelocities << endl;  // 2953
}

};  // aoc2021_17 - test: 112 - pt 1: 252

#endif /* _2021_ADVENTOFCODE_17_H_ */
