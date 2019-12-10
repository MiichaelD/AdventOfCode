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

  const char SPACE = '.';
  const char ASTEROID = '#';
  const char BEST_ASTEROID = 'X';

  const int DOWN = 1;
  const int LEFT = 2;

  const float EPSILON = 0.0001f;

  struct Result {
    vector<vector<int>> visibilityMap;
    pair<int,int> best;
    int maxVisibility;
  };

  struct SurroundingAsteroid {
    float angle;
    int orientation;
    int row;
    int col;
    SurroundingAsteroid(float a, int o, int r, int c) : angle(a), orientation(o), row(r), col(c) {}
  };

  // bool sortChingon(const SurroundingAsteroid& a, const SurroundingAsteroid& b) {
  //   if (a.orientation != b.orientation) {
  //     return a.orientation < b.orientation;
  //   }
  // }

  struct SortChingon {
    int row, col;
    SortChingon(int r, int c) : row{r}, col{c} {}
    bool operator () (const SurroundingAsteroid& a, const SurroundingAsteroid& b) {
      if (a.orientation != b.orientation) {
        return a.orientation < b.orientation;
      }
      if (fabs(a.angle - b.angle) > EPSILON) {
        return a.angle - b.angle;
      }
      int deltaA = abs(row - a.row) + abs(col - a.col);
      int deltaB = abs(row - b.row) + abs(col - b.col);
      return deltaA < deltaB;
    }
  };

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
        if (c == BEST_ASTEROID) {
          cout << "\033[1;31mX\033[0m";
        } else {
        cout << c;
        }
      }
      cout << endl;
    }
  }

  void print(const vector<SurroundingAsteroid> &vec) {
    cout << "Surrounding Asteroids: " << endl;
    int index = -1;
    for (const auto &sa: vec) {
      cout << "Asteroid " << ++index << ": [x=" << sa.col << ", y=" << sa.row << ']' << endl;
    }
  }

  inline int getOrientation(int row, int col, int r, int c) {
    return (r < row ? LEFT : 0) | (c > col ? DOWN : 0);
  }

  int getVisibility(const vector<vector<char>> &map, int row, int col) {
    if (map[row][col] == SPACE) {
      return 0;
    }
    int total = 0;
    unordered_map<pair<float,int>, int, pair_hash> visible;
    // cout << "\tAsteroid [" << row << ", " << col << "]: ";
    for (int i = 0, c, r; i < 2; ++i) {  // Direct asteroids;
      for (c = (i == 0 ? col + 1 : col - 1); c >= 0 && c < map[row].size(); i == 0 ? ++c : --c) {
        if (map[row][c] != SPACE) {
          // cout << (i == 0 ? "(right), " : "(left), ");
          ++total;
          break;
        }
      }
      for (r = (i == 0 ? row + 1 : row - 1); r >= 0 && r < map.size(); i == 0 ? ++r : --r) {
        if (map[r][col] != SPACE) {
          // cout << (i == 0 ? "(below), " : "(above), ");
          ++total;
          break;
        }
      }
    }
    // cout << total << " Direct Asteroids and ";

    for (int r = 0; r < map.size(); ++r) {  // Angular asteroids.
      for (int c = 0; c < map[r].size(); ++c) {
        if ((r == row || c == col) || map[r][c] == SPACE) { continue; }
        int orientation = getOrientation(row, col, r, c);
        float ratio = ((float) abs(row - r)) / abs(col - c);
        auto p = make_pair(ratio, orientation);
        if (visible.find(p) == visible.end()) {
          // cout << endl << "\t -> " << r << ", " << c << " is visible";
        } else {
          // cout << endl << "\t XX " << r << ", " << c << " is blocked";
        }
        ++visible[p];
      }
    }
    // cout << visible.size() << " Angular Asteroids." << endl;
    return total + visible.size();
  }

  Result createVisibilityMap(vector<vector<char>> &map) {
    Result result;
    result.visibilityMap = 
        vector<vector<int>>(map.size(), vector<int>(map.back().size(), 0));
    vector<vector<int>> &visMap = result.visibilityMap;
    int maxVisibility = INT_MIN;
    pair<int, int> bestLocation;
    for (int r = 0; r < map.size(); ++r) {
      for (int c = 0; c < map[r].size(); ++c) {
        visMap[r][c] = getVisibility(map, r, c);
        if (maxVisibility < visMap[r][c]) {
          maxVisibility = visMap[r][c];
          bestLocation.first = r;
          bestLocation.second = c;
        }
      }
    }
    result.best = bestLocation;
    result.maxVisibility = maxVisibility;
    map[bestLocation.first][bestLocation.second] = BEST_ASTEROID;
    return result;
  }

  vector<SurroundingAsteroid> getVaporizatioOrder(const vector<vector<char>> &map, int row, int col) {
    vector<SurroundingAsteroid> result;
    for (int r = 0; r < map.size(); ++r) {  // Angular asteroids.
      for (int c = 0; c < map[r].size(); ++c) {
        if ((r == row && c == col) || map[r][c] == SPACE) { continue; }
        int orientation = getOrientation(row, col, r, c);
        int deltaR = abs(row - r);
        int deltaC = abs(col - c);
        float ratio = (deltaR == 0 || deltaC == 0) ? 0 : ((float) deltaR) / deltaC;
        auto p = make_pair(ratio, orientation);
        result.emplace_back(ratio, orientation, r, c);
      }
    }
    return result;
  }

  void solve(int part = 1) {
    string input;
    vector<vector<char>> map;
    while (!cin.eof()){
      cin >> input;
      map.emplace_back(vector<char>(input.length(), '.'));
      for (int i = 0; i < input.length(); ++i) {
        map.back()[i] = input[i];
      }
    }
    auto result = createVisibilityMap(map);
    print(map);
    cout << "Asteroid [x=" << result.best.second << ", y=" << result.best.first;
    cout << "] visibility: " << result.maxVisibility << endl;
    auto vaporizationOrder = getVaporizatioOrder(map, result.best.first, result.best.second);
    sort(vaporizationOrder.begin(), vaporizationOrder.end(),
         SortChingon(result.best.first, result.best.second));
    // print(vaporizationOrder);
  }
};

#endif /* _2019_ADVENTOFCODE_10_H_ */