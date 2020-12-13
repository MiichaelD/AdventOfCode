/*
  Link:         http://adventofcode.com/2020/day/13
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/13/2020
*/

#ifndef _2020_ADVENTOFCODE_13_H_
#define _2020_ADVENTOFCODE_13_H_

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

namespace aoc2020_13 {
using namespace std;

typedef long long int lli;

bool fillBusses(vector<lli> *input) {
  string line;
  cin >> line;
  lli accum = 0;
  for (int index = 0; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ',') {
      if (accum != 0) {
        input->push_back(accum);
        accum = 0;
      }
      continue;
    } else if (line[index] == 'x') {
      continue;
    }
  }
  if (accum != 0) {
    input->push_back(accum);
    accum = 0;
  }
  return true;
}

void solve1() {
  lli earliest;
  vector<lli> busses;
  cin >> earliest;  
  fillBusses(&busses);

  lli earliestPickup = INT64_MAX;
  int earliestBus = INT_MAX;
  for (lli bus : busses) {
    lli result = earliest / bus;
    if ((result * bus) < earliest) {
      ++result;
    }
    if (result * bus < earliestPickup) {
      cout << "\t bus: " << bus <<  " @ " << result * bus << endl;
      earliestPickup = result * bus;
      earliestBus = bus;
    }
  }
  cout << (earliestPickup - earliest) * earliestBus << endl;
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

};  // aoc2020_13

#endif /* _2020_ADVENTOFCODE_13_H_ */
