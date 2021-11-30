/*
  Link:         http://adventofcode.com/2020/day/15
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/15/2020
*/

#ifndef _2021_ADVENTOFCODE_15_H_
#define _2021_ADVENTOFCODE_15_H_

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

namespace aoc2021_15 {
using namespace std;

vector<uint64_t> getInput() {
  vector<uint64_t> input;
  string line;
  cin >> line;
  uint64_t accum = 0;
  size_t bInd = 0;
  for (size_t index = 0; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ',') {
      input.push_back(accum);
      accum = 0;
    }
  }
  if (accum != 0) {
    input.push_back(accum);
    accum = 0;
  }
  return input;
}

void solve(int part = 1) {
  size_t kTotalTurns = part == 1 ? 2020 : 30000000;
  vector<uint64_t> input = getInput();
  unordered_map<uint64_t,deque<size_t>> seen;
  uint64_t last;
  for (size_t index = 0; index < input.size(); ++index) {
    last = input[index];
    cout << "Turn " << (index + 1) << ": " << last << endl;
    seen[last].push_back(index);
  }
  while (input.size() < kTotalTurns) {
    const auto &it = seen.find(last);
    deque<size_t> &reps = it->second;
    // for (int i = 0 ; i < reps.size(); ++i ){
    //   cout << "\tNumber " << last << " has been seen @ " << (reps[i] + 1) << endl;
    // }
    uint64_t number = it->second.size() < 2 ? 0 : (reps.back() - reps.at(reps.size() - 2));
    // cout << "Turn " << (input.size() + 1) << ": " << number << endl;
    // while(seen[number].size() > 1) {
    //   seen[number].pop_front();  // We only need 2 items at a time.
    // }
    seen[number].push_back(input.size());
    input.push_back(number);
    last = number;
  }
  cout << "Turn " << input.size() << ": " << last << endl;
  cout << "Total elements seen: " << seen.size() << endl;
}

};  // aoc2021_15

#endif /* _2021_ADVENTOFCODE_15_H_ */
