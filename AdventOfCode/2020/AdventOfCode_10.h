/*
  Link:         http://adventofcode.com/2020/day/10
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/10/2020
*/

#ifndef _2020_ADVENTOFCODE_10_H_
#define _2020_ADVENTOFCODE_10_H_

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

namespace aoc2020_10 {
using namespace std;


vector<size_t> getNumbers() {
  vector<size_t> numbers;
  size_t number;
  while(!cin.eof()){
    cin >> number;
    numbers.push_back(number);
  }
  sort(numbers.begin(), numbers.end());
  return numbers;
}

bool doMagic(const vector<size_t> &numbers, vector<int> &deltaCount) {
  size_t accum = 0;
  size_t deviceJolt = numbers.back() + 3;
  for (int n : numbers) {
    int delta = n - accum;
    cout << n << ", delta: " << delta << endl;
    if (delta > 3) {
      return false;
    }
    accum += delta;
    ++deltaCount[delta];
  }
  ++deltaCount[deviceJolt - accum];
  // accum += deviceJolt - accum;
  return true;
}

void print(const vector<int> &deltaCount) {
  for (int i = 0 ; i < deltaCount.size(); ++i) {
    if (deltaCount[i]) {
      cout << i << " :"  << deltaCount[i] << endl;
    }
  }
}

void solve1() {
  vector<size_t> numbers = getNumbers();
  vector<int> deltaCount = {0, 0, 0, 0};
  bool magic = doMagic(numbers, deltaCount);
  if (magic) {
    print(deltaCount);
  } else {
    cout << "Not possible" << endl;
  }
  
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

};  // aoc2020_10

#endif /* _2020_ADVENTOFCODE_10_H_ */
