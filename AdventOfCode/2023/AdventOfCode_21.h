/*
  Link:         http://adventofcode.com/2023/day/21
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/21/2023
*/

#ifndef _2023_ADVENTOFCODE_21_H_
#define _2023_ADVENTOFCODE_21_H_

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

namespace aoc2023_21 {

using namespace std;

constexpr int kMaxStepsTest = 6;
constexpr int kMaxSteps = 64;
constexpr char kStart = 'S';
constexpr char kRock = '#';
constexpr char kNorth = 'N';
constexpr char kSouth = 'S';
constexpr char kEast = 'E';
constexpr char kWest = 'W';
const vector<char> kPossibleDirs{kNorth, kEast, kSouth, kWest};

typedef unordered_set<pair<int,int>, util::pair_hash> CoordinatesSet;


// Updates the current_pos 1 step into the given direction.
void Advance(char direction, pair<int,int>& pos_dir) {
  switch(direction) {
    case kNorth: pos_dir.first -= 1; break;
    case kSouth: pos_dir.first += 1; break;
    case kWest: pos_dir.second -= 1; break;
    case kEast: pos_dir.second += 1; break;
  }
}

class PuzzleInput {
  vector<string> map;
  vector<CoordinatesSet> coordinates;
  pair<int, int> start_pos;

  void PrintMapWithCoordinates(const CoordinatesSet& coors) {
    for (int f = 0; f < map.size(); ++f) {
      for (int c = 0; c < map[f].size(); ++c) {
        cout << (coors.find({f, c}) != coors.end() ? 'O' : map[f][c]);
      }
      cout << endl;
    }
    cout << endl;
  }

  bool IsOutOfBounds(const pair<int,int> coord) {
    return (coord.first < 0 || coord.second <0 || coord.first >= map.size()
            || coord.second >= map.back().size());
  }

  void NormalizeCoord(pair<int,int>& coord) {
    if(coord.first < 0) {
      int times = (-1 * coord.first) / map.size();
      coord.first += map.size() * times;
    }
    if (coord.second <0) {
      int times = (-1 * coord.second) / map.back().size();
      coord.second += map.size() * times;
    }
    if (coord.first >= map.size()) {
      int times = coord.first / map.size();
      coord.first -= map.size() * times;
    }
    if (coord.second >= map.back().size()) {
      int times = coord.second / map.back().size();
      coord.second -= map.size() * times;
    }
  }

public: 
  static PuzzleInput GetInput(int part) {
    PuzzleInput input;
    bool found_start = false;
    int line = 0;
    while(!cin.eof()) {
      string& last_line = input.map.emplace_back();
      getline(cin, last_line);
      for (int i = 0 ; !found_start && i < last_line.size(); ++i) {
        if (last_line[i] == kStart) {
          input.start_pos = std::make_pair(line, i);
          break;
        }
      }
      ++line;
    }
    input.coordinates.emplace_back().insert(input.start_pos);
    return input;
  }

  void PrintCoordinates(int index = -1) {
    if (index < 0) {
      for (index = 0; index < coordinates.size(); ++index) {
        PrintCoordinates(index);
      }
      return;
    }
    cout << "At " << index << " steps." << endl;
    auto& coordinate_set = coordinates.at(index);
    PrintMapWithCoordinates(coordinate_set);
    // for (const auto& entry : coordinate_set) {
    //   cout << "\t"; util::printPair(entry, true);
    // }
  }

  void GenerateNextCoordinates(int part = 1) {
    auto &new_coors = coordinates.emplace_back();
    for (const auto& entry : coordinates[coordinates.size() - 2]) {
      // cout << "For coordinate: "; util::printPair(entry, true);
      for (char dir : kPossibleDirs) {
        auto new_pos = entry;
        Advance(dir, new_pos);
        // cout << "\tTrying: "; util::printPair(new_pos);
        // cout << ", has value: " << map[new_pos.first][new_pos.second] << endl;
        if (IsOutOfBounds(new_pos)) {
          if (part == 1) {
            continue;
          }
          NormalizeCoord(new_pos);
        }
        NormalizeCoord(new_pos);
        if (map[new_pos.first][new_pos.second] != kRock
            && new_coors.find(new_pos) == new_coors.end()) {
          new_coors.insert(new_pos);
        }
      }
    }
  }

  int GetPossibleSteps(int index) {
    return coordinates[index].size();
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput(part);
  for (int steps = 1; steps <= kMaxSteps; ++steps) {
    cout << "Step: " << steps << endl;
    input.GenerateNextCoordinates(part);
  }
  input.PrintCoordinates(kMaxSteps);
  auto solution = input.GetPossibleSteps(kMaxSteps);
  cout << "At step " << kMaxSteps << " we have " << solution << " possible steps" << endl;
}

};  // aoc2023_21

#endif /* _2023_ADVENTOFCODE_21_H_ */
