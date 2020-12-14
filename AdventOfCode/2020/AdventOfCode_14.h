/*
  Link:         http://adventofcode.com/2020/day/14
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/14/2020
*/

#ifndef _2020_ADVENTOFCODE_14_H_
#define _2020_ADVENTOFCODE_14_H_

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

namespace aoc2020_14 {
using namespace std;

typedef unordered_map<size_t, uint64_t> Memory;


Memory memory;
string mask;

vector<string>  getInput() {
  vector<string> input;
  string line;
  while(cin && !cin.eof()) {
    getline(cin, line);
    input.push_back(line);
  }
  return input;
}

uint64_t getMaskedValue(uint64_t value) {
  size_t len = mask.size() - 1;
  for (int i = len; i >= 0; --i) {
    switch(mask[i]) {
      case '0': value &= ~(1ull << (len - i)); break;
      case '1': value |= (1ull << (len - i)); break;
      case 'X':
      default: break;
    }
  }
  return value;
}

bool processInput(const string &input) {
  if (input.empty()) return false;
  if (input[1] == 'a') {
    mask = input.substr(6);;
    cout << "Mask = " << mask << endl;
    return true;
  } else if (input[1] == 'e') {
    size_t index = atoi(&input.c_str()[4]);
    size_t digits = log10(index) + 1;
    uint64_t value = atoi(&input.c_str()[7 + digits]);
    uint64_t maskedValue = getMaskedValue(value);
    memory[index] = maskedValue;
    cout << "Memory @ " << index << " = " << value << " Masked = " << maskedValue << endl;
    return true;
  }
  return false;
}

bool processInput(const vector<string> &input) {
  for (const string &in : input) {
    if (!processInput(in)) {
      return false;
    }
  }
  return true;
}

uint64_t getMemoryValuesSum() {
  uint64_t result = 0;
  for (const auto &entry : memory) {
    cout << "\tMemory @ " << entry.first << " = " << entry.second << endl;
    result += entry.second;
  }
  return result;
}

void solve1() {
  vector<string> input = getInput();
  processInput(input);
  uint64_t result = getMemoryValuesSum();
  cout << "Result: " << result << ". After adding " << memory.size() << " values" << endl;
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

};  // aoc2020_14

#endif /* _2020_ADVENTOFCODE_14_H_ */
