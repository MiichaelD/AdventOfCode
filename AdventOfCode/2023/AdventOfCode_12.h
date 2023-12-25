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
  for (int i = 0; i < times; ++i) {
    cout << c;
  }
}

void printProgress(
    const string& line, int index, const deque<int>& running_reality, int spring_count) {
  printRepeated(index * 2, ' '); printRepeated(index, ' '); cout << "V" << endl;
  printRepeated(index * 2, ' '); cout << line << endl;
  printRepeated(index * 2, ' '); util::printDeque(running_reality); cout << endl;
  printRepeated(index * 2, ' '); cout << "Sprint count: " <<  spring_count << endl;
}

struct Challenge {
  string read;
  deque<int> reality;
  void print() const {
    cout << "Read: " << read << ", for ";
    util::printDeque(reality); cout << endl;
  }
};

int64_t CountWaysToSolve(
    const Challenge& challenge, int index, deque<int>& running_reality,
    int spring_count);

int ProcessDelimiter(const Challenge& challenge, int index, deque<int>& running_reality,
    int& spring_count, int64_t& solutions) {
  if (spring_count == 0) {  // skip block of delimiters
    return 0;
  }
  if (running_reality.front() != spring_count) {  // Not valid
    return -1;
  }
  running_reality.pop_front();
  solutions += CountWaysToSolve(challenge, index + 1, running_reality, 0);
  running_reality.push_front(spring_count);
  spring_count = 0;
  return 0;
}

int64_t CountWaysToSolve(
    const Challenge& challenge, int index, deque<int>& running_reality,
    int spring_count = 0) {
  int64_t solutions = 0;
  if (spring_count && (running_reality.front() == spring_count)) {
    running_reality.pop_front();
    solutions += CountWaysToSolve(challenge, index + 1, running_reality, 0);
    running_reality.push_front(spring_count);
    spring_count = 0;
  }
  for (; running_reality.size() && index < challenge.read.size(); ++index) {
    printProgress(challenge.read, index, running_reality, spring_count);
    switch(challenge.read[index]) {
      case kDelimiter: {
        if (spring_count == 0) {  // skip block of delimiters
          continue;
        }
        if (running_reality.front() != spring_count) {
          return 0;
        }
        running_reality.pop_front();
        solutions += CountWaysToSolve(challenge, index + 1, running_reality, 0);
        running_reality.push_front(spring_count);
        spring_count = 0;
      } break;
      case kOptional:{
        // Try not consuming.
         printRepeated(index * 2, ' '); cout << "Considering it a ." << endl;
        solutions += CountWaysToSolve(challenge, index + 1, running_reality, spring_count);
        // Then try consuming, and there should be a separation block;
        printRepeated(index * 2, ' '); cout << "Considering it a #" << endl;
        return solutions +  CountWaysToSolve(challenge, index + 1, running_reality, spring_count + 1);
      } break;
      case kSpring: {
        ++spring_count; 
      } break;
    }
  }
  if (spring_count == running_reality.front()) {  // If we match on the last pending check.
    solutions += (running_reality.size() != 1) ? 0 : 1;
  }
  return solutions;
}

inline int64_t CountWaysToSolve(const Challenge& challenge, int index = 0) {
  deque<int> running_reality = challenge.reality;
  return CountWaysToSolve(challenge, index, running_reality);
}

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
    }
    return input;
  }
  void print() const {
    for (const auto& challenge : challenges) {
      challenge.print();
    }
  }
  int processed = 0;
  int64_t CountWaysToSolve() {
    int64_t solutions = 0;
    for (const auto& challenge : challenges) {
      cout << "Case: "; challenge.print();
      auto result = aoc2023_12::CountWaysToSolve(challenge);
      cout << "Produced: " << result << endl << endl;
      solutions += result;
      if (++processed == 2) {
        break;
      }
    }
    return solutions;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  auto solutions =  input.CountWaysToSolve();
  cout << "Possible Solutions: " << solutions << endl;
}

};  // aoc2023_12

#endif /* _2023_ADVENTOFCODE_12_H_ */
