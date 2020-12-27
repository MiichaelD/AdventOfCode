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

typedef pair<int,int> Tile;

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator () (std::pair<T1,T2> const &p) const {
    std::size_t h1 = std::hash<T1>()(p.first);
    std::size_t h2 = std::hash<T2>()(p.second);
    return h1 ^ h2;
  }
};

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
  util::printPair(result); cout << "\tfrom '" << input << "'" << endl;
  return result;
}

void solve1() {
  Tile startTile{0,0};
  unordered_set<Tile, pair_hash> tiles;
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
  cout << "Black tiles: " << tiles.size() << endl;
}

void solve2() {
  string input;
  cin >> input;
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } else {
    solve2();
  }
}

};  // aoc2020_24

#endif /* _2020_ADVENTOFCODE_24_H_ */
