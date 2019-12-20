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

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1,T2> const &p) const {
      std::size_t h1 = std::hash<T1>()(p.first);
      std::size_t h2 = std::hash<T2>()(p.second);
      return h1 ^ h2;
    }
  };

  typedef unordered_map<string, vector<pair<int,int>>> KEY_TO_PORTALS;
  typedef unordered_map<pair<int,int>, string, pair_hash> POINT_TO_PORTAL;
  typedef vector<string> MAP;

  const char WALL1 = '#';
  const char WALL2 = ' ';
  const char PATH = '.';

  template<class T>
  inline void printPair(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "]";
  }

  void printPortals(const KEY_TO_PORTALS& portals) {
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

  void fillPortals(const MAP &map, KEY_TO_PORTALS &portals, POINT_TO_PORTAL &pointToPortals) {
    int aux = 0;
    string key = "12";
    pair<int,int> pos;
    for (int y = 0; y < map.size(); ++y) {
      for (int x = 0; x < map[y].size(); ++x) {
        char c = map[y][x];
        // Skip non alpha chars, and end-of-strings (bottom or right chars)
        if (!isalpha(c) || isalpha(map[y][x-1]) || isalpha(map[y-1][x])) continue;
        if (isalpha(map[y][x+1])) {
          key[0] = c;  key[1] = map[y][x+1];
          aux = x+2 < map[y].size() && map[y][x+2] == PATH ? x+1 : x;
          pos = make_pair(aux, y);
        } else if (isalpha(map[y+1][x])) {
          key[0] = c;  key[1] =  map[y+1][x];
          aux = y+2 < map.size() && map[y+2][x] == PATH ? y+1 : y;
          pos = make_pair(x, aux);
        }
        portals[key].push_back(pos); 
        pointToPortals[pos] = key;
      }
    }
  }

  // pair<int,int> getPosNextToPortal()

  void solve(int part = 1) {
    string input;
    MAP map;
    KEY_TO_PORTALS portals;
    POINT_TO_PORTAL pointToPortals;
    while (!cin.eof()) {
      getline(cin, input);
      map.push_back(input);
    }
    printMap(map);
    fillPortals(map, portals, pointToPortals);
    printPortals(portals);
  }
};

#endif /* _2019_ADVENTOFCODE_20_H_ */