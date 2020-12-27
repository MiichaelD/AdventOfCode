/*
  Link:         http://adventofcode.com/2020/day/24
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/24/2020
*/

#ifndef _2020_ADVENTOFCODE_24_H_
#define _2020_ADVENTOFCODE_24_H_

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

namespace aoc2020_24 {
using namespace std;

const int kDays = 100;

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator () (std::pair<T1,T2> const &p) const {
    std::size_t h1 = std::hash<T1>()(p.first);
    std::size_t h2 = std::hash<T2>()(p.second);
    return h1 ^ h2;
  }
};

typedef pair<int,int> Tile;
typedef unordered_set<Tile,pair_hash> TileSet;

bool move(Tile &t, const string &dir, int index = 0) {
  if (dir[index] == 'e') {
    t.second++;
    return false;
  } else if (dir[index] == 'w') {
    t.second--;
    return false;
  } else if (dir[index] == 's') {
    if (dir[index+1] == 'e') {
      t.second++;
      t.first++;
      return true;
    } else if (dir[index+1] == 'w') {
      t.first++;
      return true;
    }
  } else if (dir[index] == 'n') {
    if (dir[index+1] == 'w') {
      t.second--;
      t.first--;
      return true;
    } else if (dir[index+1] == 'e') {
      t.first--;
      return true;
    }
  }
  return false;
}

Tile processInputLine(const string &input) {
  Tile result{0,0};
  for (int i = 0; i < input.size(); ++i) {
    if (move(result, input, i)) {
      ++i;
    }
  }
  // util::printPair(result); cout << "\tfrom '" << input << "'" << endl;
  return result;
}

void processInput(TileSet &tiles) {
  string input;
  while(!cin.eof()) {
    getline(cin,input);
    Tile t = processInputLine(input);
    if (tiles.find(t) != tiles.end()) {
      tiles.erase(t);
    } else {
      tiles.insert(t);
    }
  }
}

pair<pair<int,int>,pair<int,int>> findEdges(const TileSet &ts) {
  int maxX = INT_MIN, minX = INT_MAX, maxY = INT_MIN, minY = INT_MAX;
  for (const auto &entry : ts) {
    if (entry.first > maxY) {
      maxY = entry.first;
    }
    if (entry.first < minY) {
      minY = entry.first;
    }
    if (entry.second > maxX) {
      maxX = entry.second;
    }
    if (entry.second < minX) {
      minX = entry.second;
    }
  }
  return make_pair(make_pair(minY, maxY), make_pair(minX, maxX));
}

inline vector<Tile> getNeighbors(int f, int c) {
  vector<Tile> result{
      {f, c + 1}, // East
      {f, c - 1}, // West
      {f + 1, c + 1}, // SEast
      {f + 1, c}, // SWest
      {f - 1, c}, // NEast
      {f - 1, c - 1}, // NWest
  };
  return result;
}

int getFoundNeighborsCount(const TileSet &ts, int f, int c) {
  vector<Tile> neighbors = getNeighbors(f, c);
  int found = 0;
  for (const Tile &t : neighbors) {
    if (ts.find(t) != ts.end()) { ++found; }
  }
  return found;
}

void makeInteractive(TileSet &ts) {
  TileSet temp = ts;
  for (int i = 1; i <= kDays; ++i) {
    auto edges = findEdges(ts);
    for (int f = edges.first.first - 1; f < edges.first.second + 2; ++f) {
      for (int c = edges.second.first - 1; c < edges.second.second + 2; ++c) {
        int neighborFoundCount = getFoundNeighborsCount(ts, f, c);
        Tile tempTile{f, c};
        if (ts.find(tempTile) != ts.end()) {  // if black; 0 or more than 2
          if (neighborFoundCount == 0 || neighborFoundCount > 2) {
            temp.erase(tempTile);
          }
        } else {  // if white && 2 neighbors
          if (neighborFoundCount == 2) {
            temp.insert(tempTile);
          }
        }
      }
    }
    ts = temp;
  }
}

void solve(int part = 1) {
  TileSet tiles;
  processInput(tiles);
  cout << "Black tiles: " << tiles.size() << endl;
  if (part != 1) {
    makeInteractive(tiles);
    cout << "Day " << kDays << ": " << tiles.size() << endl;
  }
}

};  // aoc2020_24

#endif /* _2020_ADVENTOFCODE_24_H_ */
