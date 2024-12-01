/*
  Link:         http://adventofcode.com/2023/day/17
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/17/2023
*/

#ifndef _2023_ADVENTOFCODE_17_H_
#define _2023_ADVENTOFCODE_17_H_

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
#include <queue>

namespace aoc2023_17 {
using namespace std;

constexpr size_t kMaxConsecutiveSteps = 3;
constexpr char kNorth = 'N';
constexpr char kSouth = 'S';
constexpr char kEast = 'E';
constexpr char kWest = 'W';
const vector<char> kPosibleDirs{kEast, kSouth, kWest, kNorth};

struct PosDirection {
  pair<int, int> position;
  char direction;
  int rolling_value = 0;
  int consecutive = 0;
  PosDirection() = default;
  PosDirection(const PosDirection& pos_dir)
      : position(pos_dir.position), direction(pos_dir.direction),
      rolling_value(pos_dir.rolling_value), consecutive(pos_dir.consecutive) {}
  PosDirection(const pair<int,int>& pos, char d)
      : position(pos), direction(d) {}
  PosDirection(pair<int,int>&& pos, char d)
      : position(std::move(pos)), direction(d) {}

  void print() const {
    util::printPair(position);
    cout << ". Direction: " << direction << ". Rolling Value: " << rolling_value;
    cout << ". Consecutive steps: " << consecutive << endl;
  }

  // bool operator=(const PosDirection& pos_dir) const {
  //   return pos_dir.position.first == position.first
  //       && pos_dir.position.second == position.second
  //       && pos_dir.consecutive == consecutive
  //       && pos_dir.direction == direction;
  // }
  
  bool operator<(const PosDirection& pos_dir) const {
    return rolling_value > pos_dir.rolling_value;
  }
};

struct pos_dir_hash {
  std::size_t operator()(const PosDirection& pd) const {
    std::size_t h1 = std::hash<int>()(pd.position.first);
    std::size_t h2 = std::hash<int>()(pd.position.second);
    std::size_t h3 = std::hash<int>()(pd.consecutive);
    std::size_t h4 = std::hash<char>()(pd.direction);
    // std::size_t h5 = std::hash<int>()(pd.rolling_value);
    return h1 ^ h2 ^ h3 ^ h4 /*h5 ^*/ ;
  }
};

typedef unordered_map<pair<int, int>, vector<PosDirection>, util::pair_hash> VisitedPos;

// Updates the current_pos 1 step into the given direction.
inline void Advance(PosDirection& pos_dir) {
  switch(pos_dir.direction) {
    case kNorth: pos_dir.position.first -= 1; break;
    case kSouth: pos_dir.position.first += 1; break;
    case kWest: pos_dir.position.second -= 1; break;
    case kEast: pos_dir.position.second += 1; break;
  }
}

bool IsOppositeDir(char dir1, char dir2) {
  if (dir1 == kNorth && dir2 == kSouth) return true;
  if (dir1 == kSouth && dir2 == kNorth) return true;
  if (dir1 == kWest && dir2 == kEast) return true;
  if (dir1 == kEast && dir2 == kWest) return true;
  return false;
}

void AddPossibleDirections(
    const vector<vector<int>>& map, const PosDirection& current, 
    priority_queue<PosDirection>& pos_dirs) {
  for (char dir : kPosibleDirs) {
    if (dir == current.direction && current.consecutive >= kMaxConsecutiveSteps) {
      cout << "\tSkipping due consecutive direction: " << dir << endl;
      continue;
    }
    if (IsOppositeDir(dir, current.direction)) {
      cout << "\tSkipping due opposite direction: " << dir << endl;
      continue;
    }
    PosDirection new_pos = current;
    new_pos.direction = dir;
    Advance(new_pos);
    if (new_pos.position.first < 0 || new_pos.position.second < 0 ||
        new_pos.position.first >= map.size() ||
        new_pos.position.second >= map.back().size()) {
      cout << "\tSkipping due Out of Bounds: " << dir << endl;
      continue;
    }
    new_pos.rolling_value += map[new_pos.position.first][new_pos.position.second];
    if (dir == current.direction) {
      ++new_pos.consecutive;
    } else {
      new_pos.consecutive = 1;
    }
    cout << "\tAdding: "; new_pos.print();
    pos_dirs.emplace(new_pos);
  }
}

class PuzzleInput {
  vector<vector<int>> map;
  pair<int, int> start_pos{0, 0};
  pair<int, int> target_pos;

