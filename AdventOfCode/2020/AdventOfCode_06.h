/*
  Link:         http://adventofcode.com/2020/day/X
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/06/2020
*/

#ifndef _2020_ADVENTOFCODE_06_H_
#define _2020_ADVENTOFCODE_06_H_

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

namespace aoc2020_06 {  
using namespace std;

vector<string> getLines() {
  string line;
  vector<string> lines;
  stringstream ss;
  while (!cin.eof()) {
    getline(cin, line);
    ss << line;
    if (line.empty()) {
      lines.push_back(ss.str());
      // cout << "Passport: " << ss.str() << endl;
      ss.str("");
      ss.clear();
    }
  }
  if (!ss.str().empty())
    lines.push_back(ss.str());
  // cout << "Passport: " << ss.str() << endl;
  return lines;
}

void solve1() {   
  size_t diffAnswers = 0;
  for (const auto &str : getLines()) {
    unordered_set<char> answers;
    for (char c : str) {
      answers.insert(c);
    }
    diffAnswers += answers.size();
  }
  cout << "Total answers: " << diffAnswers << endl;
}

vector<vector<string>> getLines2() {
  string line;
  vector<vector<string>> lines;
  stringstream ss;
  lines.push_back({});
  while (!cin.eof()) {
    getline(cin, line);
    lines.back().push_back(line);
    if (line.empty()) {
      lines.push_back({});
    }
  }
  if (lines.back().empty()) {
    lines.pop_back();
  }
  return lines;
}

void solve2() {    
  size_t diffAnswers = 0;
  for (const vector<string> &groupAnswers : getLines2()) {
    cout << "Group" << endl;
    unordered_map<char,int> answerReps;
    for (const string &answers : groupAnswers) {
      cout << "\tanswers: " << answers << endl;
      for (char c : answers) {
        ++answerReps[c];
      }
    }
    for (const auto &entry : answerReps) {
      if (entry.second == groupAnswers.size() - 1) {
        ++diffAnswers;
      }
    }
  }
  cout << "Total answers: " << diffAnswers << endl;
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } else {
    solve2();
  }
}

};

#endif /* _2020_ADVENTOFCODE_06_H_ */