/*
  Link:         http://adventofcode.com/2020/day/X
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/06/2020
*/

#ifndef _2021_ADVENTOFCODE_06_H_
#define _2021_ADVENTOFCODE_06_H_

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

namespace aoc2021_06 {  
using namespace std;

vector<vector<string>> getLines() {
  string line;
  vector<vector<string>> lines;
  stringstream ss;
  lines.push_back({});
  while (!cin.eof()) {
    getline(cin, line);
    if (line.empty()) {
      lines.push_back({});
    } else {
      lines.back().push_back(line);
    }
  }
  if (lines.back().empty()) {
    lines.pop_back();
  }
  return lines;
}

void solve(int part = 1) {
  size_t diffAnswers = 0, commonAnswersInGroup = 0;
  for (const vector<string> &groupAnswers : getLines()) {
    unordered_map<char,int> answerReps;
    for (const string &answers : groupAnswers) {
      for (char c : answers) {
        ++answerReps[c];
      }
    }
    for (const auto &entry : answerReps) {
      ++diffAnswers;
      if (entry.second == groupAnswers.size()) {
        ++commonAnswersInGroup;
      }
    }
  }
  cout << "Part 1 - Total different answers: " << diffAnswers << endl;
  cout << "Part 2 - Group's common answers:  " << commonAnswersInGroup << endl;
}

};

#endif /* _2021_ADVENTOFCODE_06_H_ */