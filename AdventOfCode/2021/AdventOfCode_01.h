/*
  Link:         http://adventofcode.com/2020/day/1
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/01/2020
*/

#ifndef _2021_ADVENTOFCODE_01_H_
#define _2021_ADVENTOFCODE_01_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace aoc2021_01 {
  using namespace std;

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

  unsigned long long solve2() {
    deque<int> slidingWindow;
    int accum = 0, entry = 0, delta = 0, prevAccum = -1;
    while(!cin.eof()) {
      cin >> entry;
      slidingWindow.push_back(entry);
      accum += entry;
      if (slidingWindow.size() == 3) {
        cout << "\tAccum: " << accum << endl;
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
