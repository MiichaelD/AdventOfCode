/*
  Link:         http://adventofcode.com/2023/day/04
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/04/2023
*/

#ifndef _2023_ADVENTOFCODE_03_H_
#define _2023_ADVENTOFCODE_03_H_

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

namespace aoc2023_03 {
using namespace std;

unordered_map<string, vector<int64_t>> gears_to_parts;

inline void maybeInsertToGear(
  const vector<string>& map, int row, int column, int64_t part) {
  if (map[row][column] != '*') {
    return;
  }
  stringstream ss;
  ss << row << ',' << column;
  gears_to_parts[ss.str()].emplace_back(part);
}

bool isValid(
  const vector<string>& map, int row, int start, size_t end, int64_t part) {
  bool valid = false;
  start = std::max(start, 0);
  end = std::min(end, map[row].size() - 1);
  // cout << "\t\tChecking row: " << row << ". From: " << start << " to End: ";
  // cout << end << endl; 
  for (int i = start; i <= end; ++i) {
    if (row > 0 && !isdigit(map[row - 1][i]) && map[row - 1][i] != '.') {
      maybeInsertToGear(map, row - 1, i, part);
      valid = true;
    }
    if (!isdigit(map[row][i]) && map[row][i] != '.') {
      maybeInsertToGear(map, row, i, part);
      valid = true;
    }
    if (row < (map.size() - 1) && !isdigit(map[row + 1][i]) && 
        map[row + 1][i] != '.') {
      maybeInsertToGear(map, row + 1, i, part);
      valid = true;
    }
  }
  return valid;
}

int64_t findSum(const vector<string>& map) {
  int64_t answer = 0;
  int64_t aux;
  for (int f = 0; f < map.size(); ++f) {
    const string& line = map[f]; 
    for (int c = 0; c < map[f].size(); ++c) {
      if (isdigit(line[c])) {
        int start = c;
        aux = util::getNumberRef(line, c);
        int end = c;
        if (isValid(map, f, start - 1, end, aux)) {
          cout << "\tAdding: " << aux << endl;
          answer += aux;
        }
      }
    }
  }
  return answer;
}

int64_t findEngineRatio(const vector<string>& map) {
  int64_t sum = 0, engineRatio = 0;
  int64_t aux;
  for (int f = 0; f < map.size(); ++f) {
    const string& line = map[f]; 
    for (int c = 0; c < map[f].size(); ++c) {
      if (isdigit(line[c])) {
        int start = c;
        aux = util::getNumberRef(line, c);
        int end = c;
        if (isValid(map, f, start - 1, end, aux)) {
          cout << "\tAdding: " << aux << endl;
          sum += aux;
        }
      }
    }
  }
  for (const auto& entry : gears_to_parts) {
    if (entry.second.size() != 2){
      continue;
    }
    engineRatio += entry.second[0] * entry.second[1];
  }
  return engineRatio;
}

void solve(int part = 1) {
  string input;
  vector<string> map;
  while(std::getline(cin, input)) {
    map.emplace_back(std::move(input));
  }
  auto sum = part == 1 ? findSum(map) : findEngineRatio(map);
  cout << "Answer: " << sum << endl;
}

};  // aoc2023_03

#endif /* _2023_ADVENTOFCODE_03_H_ */
