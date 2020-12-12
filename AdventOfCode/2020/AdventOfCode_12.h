/*
  Link:         http://adventofcode.com/2020/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/12/2020
*/

#ifndef _2020_ADVENTOFCODE_12_H_
#define _2020_ADVENTOFCODE_12_H_

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

namespace aoc2020_12 {
using namespace std;

vector<string> getLinesWithLn() {
  string line;
  vector<string> lines;
  stringstream ss;
  while (!cin.eof()) {
    getline(cin, line);
    ss << line;
    if (line.empty()) {
      lines.push_back(ss.str());
      // cout << "Passport: " << ss.str() << endl;
      ss.str("");
      ss.clear();
    } else {
      ss << ' ';
    }
  }
  lines.push_back(ss.str());
  // cout << "Passport: " << ss.str() << endl;
  return lines;
}

void solve1() {
  string input;
  cin >> input;  
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

};  // aoc2020_12

#endif /* _2020_ADVENTOFCODE_12_H_ */
