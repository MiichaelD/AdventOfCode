/*
  Link:         http://adventofcode.com/2022/day/4

  Description:
  Compiling:    g++ -std=c++11 main.cpp -o main && cat 2022/AdventOfCode_04_input.txt | ./main

  Programmer:   Michael Duarte.

  Date:         12/07/2022
*/

#ifndef _2022_ADVENTOFCODE_04_H_
#define _2022_ADVENTOFCODE_04_H_

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../util/util.h"

namespace aoc2022_04 {
using namespace std;

typedef pair<int,int> section;
typedef pair<section,section> section_pair;


bool isSectionPairFullyOverlapping(const section_pair& sec_pair) {
  const section& s1 = sec_pair.first;
  const section& s2 = sec_pair.second;
  return ((s1.first <= s2.first && s1.second >= s2.second) || 
      (s2.first <= s1.first && s2.second >= s1.second));
}

bool isSectionPairPartiallyOverlapping(const section_pair& sec_pair) {
  if (isSectionPairFullyOverlapping(sec_pair)) {
    return true;
  }
  const section& s1 = sec_pair.first;
  const section& s2 = sec_pair.second;
  return ((s1.first <= s2.first && s1.second >= s2.first) || 
      (s1.first <= s2.second&& s1.second >= s2.second));
}


void solve(int part = 1) {
  string input;
  size_t solution = 0;
  while(getline(cin, input)) {
    int index = 0;
    section_pair sec_pair;
    sec_pair.first.first = util::getNumberRef(input, index);
    sec_pair.first.second = util::getNumberRef(input, ++index);
    sec_pair.second.first = util::getNumberRef(input, ++index);
    sec_pair.second.second = util::getNumberRef(input, ++index);
    util::printPair(sec_pair.first);
    util::printPair(sec_pair.second, true);
    if (part == 1
          ? isSectionPairFullyOverlapping(sec_pair)
          : isSectionPairPartiallyOverlapping(sec_pair)) {
      ++solution;
      cout << "\t" << "Overlap detected" << endl;
    }
  }
  cout << "Total overlapping sections pairs: " << solution << endl;
}

};  // namespace aoc2022_04

#endif /* _2022_ADVENTOFCODE_04_H_ */
