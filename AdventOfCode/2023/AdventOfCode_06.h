/*
  Link:         http://adventofcode.com/2023/day/06
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/06/2023
*/

#ifndef _2023_ADVENTOFCODE_06_H_
#define _2023_ADVENTOFCODE_06_H_

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

namespace aoc2023_06 {
using namespace std;

struct PuzzleInput {
  vector<int64_t> time;
  vector<int64_t> distance;
};

void fillVectorFromLine(
  const std::string& line, int part, vector<int64_t>& vector) {
  if (part == 2 ) {
    int64_t acum = 0;
    for (int i = 2; i < line.size(); ++i) {
      if (isdigit(line[i])) {
        acum *= 10;
        acum += line[i] - '0';
      }
    }
    vector.push_back(acum);
  } else {
    for (int i = 2; i < line.size(); ++i) {
      int aux = util::getNumberRef(line, i);
      if (aux == 0) continue;
      vector.emplace_back(aux);
    }
  }
}

PuzzleInput parseInput(int part = 1) {
  string line;
  cin >> line;  // Time:
  getline(cin, line);
  PuzzleInput input;
  fillVectorFromLine(line, part, input.time);
  util::printVector(input.time);
  cout << endl;
  
  cin >> line;  // Distance
  getline(cin, line);
  fillVectorFromLine(line, part, input.distance);
  util::printVector(input.distance);
  cout << endl;
  return input;
}

int64_t countWaysToWin(const PuzzleInput& input, int index) {
  int64_t result = 0;
  int64_t duration = input.time[index];
  for (int i = 1; i < duration; ++i) {
    int64_t distance = (duration - i) * i;
    if (distance > input.distance[index]) {
      // cout << "Pressing for " << i << " ms - boat travels: " << distance << endl;
      ++result;
    }
  }
  cout << endl;
  return result;
}

void solve(int part = 1) {
  PuzzleInput input = parseInput(part);
  int64_t value = 1;
  for (int i = 0; i < input.time.size(); ++i) {
    value *= countWaysToWin(input, i);
  }
  cout << "Final value: " << value << endl;
}

};  // aoc2023_06

#endif /* _2023_ADVENTOFCODE_06_H_ */
