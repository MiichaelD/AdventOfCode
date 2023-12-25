/*
  Link:         http://adventofcode.com/2023/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/12/2023
*/

#ifndef _2023_ADVENTOFCODE_12_H_
#define _2023_ADVENTOFCODE_12_H_

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

namespace aoc2023_12 {
using namespace std;

constexpr char kOptional = '?';
constexpr char kSpring = '#';
constexpr char kDelimiter = '.';

inline void printRepeated(int times, char c) {
  for (int i = 0; i < times; ++i) { cout << c; }
}

inline void StringToVector(const string& in, vector<char>& vc) {
  vc.reserve(in.size());
  for (char c : in) {
    vc.emplace_back(c);
  }
}

struct Challenge {
  string read;
  deque<int> reality;
  deque<vector<char>> options;

  void ExpandOptions() {
    StringToVector(read, options.emplace_back());
    for (int i = 0; i < read.size(); ++i) {
      switch(read[i]) {
        case kOptional: {
            // Duplicate options. 1/2 as Delimiter other 1/2  as spring
            size_t total = options.size();
            for (int o = 0; o < total; ++o) {
              options.emplace_back(options[o])[i] = kSpring;
              options[o][i] = kDelimiter;
            }
          }
          break;
        case kSpring:
        case kDelimiter:
        default: break;
        
      }
    }
  }
  void print() const {
    cout << "Read: " << read << ", for ";
    util::printDeque(reality); 
    // cout << endl << "Expanded options:" << endl;
    // for (const auto& option : options) {
    //   util::printVector(option); cout << endl;
    // }
    cout << endl;
  }

  bool IsOptionValid(int option_index) const {
    int spring_count = 0;
    auto expected = reality;
    const auto& option = options.at(option_index);
    // util::printVector(option); cout << endl;
    for (int i = 0; i < option.size(); ++i) {
      switch(option[i]) {
        case kSpring: {
          ++spring_count;
          if (!expected.size()) {  // Not satisfied
            // printRepeated(i, ' '); cout << "^\tUNSatisfied: due no more expectations" << endl;
            return false;
          }
        }
        break;
        case kDelimiter: {
          if (spring_count) {
            if (spring_count != expected.front()) {
              return false;
            }
            expected.pop_front();
          }
          spring_count = 0;
        }
        break;
      }
    }
    if (spring_count) {
      if (!expected.size()) {
        // cout << "\tUNSatisfied due to extra " << spring_count << " springs" << endl;
        return false;
      }
      if (spring_count != expected.front()) {
        // cout << "\tUNSatisfied due  " << spring_count << " springs != " << expected.front() << endl;
        return false;
      }
      expected.pop_front();
    }
    if (!expected.size()) {
      // cout << "\tSatisfied" << endl;
      return true;
    }
    // cout << "\tUNSatisfied due to missing " << expected.size() << " expectations" << endl;
    return false;
  }

  int64_t CountValidOptions() const {
    int64_t solutions = 0;
    for (int op_ind = 0; op_ind < options.size(); ++op_ind) {
      solutions += IsOptionValid(op_ind);
    }
    return solutions;
  }
};

struct PuzzleInput {
  vector<Challenge> challenges;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    while(!cin.eof()) {
      input.challenges.emplace_back();
      cin >> input.challenges.back().read;
      string thing_to_parse;
      cin >> thing_to_parse;
      for (int i = 0; i < thing_to_parse.size(); ++i) {
        input.challenges.back().reality.emplace_back(
        util::getNumberRef(thing_to_parse, i));
      }
      input.challenges.back().ExpandOptions();
    }
    return input;
  }
  void print() const {
    for (const auto& challenge : challenges) {
      challenge.print();
    }
  }

  int64_t CountWaysToSolve() {
    int64_t solutions = 0;
    for (const auto& challenge : challenges) {
      cout << "Case: "; challenge.print();
      auto result = challenge.CountValidOptions();
      cout << "Produced: " << result << endl << endl;
      solutions += result;
    }
    return solutions;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  // input.print();
  auto solutions =  input.CountWaysToSolve();
  cout << "Possible Solutions: " << solutions << endl;
}

};  // aoc2023_12

#endif /* _2023_ADVENTOFCODE_12_H_ */
