/*
  Link:         http://adventofcode.com/2019/day/20
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/20/2019
*/

#ifndef _2019_ADVENTOFCODE_20_H_
#define _2019_ADVENTOFCODE_20_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc2019_20 {  
  using namespace std;

  typedef unordered_map<string, vector<pair<int,int>>> PORTAL_MAP;
  typedef vector<string> MAP;

  const char WALL1 = '#';
  const char WALL2 = ' ';

  template<class T>
  inline void printPair(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "]";
  }

  void printPortals(const PORTAL_MAP& portals) {
    for (const auto &entry : portals) {
      cout << "Key: " << entry.first << " has following positions: ";
      for (const auto &p : entry.second) {
        printPair(p); cout << ", ";
      }
      cout << endl;
    }
  }

  void printMap(const MAP &map) {
    for (const string &line : map) {
      for (char c : line) {
        // if (c == WALL2) c = WALL1;
        cout << c;
      }
      cout << endl;
    }
    cout << endl;
  }

  void findPortals(const MAP &map, PORTAL_MAP &portals) {
    for (int y = 0; y < map.size(); ++y) {
      for (int x = 0; x < map[y].size(); ++x) {
        char c = map[y][x];
        // Skip non alpha chars, and end-of-strings (bottom or right chars)
        if (!isalpha(c) || isalpha(map[y][x-1]) || isalpha(map[y-1][x])) continue;
        if (isalpha(map[y][x+1])) {
          string key(2,c);  key[1] = map[y][x+1];
          portals[key].push_back(make_pair(x+1, y)); 
        } else if (isalpha(map[y+1][x])) {
          string key(2,c);  key[1] =  map[y+1][x];
          portals[key].push_back(make_pair(x, y+1)); 
        }
      }
    }
  }

  void solve(int part = 1) {
    string input;
    MAP map;
    PORTAL_MAP portals;
    while (!cin.eof()) {
      getline(cin, input);
      map.push_back(input);
    }
    printMap(map);
    findPortals(map, portals);
    printPortals(portals);
  }
};

#endif /* _2019_ADVENTOFCODE_20_H_ */