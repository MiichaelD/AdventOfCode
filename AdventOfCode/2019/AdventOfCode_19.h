/*
  Link:         http://adventofcode.com/2019/day/19
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/19/2019
*/

#ifndef _2019_ADVENTOFCODE_19_H_
#define _2019_ADVENTOFCODE_19_H_

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

namespace aoc2019_19 {  

  using namespace std;
  
  const int POS_MODE = 0;
  const int IMM_MODE = 1;
  const int REL_MODE = 2;

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
    while (index >= intCodes.size()) {
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

  void processIntCodes(vector<int64_t> &intCodes, deque<int64_t> &inputs, deque<int64_t> &outputs) {
    int64_t intCode, aux1, aux2; 
    vector<int> param = {0, 0, 0};
    int inputIndex = 0;
    for (int pc = 0;;) {
      intCode = intCodes[pc];
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
          setValue(intCodes, pc + 1, param[0], inputs[inputIndex++]);
          pc += 2;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          outputs.push_back(aux1);
          // cout << "Outputs: " << aux1 << endl;
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
    string input;
    cin >> input;
    vector<int64_t> intCodes = getIntCodes(input);
    deque<int64_t> inputs, outputs;

    int target = 50;
    int affected = 0;
    for (size_t y = 0; y < target; ++y) {
      for (size_t x = 0 ; x < target; ++x) {
        vector<int64_t> tempIntCodes = intCodes;
        inputs.clear();
        inputs.push_back(x);
        inputs.push_back(y);
        // cout << "Trying [" << x <<", " << y << "]  Inputs size = " << inputs.size() << endl;
        processIntCodes(tempIntCodes, inputs, outputs);
        if (outputs.size()) {
          cout << (outputs.back() == 1 ? " #" : " .");
          if (outputs.back()) ++affected;
        }
      }
      cout << endl;
    }
    cout << endl << "Affected points: " << affected << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_19_H_ */