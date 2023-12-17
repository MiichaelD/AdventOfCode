/*
  Link:         http://adventofcode.com/2023/day/16
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/16/2023
*/

#ifndef _2023_ADVENTOFCODE_16_H_
#define _2023_ADVENTOFCODE_16_H_

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

namespace aoc2023_16 {
using namespace std;

constexpr char kNorth = 'N';
constexpr char kSouth = 'S';
constexpr char kEast = 'E';
constexpr char kWest = 'W';

struct PosDirection {
  PosDirection(const pair<int,int>& pos, char d)
      : position(pos), direction(d) {}
  PosDirection(pair<int,int>&& pos, char d)
      : position(std::move(pos)), direction(d) {}
  pair<int, int> position;
  char direction;
};

class PuzzleInput {
 public:
  vector<string> map;
  pair<int, int> start_pos{0, 0};

  static PuzzleInput GetInput() {
    PuzzleInput input;
    while(!cin.eof()) {
      getline(cin, input.map.emplace_back());
    }
    return input;
  }

  void Print(
    const unordered_map<pair<int,int>, vector<char>, util::pair_hash>& visited_pos) const {
    for (int i = 0; i < map.size(); ++i) {
      for (int j = 0; j < map[i].size(); ++j) {
        if (visited_pos.find(std::make_pair(i, j)) != visited_pos.end()) {
          cout << "#";
        } else {
          cout << map[i][j];
        }
      }
      cout << endl;
    }
    cout << endl;
  }
};

// Updates the current_pos 1 step into the given direction.
void Advance(PosDirection& pos_dir) {
  switch(pos_dir.direction) {
    case kNorth: pos_dir.position.first -= 1; break;
    case kSouth: pos_dir.position.first += 1; break;
    case kWest: pos_dir.position.second -= 1; break;
    case kEast: pos_dir.position.second += 1; break;
  }
}

void Navigate(
    const PuzzleInput& input,
    unordered_map<pair<int,int>, vector<char>, util::pair_hash>& visited_pos,
    deque<PosDirection>& pos_dirs) {
  for (int i = 0; i < pos_dirs.size(); ++i) {
    cout << "Position " << i << " out of " << pos_dirs.size() << "\t";
    PosDirection& pos_dir = pos_dirs[i];
    util::printPair(pos_dir.position);  cout << endl;
    while(true) {
      if (pos_dir.position.first < 0 ||
          pos_dir.position.first >= input.map.size()
          || pos_dir.position.second < 0 ||
          pos_dir.position.second >= input.map.back().size()) {
        break;  // Out of bounds
      }
      bool visited = false;
      for (char c : visited_pos[pos_dir.position]) {
        if (c == pos_dir.direction) { visited = true; break; }
      }
      if (visited) { break; }
      visited_pos[pos_dir.position].push_back(pos_dir.direction);
      char pos_char = input.map[pos_dir.position.first][pos_dir.position.second];
      cout << "\tPosition: "; util::printPair(pos_dir.position); 
      cout << "=> Direction: " << pos_dir.direction << " found: " << pos_char;
      cout << " => Total visited: " << visited_pos.size() << endl;
      switch(pos_char) {
        case '|':  // North-South split
          if (pos_dir.direction == kEast || pos_dir.direction == kWest) {
            pos_dirs.emplace_back(
              std::make_pair(pos_dir.position.first - 1, pos_dir.position.second), kNorth);
              cout << "\t\tAdded: "; util::printPair(pos_dirs.back().position);
              cout << endl;
            pos_dir.direction = kSouth;
          }
          Advance(pos_dir);
          break;
        case '-':   // East-West split
          if (pos_dir.direction == kNorth || pos_dir.direction == kSouth) {
            pos_dirs.emplace_back(
              std::make_pair(pos_dir.position.first, pos_dir.position.second - 1), kWest);
              cout << "\t\tAdded: "; util::printPair(pos_dirs.back().position);
              cout << endl;
            pos_dir.direction = kEast;
          }
          Advance(pos_dir);
          break;
        case '\\':  // Mirror
          switch(pos_dir.direction) {
            case kNorth:  pos_dir.direction = kWest; break;
            case kWest: pos_dir.direction = kNorth; break;
            case kSouth: pos_dir.direction = kEast; break;
            case kEast: pos_dir.direction = kSouth; break;
          }
          Advance(pos_dir);
          break;
        case '/':  // Mirror
          // This could be replaced by flipping the direction and executing the switch from above.
          switch(pos_dir.direction) {
            case kNorth: pos_dir.direction = kEast;  break;
            case kWest:  pos_dir.direction = kSouth; break;
            case kSouth: pos_dir.direction = kWest;  break;
            case kEast:  pos_dir.direction = kNorth; break;
          }
          Advance(pos_dir);
          break;
        case '.':  // Keep going
        default:
          Advance(pos_dir);
          break;
      }
    }
  }
}

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  if (part == 1) {
    unordered_map<pair<int,int>, vector<char>, util::pair_hash> visited_pos;
    deque<PosDirection> pos_dirs;
    pos_dirs.emplace_back(input.start_pos, kEast);
    Navigate(input, visited_pos, pos_dirs);
    input.Print(visited_pos);
    cout << endl << "Visited positions: " << visited_pos.size() << endl;
  }
}

};  // aoc2023_16

#endif /* _2023_ADVENTOFCODE_16_H_ */
