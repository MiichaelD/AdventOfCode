/*
  Link:         http://adventofcode.com/2020/day/3
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/03/2020
*/

#ifndef _2021_ADVENTOFCODE_03_H_
#define _2021_ADVENTOFCODE_03_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "../util/util.h"


namespace aoc2021_03 {
  using namespace std;

  inline size_t safeIndex(size_t x, size_t len) {
    return  x >= len ? x - len : x;
  }

  vector<string> getLines() {
    string line;
    vector<string> lines;
    while(!cin.eof()) {
      getline(cin, line);
      lines.push_back(line); 
    }
    return lines;
  }

  size_t getCrashedTrees(
      const pair<int,int> deltas, const vector<string> &lines) {
    size_t x = 0, y = 0;
    size_t trees = 0;
    while (y < lines.size()) {
      x = safeIndex(x + deltas.first, lines[y].size());
      y += deltas.second;
      if (lines[y][x] == '#') {
        // cout << "Crashed on " << x << ", " << y << endl;
        ++trees;
      }
    }
    return trees;
  }

  size_t solve1() {
    vector<string> lines = getLines();
    return getCrashedTrees(make_pair(3,1), lines);    
  }

  size_t solve2() {
    vector<string> lines = getLines();
    size_t result = 1, crashedTrees;
    vector<pair<int,int>> slopes = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
    for (const auto &p : slopes) {
      crashedTrees = getCrashedTrees(p, lines); 
      result *= crashedTrees;
      cout << "Crashed trees in Slope: ";  util::printPair(p); cout << crashedTrees << endl;
    }
    return result;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2021_ADVENTOFCODE_03_H_ */
