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

  pair<int,int> getPathPosNextToPortal(const MAP &map, const pair<int,int> &portalPos) {
    int x = portalPos.first, y = portalPos.second;
    if (y + 1 < map.size() && map[y + 1][x] == PATH) return make_pair(x, y + 1);
    if (y - 1 >= 0 && map[y - 1][x] == PATH) return make_pair(x, y - 1);
    if (x + 1 < map[y].size() && map[y][x + 1] == PATH) return make_pair(x + 1, y);
    if (x - 1 >= 0 && map[y][x - 1] == PATH) return make_pair(x - 1, y);
    cout << "getPathPosNextToPortal() - This shouldnt happen! " << endl;
    return portalPos;
  }

  vector<pair<int,int>> getNeighbors(const pair<int,int> &pos) {
    int x = pos.first, y = pos.second;
    return {{x, y}, {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
  }


  bool isValidPos(const MAP &map, const pair<int,int> &pos) { 
    return map[pos.second][pos.first] == PATH;
  }

  size_t findMinSteps(const MAP &map, const pair<int,int> &sPos, const pair<int,int> &ePos) {
    unordered_map<pair<int,int>, int, pair_hash> visited;
    deque<pair<pair<int,int>, size_t>> stack;
    visited[sPos] = 0;
    stack.push_back(make_pair(sPos, 0));
    size_t maxSteps = 0;
    while(stack.size()) {
      auto item = stack.front();
      stack.pop_front();
      // cout << "Checking pos: "; printPair(item.first);
      // cout << "\t has a distance of: " << item.second << endl;
      maxSteps = max(maxSteps, item.second);
      if (item.first == ePos) {
        cout << "Steps to target: " << item.second << endl;
        break; // found it;
      }
      auto neighbors = getNeighbors(item.first);
      for (const pair<int,int> &neighPos : neighbors) {
        const auto &entry = visited.find(neighPos);
        if (isValidPos(map, neighPos) && entry == visited.end()) { // Valid position and not visited
          int step = item.second + 1;
          visited[neighPos] = step;
          stack.push_back(make_pair(neighPos, step));

        }
      }
    }
    return maxSteps;
  }

  void test(const MAP &map, const KEY_TO_PORTALS &portals, const POINT_TO_PORTAL& pointToPortals) {
  }

  void solve(int part = 1) {
    string input, startKey = "AA", endKey = "ZZ";
    MAP map;
    while (!cin.eof()) {
      getline(cin, input);
      map.push_back(input);
    }

    KEY_TO_PORTALS portals;
    POINT_TO_PORTAL pointToPortals;
    fillPortals(map, portals, pointToPortals);
    printMap(map);
    printPortals(portals);

    pair<int,int> startPos = getPathPosNextToPortal(map, portals.at(startKey)[0]);
    pair<int,int> endPos = getPathPosNextToPortal(map, portals.at(endKey)[0]);
    cout << endl << "Start Position :"; printPair(startPos);
    cout << ", End Position :"; printPair(endPos); cout << endl << endl;
    test(map, portals, pointToPortals);

    cout << "Min distance: " << findMinSteps(map, startPos, endPos) << endl;
  }
};


  // From day 15 part 2: 
  // unordered_map<pair<int,int>, int, pair_hash> getSteps(
  //     const pair<int,int> &origin, const pair<int,int> &target) {
  //   unordered_map<pair<int,int>, int, pair_hash> visited;
  //   deque<pair<pair<int,int>, int>> stack;
    // visited[origin] = 0;
    // stack.push_back(make_pair(origin, 0));
    // int maxSteps = 0;
    // while(stack.size()) {
    //   auto item = stack.front();
    //   stack.pop_front();
    //   // cout << "Checking pos: "; printPair(item.first);
    //   // cout << "\t has a distance of: " << item.second << endl;
    //   maxSteps = max(maxSteps, item.second);
    //   if (item.first == target) {
    //     cout << "Steps to target: " << item.second << endl;
    //     break; // found it;
    //   }
    //   auto neighbors = getNeighbors(item.first);
    //   for (const pair<int,int> &neighPos : neighbors) {
    //     const auto &entry = visited.find(neighPos);
    //     if (isPosValid(neighPos) && entry == visited.end()) { // Valid position and not visited
    //       int step = item.second + 1;
    //       visited[neighPos] = step;
    //       stack.push_back(make_pair(neighPos, step));

    //     }
    //   }
    // }    
    // cout << "Max Steps: " << maxSteps << endl;
  //   return visited;
  // }

#endif /* _2019_ADVENTOFCODE_20_H_ */