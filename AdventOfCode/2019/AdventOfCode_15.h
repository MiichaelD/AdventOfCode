/*
  Link:         http://adventofcode.com/2019/day/15
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/15/2019
*/

#ifndef _2019_ADVENTOFCODE_15_H_
#define _2019_ADVENTOFCODE_15_H_

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

namespace aoc2019_15 {  
  
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
  // const int DIRECTIONS[TOTAL_DIRECTIONS] = {NORTH, EAST, SOUTH, WEST};
  const int DIRECTIONS[TOTAL_DIRECTIONS] = {WEST, SOUTH, EAST, NORTH};

  const int WALL_STATUS = 0;
  const int FREE_STATUS = 1;
  const int SYS_STATUS = 2;

  template<class T>
  inline void printPair(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }

  void advancePos(int direction, pair<int,int> &pos) {
    switch(direction) {
      case NORTH: --pos.second; break;
      case SOUTH: ++pos.second; break;
      case WEST: --pos.first; break;
      case EAST: ++pos.first; break;
    };
  }

  struct Droid {
    pair<int,int> pos = {0, 0}; // x, y
    int lastDirection = NORTH;

    void setDirection(int direction) {
      lastDirection = direction;
    }

    void advance() {
      advancePos(lastDirection, pos);
    }

    void print() {
      cout << "Droid @ "; printPair(pos); cout << "- Last Dir: " << lastDirection << endl;
    }
  };

  inline void paintPos(
      const unordered_map<pair<int,int>, int, pair_hash> &map, const pair<int,int> &pos) {
    if (pos.first == 0 && pos.second == 0) {
      cout << "\033[1;31m\033[1;43m O\033[0m";
      return;
    }    
    auto entry = map.find(pos);
    if (entry == map.end()) {
      cout << "\033[1;30m\033[1;47m  \033[0m";
      return;
    }
    switch (entry->second) {
      case FREE_STATUS:
      cout << "\033[1;30m\033[1;47m F\033[0m"; break;
      case WALL_STATUS:
      cout << "\033[1;30m\033[1;46m W\033[0m"; break;
      case SYS_STATUS:
      cout << "\033[1;30m\033[1;43m S\033[0m"; break;
    }
  }

  void printMap(const unordered_map<pair<int,int>, int, pair_hash> &map) {
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (const auto &entry : map) {
      minX = min(minX, entry.first.first);
      maxX = max(maxX, entry.first.first);
      minY = min(minY, entry.first.second);
      maxY = max(maxY, entry.first.second);
      // cout << "Tile " << i++ << ": @ "; print(entry.first); cout << " => " << entry.second << endl; 
    }
    cout << "Printing: [x=" << maxX - minX + 1 << ", y=" << maxY - minY + 1 << "]" << endl;
      for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
          pair<int,int> pos = make_pair(x,y);
          paintPos(map, pos);
        }
        cout << endl;
      }
  }

  Droid droid;
  int64_t relativeBase = 0;
  unordered_map<pair<int,int>, int, pair_hash> map;

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
  
  inline void setValue(vector<int64_t> &intCodes, size_t index, int paramMode, int64_t newVal) {
    ensureSpace(intCodes, index);
    if (paramMode == POS_MODE) {
      intCodes[intCodes[index]] = newVal;
    } else if (paramMode == REL_MODE) {
      intCodes[intCodes[index] + relativeBase] = newVal;
    } else {
      intCodes[index] = newVal; // Not possible -- yet;
    }
  }

  void printInputs(deque<Droid> &inputs) {
    cout << "inputs: "<< endl;
    int i = -1;
    for (auto &droid: inputs) {
      cout << ++i << "): "; droid.print();
    }
    cout << endl;
  }

  int nextDir(int dir) {
    if (++dir == 5) {
      dir = 1;
    }
    return dir;
  }

  void updateInputs(deque<Droid> &inputs, deque<int64_t> &outputs) {
    // printInputs(inputs);
    int i = 0;
    for (int dir = droid.lastDirection; i < TOTAL_DIRECTIONS; ++i, dir = nextDir(dir)) {
      // if (droid.lastDirection == dir) {continue;}
      auto pos = droid.pos;
      advancePos(dir, pos);
      auto entry = map.find(pos);
      if (entry == map.end()) {
        droid.setDirection(dir);
        inputs.push_back(droid);
        break;
      }
    }
    if (inputs.empty()) {

      // for (int dir : DIRECTIONS) {
      i = 0;
      for (int dir = droid.lastDirection; i < TOTAL_DIRECTIONS; ++i, dir = nextDir(dir)) {
        auto pos = droid.pos;
        advancePos(dir, pos);
        auto entry = map.find(pos);
        if (entry == map.end() || entry->second == FREE_STATUS) {
          droid.setDirection(dir);
          inputs.push_back(droid);
          break;
        }
      }
    }
    std::cout << "INPUT: (" << inputs.size() << ") "; inputs.front().print();
  }

  bool processOutputs(deque<Droid> &inputs, deque<int64_t> &outputs) {
    auto pos = droid.pos;
    auto dr = inputs.front();
    switch(outputs.back()) {
      case WALL_STATUS:
        advancePos(droid.lastDirection, pos);
        map[pos] = outputs.back();
        inputs.pop_front();
        break;
      case FREE_STATUS: 
        inputs.pop_front();
        droid.setDirection(dr.lastDirection);
        droid.advance();
        map[droid.pos] = outputs.back();
        break;
      case SYS_STATUS:
        return true;
    }
    updateInputs(inputs, outputs);
    return false;
  }

  int times = 0;
  bool processIntCodes(vector<int64_t> &intCodes, deque<Droid> &inputs, deque<int64_t> &outputs) {
    int64_t intCode, aux1, aux2; 
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
          setValue(intCodes, pc + 1, param[0], inputs.front().lastDirection);
          pc += 2;
          if (++times >= MAX_TIMES) {
            return false;
          }
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          outputs.push_back(aux1);
          cout << "Output: " << aux1 << endl;
          pc += 2;
          if (processOutputs(inputs, outputs)) {
            cout << "Found System." << endl;
            return true;
          }
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

  size_t countTiles(const unordered_map<pair<int,int>, int, pair_hash> &tiles, const int type) {
    size_t tileCount = 0;
    int i = 0; 
    for (const auto &entry : tiles) {
      // cout << "Tile " << i++ << ": @ "; printPair(entry.first); cout << " => " << entry.second << endl; 
      if (entry.second == type) {
        ++tileCount;
      }
    }
    return tileCount;
  }

  void solve(int part = 1) {
    string input;
    cin >> input;
    vector<int64_t> intCodes = getIntCodes(input);
    deque<Droid> inputs {droid};
    deque<int64_t> outputs;
    if (part == 2) { }
    bool res = processIntCodes(intCodes, inputs, outputs);
    printMap(map);
    if (res) {
      cout << "System found in location:"; printPair(droid.pos); cout << endl;
    }
    cout << "Outputs: " << outputs.size() << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_15_H_ */