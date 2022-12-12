/*
  Link:         http://adventofcode.com/2022/day/8
  Compiling:    g++ -std=c++17 main.cpp -o main && cat 2022/AdventOfCode_08_input.txt | ./main
  Programmer:   Michael Duarte.
  Date:         12/07/2022

         0  1  2  3  4  5  6  7  8  9             765432
a = 8 (6,    5, 5,    5, 6, 3, 7, 6) _3_5556667 133010
b = 6 (6,          4, 5, 6,    7, 6) __45__6667 131100
c = 8 (6, 2, 5, 5, 4,       3, 7, 6) 23_55_66_7 122111
d = 7 (      5, 5, 4, 5, 6,    7, 6) __455566_7 123100
e = 4 (6,    5,          6,    7   ) ___5__66_7 121000
f = 9 (6, 2,    5, 4, 5, 6, 3, 7, 6) 23455_6667 132111
g = 7 (6,    5, 5,    5, 6,    7, 6) ___5556667 133000
*/

#ifndef _2022_ADVENTOFCODE_08_H_
#define _2022_ADVENTOFCODE_08_H_

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
#include "../util/robot_2020.h"

namespace aoc2022_08 {
using namespace std;

typedef vector<vector<int>> Map;

void printMap(const Map& map) {
  for (int r = 0; r < map.size(); ++r) {
    for (int c = 0; c < map[r].size(); ++c) {
      cout << map[r][c];
    }
    cout << endl;
  }
}

bool isTreeVisible(const Map& map, int r, int c) {
  // top
  int i = 0;
  int curTree = map[r][c];
  for (i = r - 1; i >= 0; --i) {
    if (map[i][c] >= curTree) {
      break;
    }
  }
  if (i == -1) {
    cout << curTree << " Visible from the top" << endl;
    return true;
  }
  // left
  for (i = c - 1; i >= 0; --i) {
    if (map[r][i] >= curTree) {
      break; 
    }
  }
  if (i == -1) {
    cout << curTree << " Visible from the left" << endl;
    return true;
  }
  // bottom
  for (i = r + 1; i < map[r].size(); ++i) {
    if (map[i][c] >= curTree) {
      break;
    }
  }
  if (i == map[r].size()) {
    cout << curTree << " Visible from the bottom" << endl;
    return true;
  }
  // right
  for (i = c + 1; i < map[r].size(); ++i) {
    if (map[r][i] >= curTree) {
      break;
    }
  }
  if (i == map.size()) {
    cout << curTree << " Visible from the right" << endl;
    return true;
  }
  cout << curTree << " Not visible" << endl;
  return false;
}

size_t treeScenicScore(const Map& map, int r, int c) {
  
  size_t score = 0, top_score = 0, left_score = 0, bottom_score = 0, right_score = 0;
  int i = 0;
  int curTree = map[r][c];
  // top
  for (i = r - 1; i >= 0; --i) {
      ++top_score;
    if (map[i][c] >= curTree) {
      break;
    }
  }
  cout << "\t" <<curTree << " Score from the top: " << top_score << endl;
  // left
  for (i = c - 1; i >= 0; --i) {
    ++left_score;
    if (map[r][i] >= curTree) {
      break;
    }
  }
  cout << "\t" <<curTree << " Score from the left: " << left_score << endl;
  // bottom
  for (i = r + 1; i < map[r].size(); ++i) {
    ++bottom_score;
    if (map[i][c] >= curTree) {
      break;
    }
  }
  cout << "\t" << curTree << " Score from the bottom: " << bottom_score << endl;
  // right
  for (i = c + 1; i < map[r].size(); ++i) {
    ++right_score;
    if (map[r][i] >= curTree) {
      break;
    }
  }
  cout << "\t" <<curTree << " Score from the right: " << right_score << endl;
  score = top_score * left_score * right_score * bottom_score;
  return score;
}

size_t visibleTreeCount(const Map& map) {
  size_t result = (map.front().size() * 2) + ((map.size() - 2) * 2);
  for (int r = 1; r < map.size() - 1; ++r) {
    for (int c = 1; c < map[r].size() - 1; ++c) {
      if (isTreeVisible(map, r, c)) {
        ++result;
      }
    }
  }
  return result;
}

size_t bestScenicScore(const Map& map) {
  size_t best_score = 0;
  pair<int,int> best_tree;
  for (int r = 1; r < map.size() - 1; ++r) {
    for (int c = 1; c < map[r].size() - 1; ++c) {
      size_t score = treeScenicScore(map, r, c); 
      if (score > best_score) {
        best_score = score;
        best_tree = {r, c};
        cout << "Tree " << map[r][c] << " at (" << r << ", " << c << ") is the best tree so far." << endl;
      }
    }
  }

  // size_t score = treeScenicScore(map, 3, 2);
  // if (score > best_score) {
  //   best_score = score;
  //   best_tree = {3, 2};
  // }
  cout << "Tree " << map[best_tree.first][best_tree.second] << " at (";
  cout << best_tree.first << ", " << best_tree.second << ") is the best tree." << endl;
  return best_score;
}

void solve(int part = 1) {
  string line;
  Map map;
  while(getline(cin, line)) {
    map.emplace_back(vector<int>());
    for (char c: line) {
      map.back().push_back(c - '0');
    }
  }
  printMap(map);
  if (part == 1) {
    size_t result = visibleTreeCount(map);
    cout << "Total visible trees: " << result << endl;
  } else {
    size_t result = bestScenicScore(map);
    cout << "Tree with best score: " << result << endl;
  }
}

};  // aoc2022_08

#endif /* _2022_ADVENTOFCODE_08_H_ */
