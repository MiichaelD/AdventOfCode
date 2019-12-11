/*
  Link:         http://adventofcode.com/2019/day/11
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/XX/2019
*/

#ifndef _2019_ADVENTOFCODE_11_H_
#define _2019_ADVENTOFCODE_11_H_
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

namespace aoc2019_11 {  

  using namespace std;

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1,T2> const &p) const {
      std::size_t h1 = std::hash<T1>()(p.first);
      std::size_t h2 = std::hash<T2>()(p.second);
      return h1 ^ h2;
    }
  };
  
  const int POS_MODE = 0;
  const int IMM_MODE = 1;
  const int REL_MODE = 2;
  
  const int ORIENTATION_LEFT = 0;
  const int ORIENTATION_RIGHT = 1;
  const int ORIENTATION_UP = 2;
  const int ORIENTATION_DOWN = 3;

  template<class T>
  inline void print(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }
  
  class Robot {
    int x,y;
    int orientation;
    int totalPanels = 0;
    unordered_map<pair<int,int>, int, pair_hash> paintedCoords;
    
    public:
    pair<int,int> getCoords() const {
      return make_pair(x,y);
    }

    int getColorAt(const pair<int,int>  &p) const {
      auto findRes = paintedCoords.find(p);
      if (findRes != paintedCoords.end()){
        return findRes->second;
      }
      return 0; // black by default
    }

    int getColor() const {
      auto findRes = paintedCoords.find(getCoords());
      if (findRes != paintedCoords.end()){
        return findRes->second;
      }
      return 0; // black by default
    }

    unordered_map<pair<int,int>, int, pair_hash> getPaintedPanels() const {
      return paintedCoords;  // copy
    }

    void paint(int color) {
      // auto pos = getCoords();
      // auto findRes = paintedCoords.find(pos);
      // if (findRes == paintedCoords.end()){
      //    cout << "New position saved: "; print(pos); cout << "- Total " << ++totalPanels << endl;
      // }
      paintedCoords[getCoords()] = color;
    }
    
    void move(int direction) {
      switch(orientation) {
      case ORIENTATION_LEFT: 
        orientation = direction == ORIENTATION_LEFT ? ORIENTATION_DOWN : ORIENTATION_UP;
        break;
      case ORIENTATION_RIGHT:
        orientation = direction == ORIENTATION_LEFT ? ORIENTATION_UP : ORIENTATION_DOWN;
        break;
      case ORIENTATION_UP: 
        orientation = direction == ORIENTATION_LEFT ? ORIENTATION_LEFT : ORIENTATION_RIGHT;
        break;
      case ORIENTATION_DOWN: 
        orientation = direction == ORIENTATION_LEFT ? ORIENTATION_RIGHT : ORIENTATION_LEFT;
        break;
      }

      switch (orientation) {
      case ORIENTATION_LEFT: --x; break;
      case ORIENTATION_RIGHT: ++x; break;
      case ORIENTATION_UP: --y; break;
      case ORIENTATION_DOWN: ++y; break;
      }
    }
  };

  long long relativeBase = 0;
  Robot robot;

  void printIntCodes(const vector<int> &intCodes) {
    for (int i : intCodes) {
      cout << i << ", ";
    }
    cout << endl;
  }

  vector<long long> getIntCodes(const string &input) {
    vector<long long> intCodes;
    long long aux = 0;
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

  inline void ensureSpace(vector<long long> &intCodes, size_t index) {
    if (index >= intCodes.size()) {
      intCodes.push_back(0ll);
    }
  }

  inline long long getValue(vector<long long> &intCodes, size_t index, int paramMode) {
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
  
  inline void setValue(vector<long long> &intCodes, size_t index, int paramMode, long long newVal) {
    ensureSpace(intCodes, index);
    if (paramMode == POS_MODE) {
      intCodes[intCodes[index]] = newVal;
    } else if (paramMode == REL_MODE) {
      intCodes[intCodes[index] + relativeBase] = newVal;
    } else {
      intCodes[index] = newVal; // Not possible -- yet;
    }
  }

  void processRobotOutputs(deque<long long> &outputs) {
    int n = outputs.size();
    if ((n % 2) == 0) {
      robot.paint(outputs[n - 2]);
      robot.move(outputs[n - 1]);
    }
  }

  void processIntCodes(vector<long long> &intCodes, deque<long long> &outputs) {
    long long intCode, aux1, aux2; 
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
          // setValue(intCodes, pc + 1, param[0], outputs.front());
          // outputs.pop_front();
          setValue(intCodes, pc + 1, param[0], robot.getColor());
          pc += 2;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          outputs.push_back(aux1);
          // cout << aux1 << endl;
          processRobotOutputs(outputs);
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
          return;
      }
    }
  }

  void solve(int part = 1) {
    using namespace std;
    string input;
    cin >> input;
    vector<long long> intCodes = getIntCodes(input);
    deque<long long> outputs;
    if (part == 2) {
      robot.paint(1); // start at white panel
    }
    processIntCodes(intCodes, outputs);
    cout << "Panels painted: " << robot.getPaintedPanels().size() << endl; 
  }
};

#endif /* _2019_ADVENTOFCODE_11_H_ */