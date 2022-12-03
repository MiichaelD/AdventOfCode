/*
  Link:         http://adventofcode.com/2022/day/3
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main && cat 2022/AdventOfCode_03_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/03/2022
*/

#ifndef _2022_ADVENTOFCODE_03_H_
#define _2022_ADVENTOFCODE_03_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "../util/util.h"


namespace aoc2022_03 {
  using namespace std;

  // Split input in half, find repated char in both parts.
  size_t solve1() {
    string input;
    size_t acumPriority = 0;
    while(!cin.eof()) {
      cin >> input;
      if (input.empty()) {
        break;
      }
      size_t len = input.size() / 2;
      unordered_set<char> seen;
      char repeated;
      int repeatedPriority = 0;
      for (size_t i = 0; i < len; ++i) {
        seen.insert(input[i]);
      }
      for (; len < input.size(); ++len) {
        if (seen.find(input[len]) != seen.end()) {
          repeated = input[len];
          if (repeated >= 'a') {
            repeatedPriority = repeated - 'a' + 1;
          } else {
            repeatedPriority = 26 + repeated - 'A' + 1;
          }
          break;
        }
      }
      acumPriority += repeatedPriority;
      cout << "Repeated letter: " << repeated << ". Priority: " << repeatedPriority << endl;
    }
    return acumPriority;
  }

  // Find repeated char in the last 3 lines.
  uint64_t solve2() {
    string input;
    size_t acumPriority = 0;
    int entry = 0;
    vector<unordered_set<char>> seenPerLine;
    while(!cin.eof()) {
      cin >> input;
      if (input.empty()) {
        break;
      }
      if ((++entry % 3) == 0) {
        for (size_t i = 0; i < input.size(); ++i) {
          if (seenPerLine.front().find(input[i]) != seenPerLine.front().end() &&
              seenPerLine.back().find(input[i]) != seenPerLine.back().end()) {
            char repeated = input[i];
            size_t repeatedPriority = 0;
            if (repeated >= 'a') {
              repeatedPriority = repeated - 'a' + 1;
            } else {
              repeatedPriority = repeated - 'A' + 27;
            }
            acumPriority += repeatedPriority;
            std::cout << "Repeated letter: " << repeated << ". Priority: "
                << repeatedPriority << endl;
            break;
          }
        }
        seenPerLine.clear();
        continue;
      }
      seenPerLine.emplace_back(unordered_set<char>());
      for (size_t i = 0; i < input.size(); ++i) {
        seenPerLine.back().insert(input[i]);
      }
    }
    return acumPriority;
  }

  void solve(int part = 1) {
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2022_ADVENTOFCODE_03_H_ */
