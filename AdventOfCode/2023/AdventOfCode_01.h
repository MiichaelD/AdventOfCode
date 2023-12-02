/*
  Link:         http://adventofcode.com/2023/day/1
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2023/AdventOfCode_01_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/02/2023
*/

#ifndef _2023_ADVENTOFCODE_01_H_
#define _2023_ADVENTOFCODE_01_H_

#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>

 std::unordered_map<std::string, int> numbers = 
 {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4},  {"five", 5},
 {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9 }};

namespace aoc2023_01 {
  using namespace std;

  int64_t solve1(const std::string& input){
    int first = 0;
    int last = 0;
    for (int i = 0 ; i < input.size(); ++i) {
      if (isdigit(input.at(i))) {
        first = input.at(i) - '0';
        break;
      }
    }
    for (int i = input.size() - 1 ; i >= 0; --i) {
      if (isdigit(input.at(i))) {
        last = input.at(i) - '0';
        break;
      }
    }
    cout << "first: " << first << ". last: " << last << endl;
    return first * 10 + last;
  }

  int64_t solve2(const std::string& input) {
    int first = 0;
    int last = 0;
    for (int i = 0 ; i < input.size(); ++i) {
      if (isdigit(input.at(i))) {
        first = input.at(i) - '0';
        break;
      } else {
        bool match = true;
        for (const auto& entry : numbers) {
          match = true;
          for (int x = 0; x < entry.first.size(); ++x) {
            if (input.at(i + x) != entry.first[x]) {
              match = false;
              break;
            }
          }
          if (match) {
            first = entry.second;
            break;
          }
        }
        if (match) {
          break;
        }
      }
    }
    for (int i = input.size() - 1 ; i >= 0; --i) {
      if (isdigit(input.at(i))) {
        last = input.at(i) - '0';
        break;
      } else {
        bool match = true;
        for (const auto& entry : numbers) {
          match = true;
          for (int x = 0; x < entry.first.size(); ++x) {
            if (input.at(i - x) != entry.first[entry.first.size() -1 - x]) {
              match = false;
              break;
            }
          }
          if (match) {
            last = entry.second;
            break;
          }
        }
        if (match) {
          break;
        }
      }
    }
    cout << "first: " << first << ". last: " << last << endl;
    return first * 10 + last;
  }


  void solve(int part = 1) {
    std::string input;
    int64_t acum = 0;
    cout << "solving part: " << part << endl;
    while (!cin.eof()) {
      cin >> input;
      cout << "input: " << input << endl;
      acum += (part == 1 ? solve1(input) : solve2(input));

    }
    cout << acum << endl;
  }
};
#endif /* _2023_ADVENTOFCODE_01_H_ */