  void FindOptimalPath(
      priority_queue<PosDirection>& pos_dirs, VisitedPos& visited_pos, 
      deque<PosDirection>& rolling_path, deque<deque<PosDirection>>& solutions) const {
    int end_pos_val = INT_MAX;
    for (int i = 0; pos_dirs.size(); ++ i){
      auto pos_dir = pos_dirs.top();
      pos_dirs.pop();
      cout << "Exploring: "; util::printPair(pos_dir.position);
      cout << ". Pending: " << pos_dirs.size();
      cout << ". RollValue: " << pos_dir.rolling_value;
      cout << ". MapVal: " << map[pos_dir.position.first][pos_dir.position.second] << endl;
      auto it = visited_pos.find(pos_dir.position);
      // if (it != visited_pos.end() && it->second.back().rolling_value < pos_dir.rolling_value) {
      if (it != visited_pos.end() && it->second.size() > 3) {
        // We have seen this position and with a lower heat lost.
        continue;
      }
      // Traverse from this point.
      cout << "Marked as visited with new value: " << pos_dir.rolling_value << endl;
      visited_pos[pos_dir.position].push_back(pos_dir);
      rolling_path.push_back(pos_dir);
      if (pos_dir.position == target_pos &&
          pos_dir.rolling_value <= end_pos_val) {  // We found a better path.
        cout << "Found a path with value: " << pos_dir.rolling_value << endl;
        solutions.push_back(rolling_path);
        end_pos_val = pos_dir.rolling_value;
        rolling_path.pop_back();  // Remove last added point.
        break;
      }
      AddPossibleDirections(map, pos_dir, pos_dirs);
      rolling_path.pop_back();  // Remove last added point.
    }
    cout << "end_pos_val: " << end_pos_val << endl;
  }

 public:
  static PuzzleInput GetInput(int part) {
    PuzzleInput input;
    std::string aux;
    while(!cin.eof()) {
      getline(cin, aux);
      input.map.emplace_back();
      for (char c : aux) {
        input.map.back().emplace_back(c - '0');
      }
    }
    input.target_pos =
        std::make_pair(input.map.size() -1, input.map.back().size() -1);
    return input;
  }

  void Print() const {
    for (const auto& line : map) {
      util::printVector(line);
      cout << endl;
    }
  }

  // vector<PosDirection> GetVisitedPosByPos(
  //     const VisitedPos& visited_pos, const pair<int,int>& pos) const {
  //   vector<PosDirection> solutions;
  //   PosDirection pos_dir(pos, kEast);
  //   for (int i = 1 ; i <= kMaxConsecutiveSteps; ++i) {
  //     pos_dir.consecutive = i;
  //     for (char dir : kPosibleDirs) {
  //       pos_dir.direction = dir;
  //       auto it = visited_pos.find(pos_dir);
  //       if (it != visited_pos.end()) {
  //         cout << "Found a solution with rolling value of: " << it->first.rolling_value << endl;
  //         solutions.push_back(it->first);
  //       }
  //     }
  //   }
  //   return solutions;
  // }

  void Print(const VisitedPos& visited_pos) const {
    for (int f = 0; f < map.size(); ++f) {
      for (int c = 0; c < map.back().size(); ++c) {
        auto it = visited_pos.find(make_pair(f, c)) ;
        cout << (it != visited_pos.end() ? it->second.back().rolling_value : 0) << ", ";
      }
      cout << endl;
    }
  }

  int MinHeatLossPath() const {
    deque<PosDirection> path;
    VisitedPos visited_pos;
    deque<deque<PosDirection>> solutions;
    priority_queue<PosDirection> pos_dirs;  
    pos_dirs.push({start_pos, 'E'});
    FindOptimalPath(pos_dirs, visited_pos, path, solutions);
    for (const auto& solution : solutions) {
      cout << "Solution: ";
      for (const auto& pos_dir : solution) {
        pos_dir.print();
      }
      cout << endl;
    }
    Print(visited_pos);
    auto it = visited_pos.find(target_pos) ;
    cout << "Solutions found:";
    for (const auto& item : it->second) {
      item.print();
    }
    return (it != visited_pos.end() ? it->second.back().rolling_value : 0);
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput(part);
  input.Print();
  auto heat_loss = input.MinHeatLossPath();
  cout << endl << "Route with min heat lost of: " << heat_loss << endl;
}

};  // aoc2023_17

#endif /* _2023_ADVENTOFCODE_17_H_ */
