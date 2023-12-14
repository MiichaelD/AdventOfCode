/*
  Link:         http://adventofcode.com/2023/day/14
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/14/2023
*/

#ifndef _2023_ADVENTOFCODE_14_H_
#define _2023_ADVENTOFCODE_14_H_

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

namespace aoc2023_14 {
using namespace std;

class PuzzleInput {
  public:
  vector<string> map;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    while(!cin.eof()) {
      getline(cin, input.map.emplace_back());
    }
    return input;
  }

  void ShakeNorth() {
    cout << endl << "Shaking North" << endl << endl;
    for (int c = 0; c < map.back().size(); ++c) {
      int empty_spaces = 0;
      int first_empty_row = -1;
      for (int r = 0; r < map.size(); ++r) {
        switch(map[r][c]) {
          case 'O': {
            if (!empty_spaces) {
              break;
            }
            map[first_empty_row++][c] = 'O';
            map[r][c] = '.';
            --r; // let's reevaluate.
            // int distance_to_move = r - first_empty_row;
            // int movable_stones = 1;
            // int r2 = r + 1;
            // // how many stones we can move down?
            // for (;r2 < map.size() && empty_spaces; ++r2, --empty_spaces) {
            //   if (map[r2][c] != '0') {
            //     break;
            //   }
            //   ++movable_stones;
            // }
            // // move stones down;
            // int moved = std::min(empty_spaces, movable_stones);
            // for (int i = first_empty_row; i < moved; ++i) {
            //   map[r][c] = '0';
            // }
            // // reset rest of stones up;
            // // for (;r2 )
          }
          break;
          case '.': 
            if (empty_spaces++ == 0) {
              first_empty_row = r;
            }
            break;
          case '#': 
            first_empty_row = -1;
            empty_spaces = 0;
            break;
        }
      }
    }
  }

  int64_t GetValue() {
    int64_t result = 0;
    for (int r = 0; r < map.size(); ++r) {
      int64_t row_result = 0;
      for (int c = 0; c < map.back().size(); ++c) {
        if (map[r][c] == 'O') {
          row_result += map.size() - r;
        }
      }
      cout << "\tresult in row " << r << ", is: " << row_result << endl;
      result += row_result;
    }
    return result;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  util::printVector(input.map, true);
  input.ShakeNorth();
  util::printVector(input.map, true);
  cout << "Result: " << input.GetValue() << endl;

}

};  // aoc2023_14

#endif /* _2023_ADVENTOFCODE_14_H_ */
