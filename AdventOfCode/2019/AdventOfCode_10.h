/*
  Link:         http://adventofcode.com/2019/day/10
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/XX/2019
*/

#ifndef _2019_ADVENTOFCODE_10_H_
#define _2019_ADVENTOFCODE_10_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc2019_10 {  
  using namespace std;

  const char ASTEROID = '#';
  const char SPACE = '.';

  const int DOWN = 1;
  const int RIGHT = 2;

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1,T2> const &p) const {
      std::size_t h1 = std::hash<T1>()(p.first);
      std::size_t h2 = std::hash<T2>()(p.second);
      return h1 ^ h2;
    }
  };

  template<class T>
  void print(const vector<vector<T>> &map) {
    cout << "Map: " << endl;
    for (const auto &vec: map) {
      for (T c : vec) {
        cout << c;
      }
      cout << endl;
    }
  }

  inline int getOrientation(int row, int col, int r, int c) {
    return (r > row ? RIGHT : 0) | (c > col ? DOWN : 0);
  }

  int getVisibility(const vector<vector<char>> &map, int row, int col) {
    if (map[row][col] == SPACE) {
      return 0;
    }
    int total = 0;
    unordered_set<pair<float,int>, pair_hash> visible;
    // cout << "\tAsteroid [" << row << ", " << col << "]: ";
    // Direct asteroids;
    for (int i = 0, c, r; i < 2; ++i) {
      for (c = (i == 0 ? col + 1 : col - 1); c >= 0 && c < map[row].size(); i == 0 ? ++c : --c) {
        if (map[row][c] == ASTEROID) {
          // cout << (i == 0 ? "(right), " : "(left), ");
          ++total;
          break;
        }
      }
      for (r = (i == 0 ? row + 1 : row - 1); r >= 0 && r < map.size(); i == 0 ? ++r : --r) {
        if (map[r][col] == ASTEROID) {
          // cout << (i == 0 ? "(below), " : "(above), ");
          ++total;
          break;
        }
      }
    }
    // cout << total << " Direct Asteroids and ";

    // Angular asteroids.
    for (int r = 0; r < map.size(); ++r) {
      for (int c = 0; c < map[r].size(); ++c) {
        if ((r == row || c == col) || map[r][c] == SPACE) { continue; }
        int orientation = getOrientation(row, col, r, c);
        float ratio = ((float) abs(row - r)) / abs(col - c);
        auto p = make_pair(ratio, orientation);
        if (visible.find(p) == visible.end()) {
          // cout << endl << "\t -> " << r << ", " << c << " is visible";
          visible.insert(p);
        } else {
          // cout << endl << "\t XX " << r << ", " << c << " is blocked";
        }
      }
    }
    // cout << visible.size() << " Angular Asteroids." << endl;

    return total + visible.size();
  }

  vector<vector<int>> createVisibilityMap(const vector<vector<char>> &map) {
    vector<vector<int>> result = 
        vector<vector<int>>(map.size(), vector<int>(map.back().size(), 0));
    int maxVisibility = INT_MIN;
    for (int r = 0; r < map.size(); ++r) {
      for (int c = 0; c < map[r].size(); ++c) {
        result[r][c] = getVisibility(map, r, c);
        maxVisibility = max(maxVisibility, result[r][c]);
      }
    }
    cout << maxVisibility << endl;
    return result;
  }

  void solve1() {   
    string input;
    vector<vector<char>> map;
    while (!cin.eof()){
      cin >> input;
      map.emplace_back(vector<char>(input.length(), '.'));
      for (int i = 0; i < input.length(); ++i) {
        map.back()[i] = input[i];
      }
    }
    print(map);
    auto visibilityMap = createVisibilityMap(map);
  }

  void solve2() {    
    
  }

  void solve(int part = 1) {
    using namespace std;
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};

#endif /* _2019_ADVENTOFCODE_10_H_ */