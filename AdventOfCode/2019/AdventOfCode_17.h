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
  
  const int LEFT = 5;
  const int RIGHT = 6;

  const int EOL = 10;
  const int SCAFFOLD = '#';
  const int SPACE = '.';
  const int COMMA = ',';
  
  const int A = 'A';  // Movement functions
  const int B = 'B';
  const int C = 'C';

  const int L = 'L';  // left
  const int R = 'R';  // right
  const int Y = 'y';  // yes
  const int N = 'n';  // no

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

  bool processIntCodes(vector<int64_t> &intCodes, deque<int> &inputs, deque<int> &outputs) {
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
          cout << "\tInput: " << inputs.front() << ". Remaining: " << inputs.size() << endl;
          setValue(intCodes, pc + 1, param[0], inputs.front());
          inputs.pop_front();
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

  void printMap(const vector<vector<char>> &map) {
    for (int y = 0; y < map.size(); ++y) {
      for (int x = 0; x < map.back().size(); ++x) {
        cout << ' ' << map[y][x];
      }
      cout << endl;
    }
  }

  vector<vector<char>> generateMap( deque<int> &outputs) {
    vector<vector<char>> result;
    result.emplace_back(vector<char>());
    int scaffoldPieces = 0;
    for (int i : outputs) {
      char c = (char) i;
      cout << ' ' <<  c;
      switch(i) {
        case 10: result.emplace_back(vector<char>()); break;
        case '^':
        case '>':
        case '<':
        case 'v': 
          robotPos = make_pair(result.back().size(), result.size() - 1);  // X, Y
          result.back().push_back(c);
          break;
        case SCAFFOLD: 
          ++scaffoldPieces;   // Fall through
        default:
          result.back().push_back(c);break;
      }
    }
    while (!result.back().size()) {
      result.pop_back();
    }
    cout << "There are " << scaffoldPieces << " scaffold pieces. Robot found in position [x, y] = "; 
    printPair(robotPos); cout << endl;
    return result;
  }

  vector<pair<int,int>> getIntersections(const vector<vector<char>> &map) {
    vector<pair<int,int>> result;
    for (int y = 1; y < (map.size() -1); ++y) {
      for (int x = 1; x < (map.back().size() - 1); ++x) {
        if (map[y][x] == SCAFFOLD && map[y-1][x] == SCAFFOLD && map[y+1][x] == SCAFFOLD
            && map[y][x-1] == SCAFFOLD && map[y][x+1] == SCAFFOLD) {
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
      // cout << "Intersection: "; printPair(p); cout << " -> total: " << result << endl;
    }
    return result;
  }

  pair<int,int> findNextOrientation(
    const vector<vector<char>> &map, const pair<int,int> &pos, int curOrient) {
      int x = pos.first, y = pos.second;
    if (curOrient != NORTH && y+1 < map.size() && map[y+1][x] == SCAFFOLD) { // Try Down;
      // cout << "\tSOUTH!" << endl;
      return make_pair(SOUTH, curOrient == WEST ? LEFT : RIGHT);
    }
    if (curOrient != SOUTH && y-1 >= 0 && map[y-1][x] == SCAFFOLD) { // Try Up;
      // cout << "\tNORTH!" << endl;
      return make_pair(NORTH, curOrient == WEST ? RIGHT : LEFT);
    }
    if (curOrient != WEST && x+1 < map.back().size() && map[y][x+1] == SCAFFOLD) { // Try East;
      // cout << "\tEast!" << endl;
      return make_pair(EAST, curOrient == SOUTH ? LEFT : RIGHT);
    }
    if (curOrient != EAST && x-1 >= 0 && map[y][x-1] == SCAFFOLD) { // Try West;
      // cout << "\tWest!" << endl;
      return make_pair(WEST, curOrient == SOUTH ? RIGHT : LEFT);
    }
    return make_pair(-1,-1); // Nowhere to go.
  }

  int getStepsCount(const vector<vector<char>> &map, pair<int,int> &pos, int curOrient) {
    int steps = 0;
    int x = pos.first, y = pos.second;
    switch(curOrient) {
      case NORTH: 
        for (--y; y >= 0 && map[y][x] == SCAFFOLD; --y, ++steps);
        ++y;
        break;
      case WEST:
        for (--x; x >= 0 && map[y][x] == SCAFFOLD; --x, ++steps);
        ++x;
        break;
      case SOUTH:
        for (++y; y < map.size() && map[y][x] == SCAFFOLD; ++y, ++steps);
        --y;
        break;
      case EAST:
        for (++x; x < map.back().size() && map[y][x] == SCAFFOLD; ++x, ++steps);
        --x;
        break;
    }
    pos = make_pair(x, y);
    return steps;
  }

  void printPath(const vector<vector<char>> &map) {
    pair<int,int> pos = robotPos;
    int totalSteps = 0, steps, orientation = NORTH, direction; // direction == where to turn;
    unordered_set<pair<int,int>, pair_hash> visitedPos;
    for (;;) {
      auto turnResult = findNextOrientation(map, pos, orientation);
      orientation = turnResult.first;  // Where is the robot looking at.
      direction = turnResult.second;   // Left or Right;
      steps = getStepsCount(map, pos, orientation);
      if ((orientation == -1 && direction == -1) || steps < 1) {
        break;
      }
      totalSteps += steps;
      cout << (direction == LEFT ? "L" : "R") << steps << ",";
      // cout << "New Pos: "; printPair(pos); cout << endl;
    }
    cout << endl << "Total Steps: " << totalSteps << endl;;
  }

  void solve(int part = 1) {
    string input;
    cin >> input;
    vector<int64_t> intCodes = getIntCodes(input);
    deque<int> inputs;
    deque<int> outputs;
    if (part == 1) {
      processIntCodes(intCodes, inputs, outputs);
      auto map = generateMap(outputs);
      auto intersections = getIntersections(map);
      int64_t intersectionsSum = getSumOfIntersectionCoords(intersections);
      cout << "Sum of intersections: " << intersectionsSum << endl;
    } else if (part == 2) {
      outputs[0] = 2;
      inputs = {
        A,COMMA,B,COMMA,A,COMMA,C,COMMA,C,COMMA,A,COMMA,C,COMMA,EOL, // MAIN ROUTINE
        L,COMMA,'1','0',COMMA,L,'6',COMMA,R,'1','0',EOL, // A  
        R,'6',COMMA,R,'8',COMMA,R,'8',COMMA,L,'6',COMMA,R,'8',EOL, // B
        L,'1','0',COMMA,R,'8',COMMA,R,'8',COMMA,L,'1','0',COMMA,R,'6',COMMA,R,'8',COMMA,R,'8',COMMA,L,'6',COMMA,R,'8',EOL,// C
        Y,EOL,EOL
      };
      processIntCodes(intCodes, inputs, outputs);
      // auto map = generateMap(outputs);
      // auto intersections = getIntersections(map);
      // int64_t intersectionsSum = getSumOfIntersectionCoords(intersections);
      // cout << "Sum of intersections: " << intersectionsSum << endl;
    // printPath(map);
    }
  }
};

#endif /* _2019_ADVENTOFCODE_17_H_ */