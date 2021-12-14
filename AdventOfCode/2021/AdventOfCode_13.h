/*
  Link:         http://adventofcode.com/2020/day/13
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/13/2020
*/

#ifndef _2021_ADVENTOFCODE_13_H_
#define _2021_ADVENTOFCODE_13_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <cstring>  // strcmp()
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

namespace aoc2021_13 {
using namespace std;

typedef pair<int,int> Coordinate;

void updateMinAndMax(Coordinate &min, Coordinate &max, const Coordinate &point) {
  // util::printPair(point, true);
  if (point.first < min.first) {
    min.first = point.first;
  }
  if (point.second < min.second) {
    min.second = point.second;
  }
  if (point.first > max.first) {
    max.first = point.first;
  }
  if (point.second > max.second) {
    max.second = point.second;
  }
}

void printCoordinates(
  const unordered_set<Coordinate, util::pair_hash> &coordinates,
  const Coordinate &min, const Coordinate &max) {
    cout << "Map: " << endl;
    for (int f = min.second; f <= max.second; ++f) { // Row (Y dimension);
      cout << f << ":\t";
      for (int c = min.first; c <= max.first; ++c) { // Column (X dimension)
        cout << (coordinates.contains({c,f}) ? '#' : '.');  // {c,f} == {x,y};
      }
      cout << endl;
    }
    cout << endl;
}

void printCoordinates(const unordered_set<Coordinate, util::pair_hash> &coordinates) {
  Coordinate min, max;
  for (const Coordinate c : coordinates) {
    updateMinAndMax(min, max, c);
  }
  printCoordinates(coordinates, min, max);
}

void processInput(
  unordered_set<Coordinate, util::pair_hash> &coordinates,
  vector<pair<char,int>> &instructions,
  Coordinate &min, Coordinate &max, int part) {
  cout << endl << "Min: ";
  util::printPair(min);
  cout << "\tMax: ";
  util::printPair(max, true);
  printCoordinates(coordinates, min, max);

  for (int i = 0; i < (part == 1 ? 1 : instructions.size()); ++i) {
    unordered_set<Coordinate, util::pair_hash> ci = coordinates;
    pair<char,int> &instruction = instructions[i];
    cout << "Folding in " << instruction.first << " axis, folding point: " << instruction.second << endl;
    if (instruction.first == 'x') {
      int x = instruction.second;
      for (const Coordinate &coord : ci) {
        if (coord.first < x) {
          continue;
        }
        int delta = coord.first - x;
        Coordinate foldedPosition(std::max(0, x - delta), coord.second);
        if (!coordinates.contains(foldedPosition)) {
          coordinates.insert(foldedPosition);
        }
        coordinates.erase(coord);
      }
    } else {
      int y = instruction.second;
      for (const Coordinate &coord : ci) {
        if (coord.second < y) {
          continue;
        }
        int delta = coord.second - y;
        Coordinate foldedPosition(coord.first, std::max(0, y - delta));
        if (!coordinates.contains(foldedPosition)) {
          coordinates.insert(foldedPosition);
        }
        coordinates.erase(coord);
      }
    }
    printCoordinates(coordinates);
  }
}

void solve(int part = 1) {
  string line;
  unordered_set<Coordinate, util::pair_hash> coordinates;
  vector<pair<char,int>> instructions;
  Coordinate min, max;
  while(!cin.eof()) {
    getline(cin,line);
    if (line.empty()) break;
    int i = 0;
    Coordinate coordinate(util::getNumberRef(line, i), util::getNumberRef(line, ++i));
    coordinates.insert(coordinate);
    updateMinAndMax(min, max, coordinate);
  }
  cout << endl;
  while(!cin.eof()) {
    getline(cin,line);
    instructions.emplace_back(line[11], util::getNumber(line, 13));
    util::printPair(instructions.back(),true);
  }
  processInput(coordinates, instructions, min, max, part);
  cout << "Part 1 - Total points remaining: " << coordinates.size() << endl;
  if (part == 2) {
    cout << "Part 2:";
    printCoordinates(coordinates);
  }
}

};  // aoc2021_13

#endif /* _2021_ADVENTOFCODE_13_H_ */
