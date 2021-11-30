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
    unordered_set<int>  entries;
    int entry, delta;
    while(!std::cin.eof()) {
      cin >> entry;
      delta = 2020 - entry;
      if (entries.find(delta) != entries.end()) {
        return delta * entry;
      }
      entries.insert(entry);
    }
    return -1;
  }

  unsigned long long solve2() {
    vector<int>  entriesVec;
    unordered_set<int> entries;
    int entry;
    unsigned long long delta;
    while(!std::cin.eof()) {
      cin >> entry;
      entriesVec.push_back(entry);
      entries.insert(entry);
    }

    for (int i = 0; i < entriesVec.size(); ++i) {
      for (int j = i + 1; j < entriesVec.size(); ++j) {
        delta = 2020 - entriesVec[i] - entriesVec[j];
        if (entries.find(delta) != entries.end()) {
          return delta * entriesVec[i] * entriesVec[j];
        }
      }
    }
    return -1;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2021_ADVENTOFCODE_01_H_ */
