/*
  Link:         http://adventofcode.com/2020/day/5
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/05/2019
*/

#ifndef _2021_ADVENTOFCODE_05_H_
#define _2021_ADVENTOFCODE_05_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <climits>  // INT_MAX and such numeric limits
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/util.h"

namespace aoc2021_05 {  
using namespace std;

typedef pair<int,int> Point;
typedef pair<Point,Point>  Line;

Point getPoint(const string &input) {
  pair<int,int> result;
  int index = 0;
  result.first = util::getNumberRef(input, index);
  result.second = util::getNumber(input, index + 1);
  return result;
}

inline bool IsHorizontal(const Line &line) {
  return (line.first.second == line.second.second);
}

inline bool IsVertical(const Line &line) {
  return (line.first.first == line.second.first);
}

inline bool IsDiagonal(const Line &line) {
  return !IsHorizontal(line) && !IsVertical(line);
}

void solve(int part = 1) {
  string point1, point2, aux;
  vector<Line> lines;
  unordered_map<Point, int, util::pair_hash> overlapPerPoint;
  while (!cin.eof()) {
    cin >> point1 >> aux >> point2;
    Line line;
    line.first = getPoint(point1);
    line.second = getPoint(point2);
    lines.push_back(line);
    cout << "Line: "; util::printPair(line.first); cout << ", "; util::printPair(line.second);
    if (IsHorizontal(line)) {
      cout << " Horizontal:" << endl;
      Point *a, *b;
      if (line.first.first < line.second.first) {
        a = &line.first;
        b = &line.second;
      } else {
        a = &line.second;
        b = &line.first;
      }
      for (int i = a->first; i <= b->first ; ++i) {
        cout << "\tVisited point: " << i << ", " << a->second << " : " <<
             ++overlapPerPoint[{i, a->second}];
        cout << " times" << endl;
      }
    } else if (IsVertical(line)) {
      cout << " Vertical:" << endl;
      Point *a, *b;
      if (line.first.second < line.second.second) {
        a = &line.first;
        b = &line.second;
      } else {
        a = &line.second;
        b = &line.first;
      }
      for (int i = a->second; i <= b->second ; ++i) {
        cout << "\tVisited point: " << a->first << ", " << i << " : " <<
            ++overlapPerPoint[{a->first, i}];
        cout << " times" << endl;
      }
    } else {
      cout << "Skipped." << endl;
    }
  }
  int totalPointsWithOverlap = 0;
  for (const auto &entry : overlapPerPoint) {
    if (entry.second > 1) {
      ++totalPointsWithOverlap;
    }
  }
  cout << "Total Points w/ Overlap: " << totalPointsWithOverlap << endl;
}

};

#endif /* _2021_ADVENTOFCODE_05_H_ */