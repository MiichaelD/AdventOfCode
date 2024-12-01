/*
  Link:         http://adventofcode.com/2023/day/18
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/18/2023
*/

#ifndef _2023_ADVENTOFCODE_18_H_
#define _2023_ADVENTOFCODE_18_H_

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

namespace aoc2023_18 {
using namespace std;

constexpr char kNorth = 'U';
constexpr char kSouth = 'D';
constexpr char kEast = 'R';
constexpr char kWest = 'L';
const vector<char> kPosibleDirs{kNorth, kEast, kSouth, kWest};


void Advance(pair<int,int>& pos, char dir) {
  switch(dir) {
    case kNorth: pos.first -= 1; break;
    case kSouth: pos.first += 1; break;
    case kWest: pos.second -= 1; break;
    case kEast: pos.second += 1; break;
  }
}


void PrintMap(const unordered_set<pair<int,int>, util::pair_hash>& edges) {
    int min_col = INT_MAX, min_row = INT_MAX;
    int max_col = 0, max_row = 0;
    for (const auto& point : edges) {
      max_row = std::max(point.first, max_row);
      min_row = std::min(point.first, min_row);
      min_col = std::min(point.second, min_col);
      max_col = std::max(point.second, max_col);
    }
    for (int i = min_row ; i <= max_row; ++i) {
      for (int j = min_col; j <= max_col; ++j) {
        cout << (edges.find({i, j}) != edges.end() ? "#" : ".");
      }
      cout << endl;
    }
    cout << endl;
  }

struct DirLen {
  DirLen(char d, int l) : dir(d), len(l) {}
  char dir;
  int len;
  void Print() const {
    cout << "[" << dir << ", " << len << "]" << endl;
  }
};

struct PuzzleInput {
  vector<DirLen> trench;
  unordered_set<pair<int,int>, util::pair_hash> edges;
  unordered_set<pair<int,int>, util::pair_hash> filled;
  int min_col = INT_MAX, min_row = INT_MAX;
  int max_col = 0, max_row = 0;

  void InitMapEdges() {
    for (const auto& point : edges) {
      max_row = std::max(point.first, max_row);
      min_row = std::min(point.first, min_row);
      min_col = std::min(point.second, min_col);
      max_col = std::max(point.second, max_col);
    }
  }

  void PrintEdges() {
    for (int i = min_row ; i <= max_row; ++i) {
      for (int j = min_col; j <= max_col; ++j) {
        cout << (edges.find({i, j}) != edges.end() ? "#" : ".");
      }
      cout << endl;
    }
    cout << endl;
  }

  void Print() const {
    for (const auto& entry : trench) {
      entry.Print();
    }
  }

  void InitEdges() {
    pair<int,int> aux{0, 0};
    edges.insert(aux);
    for (const auto& entry : trench) {
      for (int i = 0; i < entry.len; ++i) {
        Advance(aux, entry.dir);
        edges.insert(aux);
      }
    }
    InitMapEdges();
    PrintEdges();
  }

  pair<int,int> FindCoordinateToFlood() {
    // find line with 2 # that aren't adjacent.
    for (int i = min_row ; i <= max_row; ++i) {
      vector<pair<int,int>> indeces;
      for (int j = min_col; j <= max_col; ++j) {
        const bool is_edge = (edges.find({i, j}) != edges.end());
        if (is_edge) {
          indeces.emplace_back(i, j);
        }
      }
      if (indeces.size() == 2) {
        if (++indeces[0].second < indeces[1].second) {
          cout << "Flooding: "; util::printPair(indeces[0], true);
          return indeces[0];
        }
      }
    }
    return {0, 0};
  }

  size_t FillPool() {
    filled = edges;
    deque<pair<int,int>> coords_to_fill{FindCoordinateToFlood()};
    for (;coords_to_fill.size();) {
      const auto coord = coords_to_fill.front();
      coords_to_fill.pop_front();
      if (filled.find(coord) != filled.end()) {
        continue;
      }
      filled.insert(coord);
      for (char dir : kPosibleDirs) {
        auto temp_coord = coord;
        Advance(temp_coord, dir);
        if (filled.find(temp_coord) != filled.end()) {
          continue;  // Out of bounds or already seen
        }
        // cout << "\tAdding: "; util::printPair(temp_coord, true);
        coords_to_fill.push_back(temp_coord);
      }
    }
    PrintMap(filled);
    return filled.size();
  }

  size_t GetPoolSize(){
    size_t size = 0;
    unordered_set<pair<int,int>, util::pair_hash> filled;
    for (int i = min_row ; i <= max_row; ++i) {
      bool within = false;
      bool last = false;
      for (int j = min_col; j <= max_col; ++j) {
        const bool is_edge = (edges.find({i, j}) != edges.end());
        if (is_edge) {
          bool through_many_edges = false;
          int e = 1;
          for (; e < max_col; ++e) {
            if (edges.find({i, j + e}) == edges.end()) break;
            filled.insert({i, j + e});
            ++size;
            through_many_edges = true;
          }
          j += e - 1;
          if (!last && !through_many_edges) {
            within = !within;
          }
        }
        if (within || is_edge) {
          filled.insert({i, j});
          ++size;
        }
        last = is_edge;
      }
    }
    cout << endl;
    PrintMap(filled);
    return size;
  }

  static PuzzleInput GetInput(int part) {
    PuzzleInput input;
    std::string aux;
    while(!cin.eof()) {
      getline(cin, aux);
      input.trench.emplace_back(aux[0], util::getNumber(aux, 2));
    }
    return input;
  }

};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput(part);
  input.InitEdges();
  size_t pool_size = input.FillPool();
  cout << "Pool size: " << pool_size << endl;
}

};  // aoc2023_18

#endif /* _2023_ADVENTOFCODE_18_H_ */
