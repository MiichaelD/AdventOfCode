/*
  Link:         http://adventofcode.com/2022/day/6
  Compiling:    g++ -std=c++17 main.cpp -o main && cat 2022/AdventOfCode_06_input.txt | ./main
  Programmer:   Michael Duarte.
  Date:         12/12/2022
*/

#ifndef _2022_ADVENTOFCODE_06_H_
#define _2022_ADVENTOFCODE_06_H_

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

namespace aoc2022_06 {  
using namespace std;
const int kStartPacketSize = 4;
const int kStartMessageSize = 14;

size_t findPos(const string& input, int target) {
  deque<char> q;
  unordered_map<char, int> seen;
  for (size_t i = 0; i < input.size(); ++i) {
    q.push_back(input[i]);
    if (q.size() > target) {
      char removed = q.front();
      q.pop_front();
      --seen.at(removed);
      if (seen.at(removed) == 0) {
        seen.erase(removed);
      }
    }
    if (seen.find(input[i]) == seen.end()) {
      seen.insert({input[i], 0});
    }
    ++seen.at(input[i]);
    if (seen.size() == target) {
      return i;
    }
  }
  return 0;
}

void solve(int part = 1) {
  string input;
  while(getline(cin, input)) {
    size_t solution = findPos(input, part == 1 ? kStartPacketSize : kStartMessageSize) + 1;
    cout << "End of first Marker at pos: " << solution << endl;
  }
}

};

#endif /* _2022_ADVENTOFCODE_06_H_ */