/*
  Link:         http://adventofcode.com/2021/day/1
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2021/AdventOfCode_01_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         11/30/2021
*/

#ifndef _2021_ADVENTOFCODE_01_H_
#define _2021_ADVENTOFCODE_01_H_

#include <iostream>
#include <deque>

namespace aoc2021_01 {
  using namespace std;

  // Iterate each line, count the amount of times that current value is bigger than the last.
  unsigned long long solve1() {
    int prevEntry, entry, delta = 0;
    cin >> prevEntry;
    while(!cin.eof()) {
      cin >> entry;
      if (entry > prevEntry) {
        ++delta;
      }
      prevEntry = entry;
    }
    return delta;
  }

  // Keep a sliding window of 3 items, count the amount of times that the current sliding window's
  // sum is greater than the previous sliding window's sum.
  unsigned long long solve2() {
    deque<int> slidingWindow;
    int accum = 0, entry = 0, delta = 0, prevAccum = -1;
    while(!cin.eof()) {
      cin >> entry;
      accum += entry;
      slidingWindow.push_back(entry);
      if (slidingWindow.size() == 3) {
        if (prevAccum != -1 && prevAccum < accum) {
          ++delta;
        }
        prevAccum = accum;
        accum -= slidingWindow.front();
        slidingWindow.pop_front();
      }
    }
    return delta;
  }

  void solve(int part = 1) {
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }
};
#endif /* _2021_ADVENTOFCODE_01_H_ */
