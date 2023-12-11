/*
  Link:         http://adventofcode.com/2023/day/10
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/10/2023
*/

#ifndef _2023_ADVENTOFCODE_10_H_
#define _2023_ADVENTOFCODE_10_H_

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

namespace aoc2023_10 {

constexpr char kStartChar = 'S';
constexpr char kNorth = 'N';
constexpr char kSouth = 'S';
constexpr char kEast = 'E';
constexpr char kWest = 'W';

using namespace std;


struct PuzzleInput {
  vector<string> map;
  pair<int,int> start_pos;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    bool start_pos_found = false;
    while(!cin.eof()) {
      input.map.emplace_back();
      getline(cin, input.map.back());
      if (!start_pos_found) {
        for (int c = 0; c < input.map.back().size(); ++c) {
          if (input.map.back()[c] == kStartChar) {
            start_pos_found = true;
            input.start_pos = {input.map.size()-1, c};
          }
        }
      }
    }
    util::printVector(input.map, true);
    cout << "Starting position: ";
    util::printPair(input.start_pos);
    cout << endl;
    return input;
  }
};

pair<int,int> FindCharInMap(const PuzzleInput& input, char target) {
  for (int row = 0; row < input.map.size(); ++row) {
    for (int column = 0; column < input.map[row].size(); ++column) {
      if (input.map[row][column] == target) {
        return {row, column};
      }
    }
  }
  return {-1, -1};
}

void Advance(pair<int,int>& current_pos, char direction) {
  switch(direction) {
    case kNorth: current_pos.first -= 1; break;
    case kSouth: current_pos.first += 1; break;
    case kWest: current_pos.second -= 1; break;
    case kEast: current_pos.second += 1; break;
  }
}

size_t Navigate(
  const PuzzleInput& input,
  unordered_map<pair<int,int>, size_t, util::pair_hash>& visited_pos,
  char direction) {
  pair<int,int> current_pos = input.start_pos;
  visited_pos[current_pos] = 0;
  Advance(current_pos, direction);
  size_t longest = 0;
  size_t steps = 1;
  while(!longest) {
    visited_pos[current_pos] = steps;
    char pos_char = input.map[current_pos.first][current_pos.second];
    // cout << "\t Direction: " << direction << " found: " << pos_char;
    // cout << ". At step: " << steps << endl;
    ++steps;
    switch(pos_char) {
      case '|':  // North-South
        current_pos.first += direction == kNorth ? -1 : 1;
        break;
      case '-':   // East-West
        current_pos.second += direction == kWest ? -1 : 1;
        break;
      case 'L':  // North-East
        if (direction == kSouth) {
          direction = kEast;
          current_pos.second += 1;
        } else {
          direction = kNorth;
          current_pos.first -= 1;
        }
        break;
      case 'J':  // North-West
        if (direction == kSouth) {
          direction = kWest;
          current_pos.second -= 1;
        } else {
          direction = kNorth;
          current_pos.first -= 1;
        }
        break;
      case '7':  // South-West
        if (direction == kNorth) {
          direction = kWest;
          current_pos.second -= 1;
        } else {
          direction = kSouth;
          current_pos.first += 1;
        }
        break;
      case 'F':  // South-East
        if (direction == kNorth) {
          direction = kEast;
          current_pos.second += 1;
        } else {
          direction = kSouth;
          current_pos.first += 1;
        }
        break;
      case 'S':
      default: 
        return longest;
    }
    auto it = visited_pos.find(current_pos);
    if (it != visited_pos.end() && it->second <= steps) {
      cout << "\tNext step found before: " << it->second << endl;
      longest = steps;
    }
  }
  return longest;
}

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  vector<char> possible_directions{kNorth, kWest, kSouth, kEast};
  unordered_map<pair<int,int>, size_t, util::pair_hash> visited_pos;
  for (char dir: possible_directions) {
    auto res = Navigate(input, visited_pos, dir);
    cout << "Going " << dir << ". Longest position at: " << res << " steps" << endl;
  }
}

};  // aoc2023_10

#endif /* _2023_ADVENTOFCODE_10_H_ */
