/*
  Link:         http://adventofcode.com/2019/day/17
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/16/2019
*/

#ifndef _2019_ADVENTOFCODE_17_H_
#define _2019_ADVENTOFCODE_17_H_

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

namespace aoc2019_17 {  
  using namespace std;

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1,T2> const &p) const {
      std::size_t h1 = std::hash<T1>()(p.first);
      std::size_t h2 = std::hash<T2>()(p.second);
      return h1 ^ h2;
    }
  };
  
  const int MAX_TIMES = 1000;

  const int POS_MODE = 0;
  const int IMM_MODE = 1;
  const int REL_MODE = 2;

  const int BLACK = 0;
  const int WHITE = 1;
  
  const int TOTAL_DIRECTIONS = 4;
  const int NORTH = 1;
  const int SOUTH = 2;
  const int WEST = 3;
  const int EAST = 4;

  pair<int,int> robotPos;

  template<class T>
  inline void printPair(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }

  int64_t relativeBase = 0;

  void printIntCodes(const vector<int> &intCodes) {
    for (int i : intCodes) {
      cout << i << ", ";
    }
    cout << endl;
  }

  vector<int64_t> getIntCodes(const string &input) {
    vector<int64_t> intCodes;
    int64_t aux = 0;
    bool negative = false;
    for (int i = 0 ; i < input.length(); ++i) {
      if (input[i] == '-') {
        negative = true;
      } else if (input[i] == ',') {
        intCodes.push_back(negative ? -aux : aux);
        negative = false;
        aux = 0;
      } else {
        aux *= 10;
        aux += (input[i] - '0');
      }
    }
    intCodes.push_back(negative ? -aux : aux);
    return intCodes;
  }

  inline void ensureSpace(vector<int64_t> &intCodes, size_t index) {
    if (index >= intCodes.size()) {
      intCodes.push_back(0ll);
    }
  }

  inline int64_t getValue(vector<int64_t> &intCodes, size_t index, int paramMode) {
    ensureSpace(intCodes, index);
    switch (paramMode) {
      default:
      case POS_MODE:
        return intCodes[intCodes[index]];
      case IMM_MODE:
        return intCodes[index];
      case REL_MODE:
        return intCodes[intCodes[index] + relativeBase];
    }
  }
  
  inline void setValue(vector<int64_t> &intCodes, size_t index, int paramMode, int newVal) {
    ensureSpace(intCodes, index);
    if (paramMode == POS_MODE) {
      intCodes[intCodes[index]] = newVal;
    } else if (paramMode == REL_MODE) {
      intCodes[intCodes[index] + relativeBase] = newVal;
    } else {
      intCodes[index] = newVal; // Not possible -- yet;
    }
  }

  bool processIntCodes(vector<int64_t> &intCodes, deque<int> &outputs) {
    int64_t intCode;
    int aux1, aux2; 
    for (int pc = 0;;) {
      intCode = intCodes[pc];
      int param[3] = {0, 0, 0};
      param[0] = (intCode / 100) % 10;
      param[1] = (intCode / 1000) % 10;
      param[2] = (intCode / 10000) % 10;
      
      switch (intCode %= 100) {
        case 1:  // fall through
        case 2:  // Operations
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          setValue(intCodes, pc + 3, param[2], intCode == 1 ? (aux1 + aux2) : (aux1 * aux2));
          pc += 4;
          break;
        case 3: // Input
          setValue(intCodes, pc + 1, param[0], 0);
          pc += 2;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          outputs.push_back(aux1);
          pc += 2;
          break;
        case 5:  // Non-Zero Jump
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          pc = aux1 ? aux2 : pc + 3;
          break;
        case 6:  // Eq-Zero Jump 
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          pc = (aux1 == 0 ? aux2 : pc + 3);
          break;
        case 7:  // Less-than Jump
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          setValue(intCodes, pc + 3, param[2], (aux1 < aux2 ? 1 : 0));
          pc += 4;
          break;
        case 8:  // Equals Jump
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          setValue(intCodes, pc + 3, param[2], (aux1 == aux2 ? 1 : 0));
          pc += 4;
          break;
        case 9:  // Update relative base
          aux1 = getValue(intCodes, pc + 1, param[0]);
          relativeBase += aux1;
          pc += 2;
          break;
        case 99:
        default:
          return false;
      }
    }
  }

  vector<vector<char>> generateMap( deque<int> &outputs) {
    vector<vector<char>> result;
    result.emplace_back(vector<char>());
    for (int i : outputs) {
      char c = (char) i;
      cout << c;
      if (i == 10) {
        result.emplace_back(vector<char>());
      } else {
        if (c == '^' || c == '>'  || c == '<'  || c == 'v' ) {
          robotPos = make_pair(result.back().size(), result.size() - 1);
        }
        result.back().push_back(c);
      }
    }
    
    while (!result.back().size()) {
      result.pop_back();
    }
    cout << "Robot found in position [x, y] = "; 
    printPair(robotPos); cout << endl;
    return result;
  }

  vector<pair<int,int>> getIntersections(const vector<vector<char>> &map) {
    vector<pair<int,int>> result;
    for (int y = 1; y < (map.size() -1); ++y) {
      for (int x = 1; x < (map.back().size() - 1); ++x) {
        if (map[y][x] == '#' && map[y-1][x] == '#' && map[y+1][x] == '#' && map[y][x-1] == '#'
            && map[y][x+1] == '#') {
          result.push_back(make_pair(x,y));
        } 
      }
    }
    return result;
  }

  int64_t getSumOfIntersectionCoords(const vector<pair<int,int>> &intersections) {
    int64_t result = 0;
    for (const auto &p : intersections) {
      result += p.first * p.second;
      cout << "Intersection: "; printPair(p); cout << " -> total: " << result << endl;
    }
    return result;
  }

  void solve1(int part = 1) {
    string input;
    cin >> input;
    vector<int64_t> intCodes = getIntCodes(input);
    deque<int> outputs;
    if (part == 2) { }
    processIntCodes(intCodes, outputs);
    auto map = generateMap(outputs);
    auto intersections = getIntersections(map);
    int64_t intersectionsSum = getSumOfIntersectionCoords(intersections);
    cout << "Sum of intersections: " << intersectionsSum << endl;
  }

  void solve2() {    
    
  }

  void solve(int part = 1) {
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};

#endif /* _2019_ADVENTOFCODE_17_H_ */