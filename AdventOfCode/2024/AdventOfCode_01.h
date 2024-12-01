/*
  Link:         http://adventofcode.com/2024/day/1
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2024/AdventOfCode_01_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/01/2024
*/

#ifndef _2024_ADVENTOFCODE_01_H_
#define _2024_ADVENTOFCODE_01_H_

#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>


namespace aoc2024_01 {
  using namespace std;

  void solve1() {
    std::string input;
    vector<int> left, right;
    int64_t result = 0;
    while (!cin.eof()) {
      cin >> left.emplace_back() >> right.emplace_back();
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    for (int i = 0 ; i < left.size(); ++i) {
      int delta = abs(left[i] - right[i]);
      cout << (i+1) << ".- Delta: " << delta << endl;
      result += delta;
    }
    cout << result << endl;
  }

  void solve2() {
    std::string input;
    vector<int> left;
    left.reserve(1001);
    unordered_map<int, int> right;
    int r;
    int64_t result = 0;
    while (!cin.eof()) {
      if (!(cin >> left.emplace_back())) {
        left.pop_back(); 
        break;
      }
      cin >> r;
      ++right[r];
    }
    for (int l : left) {
      cout << l << " similarity score: ";
      if (auto it = right.find(l); it != right.end()) {
        int score = (l * it->second);
        cout << score << endl;
        result += score;
      } else {
        cout << 0 << endl;
      }
    }
    cout << result << endl;
  }

  void solve(int part = 1) {
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};
#endif /* _2024_ADVENTOFCODE_01_H_ */
