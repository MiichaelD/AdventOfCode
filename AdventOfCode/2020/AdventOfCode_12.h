/*
  Link:         http://adventofcode.com/2020/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/12/2020
*/

#ifndef _2020_ADVENTOFCODE_12_H_
#define _2020_ADVENTOFCODE_12_H_

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

namespace aoc2020_12 {
using namespace std;

typedef long long int lli;

constexpr char F = 'F';
constexpr char N = 'N';
constexpr char S = 'S';
constexpr char E = 'E';
constexpr char W = 'W';
constexpr char L = 'L';
constexpr char R = 'R';

class Navigator {
private:
  bool rotateLeft(int val=90) {
    size_t times = val / 90;
    cout << "Rotating left " << times << " times, now facing: '";  
    for (size_t i = 0 ; i < times; ++i) {
      switch(direction) {
        case E: direction = N; break;
        case N: direction = L; break;
        case L: direction = S; break;
        case S: direction = E; break;
        default: cout << "This shouldn't happen." << endl; return false;
      }
    }
    cout << direction << "'\t";
    return true;
  }
  
  bool rotateRight(int val=90) {
    size_t times = val / 90;
    cout << "Rotating right " << times << " times, now facing: '";
    for (size_t i = 0 ; i < times; ++i) {
      switch(direction) {
        case E: direction = S; break;
        case S: direction = L; break;
        case L: direction = N; break;
        case N: direction = E; break;
        default: cout << "This shouldn't happen." << endl; return false;
      }
    }
    cout << direction << "'\t";
    return true;
  }

  bool forward(int val) {
    switch(direction) {
      case E: cout << "Moving E \t"; coordinates.first += val; break;
      case N: cout << "Moving N \t"; coordinates.second -= val; break;
      case L: cout << "Moving L \t"; coordinates.first -= val; break;
      case S: cout << "Moving S \t"; coordinates.second += val; break;
      default: cout << "This shouldn't happen." << endl; return false;
    }
    return true;
  }

  vector<string> getCommands() {
    string line;
    vector<string> lines;
    while(!cin.eof()) {
      cin >> line;
      lines.push_back(line); 
    }
    return lines;
  }

  bool rotateWaypointLeft(int val=90) {
    size_t times = val / 90;
    cout << "Rotating left " << times << " times, now facing: '";  
    for (size_t i = 0 ; i < times; ++i) {
      lli aux = waypoint.first;
      waypoint.first = waypoint.second;
      waypoint.second = -aux;
    }
    cout << direction << "'\t";
    return true;
  }
  
  bool rotateWaypointRight(int val=90) {
    size_t times = val / 90;
    cout << "Rotating right " << times << " times, now facing: '";
    for (size_t i = 0 ; i < times; ++i) {
      lli aux = waypoint.first;
      waypoint.first = -waypoint.second;
      waypoint.second = aux;
    }
    cout << direction << "'\t";
    return true;
  }

  bool forwardWaypoint(int val) {
    coordinates.first += waypoint.first * val;
    coordinates.second += waypoint.second * val;
    return true;
  }

  pair<lli,lli> coordinates;
  char direction;
  pair<lli,lli> waypoint;
  bool hasWaypoint;

public:
  Navigator() : coordinates{make_pair(0, 0)}, direction{E}, hasWaypoint{false} {}

  void setWaypoint(const pair<lli,lli> *coords) {
    if (coords == nullptr) {
      hasWaypoint = false;
      return;
    }
    waypoint = *coords;
    hasWaypoint = true;
  }

  bool processInput(char cmd, int val) {
    bool result = true;
    cout << "Processing: '" << cmd << "'' x '" << val << "'\t";
    pair<lli,lli> &coords = hasWaypoint ? waypoint : coordinates;
    switch(cmd) {
      case F: result = hasWaypoint ? forwardWaypoint(val) : forward(val); break;
      case N: coords.second -= val; break;
      case S: coords.second += val; break;
      case E: coords.first += val; break;
      case W: coords.first -= val; break;
      case L: result = hasWaypoint ? rotateWaypointLeft(val) : rotateLeft(val); break;
      case R: result = hasWaypoint ? rotateWaypointRight(val) : rotateRight(val); break;
      default: cout << "This shouldn't happen." << endl;  return false;
    }
    if (hasWaypoint) {
      util::printPair(coordinates);
      cout << ". Waypoint: ";
      util::printPair(waypoint, true);
    } else {
      util::printPair(coordinates, true); 
    }
    return result;
  }

  bool processInput() {
    vector<string> lines = getCommands();
    for (const string &str : lines) {
      char cmd = str[0];
      int val = atoi(&str.c_str()[1]);
      if (!processInput(cmd, val)) {
        cout << "ERROR" << endl; return false; 
      }
    }
    cout << "Navigator's Manhattan Distance: ";
    util::printPair(coordinates);
    cout << ". Added: " << (abs(coordinates.first) + abs(coordinates.second)) << endl;
  }

  pair<lli,lli> getManhattanDistance() const {
    return coordinates;
  }
};

void solve(int part = 1) {
  Navigator navigator;
  if (part == 1) {
    navigator.processInput();
  } else {
    pair<lli,lli> waypoint(10, -1);
    navigator.setWaypoint(&waypoint);
    navigator.processInput();
  }
}

};  // aoc2020_12

#endif /* _2020_ADVENTOFCODE_12_H_ */
