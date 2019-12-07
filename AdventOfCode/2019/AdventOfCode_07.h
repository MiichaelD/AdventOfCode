/*
  Link:         http://adventofcode.com/2019/day/7
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/6/2019
*/

#ifndef _2019_ADVENTOFCODE_07_H_
#define _2019_ADVENTOFCODE_07_H_

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

namespace aoc2019_07 {  
  using namespace std;
  
  const int POS_MODE = 0;
  const int IMM_MODE = 1;

  int defaultInputSignal = 0;

  void printIntCodes(const vector<int> &intCodes) {
    for (int i : intCodes) {
      cout << i << ", ";
    }
    cout << endl;
  }

  vector<int> getIntCodes(const string &input) {
    vector<int> intCodes;
    int aux = 0;
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

  inline int getValue(const vector<int> &intCodes, int index, int paramMode) {
    return paramMode == POS_MODE ? intCodes[intCodes[index]] : intCodes[index];
  }
  
  inline void setValue(vector<int> &intCodes, int index, int paramMode, int newVal) {
    if (paramMode == POS_MODE) {
      intCodes[intCodes[index]] = newVal;
    } else {
      intCodes[index] = newVal; // Not possible -- yet;
    }
  }

  void processIntCodes(vector<int> intCodes, int initialInputSignal, bool initial = true) {
    int intCode, aux1, aux2; 
    for (size_t i = 0; i < intCodes.size();) {
      intCode = intCodes[i];
      int param[3] = {0, 0, 0};
      param[0] = (intCode / 100) % 10;
      param[1] = (intCode / 1000) % 10;
      param[2] = (intCode / 10000) % 10;
      intCode %= 100;
      // cout << "Processing " << intCode << endl; 
      switch (intCode) {
        case 1: // fall through
        case 2: 
          aux1 = getValue(intCodes, i + 1, param[0]);
          aux2 = getValue(intCodes, i + 2, param[1]);
          setValue(intCodes, i + 3, POS_MODE, intCode == 1 ? (aux1 + aux2) : (aux1 * aux2));
          i += 4;
          break;
        case 3:
          setValue(intCodes, i + 1, POS_MODE, initial ? initialInputSignal : defaultInputSignal);
          initial = false;
          i += 2;
          break;
        case 4: 
          aux1 = getValue(intCodes, i + 1, param[0]);
          defaultInputSignal = aux1;
          // cout << aux1 << endl;
          i += 2;
          break;
        case 5: 
          aux1 = getValue(intCodes, i + 1, param[0]);
          aux2 = getValue(intCodes, i + 2, param[1]);
          i = aux1 ? aux2 : i + 3;
          break;
        case 6: 
          aux1 = getValue(intCodes, i + 1, param[0]);
          aux2 = getValue(intCodes, i + 2, param[1]);
          i = aux1 == 0 ? aux2 : i + 3;
          break;
        case 7: 
          aux1 = getValue(intCodes, i + 1, param[0]);
          aux2 = getValue(intCodes, i + 2, param[1]);
          setValue(intCodes, i + 3, POS_MODE, (aux1 < aux2 ? 1 : 0));
          i += 4;
          break;
        case 8: 
          aux1 = getValue(intCodes, i + 1, param[0]);
          aux2 = getValue(intCodes, i + 2, param[1]);
          setValue(intCodes, i + 3, POS_MODE, (aux1 == aux2 ? 1 : 0));
          i += 4;
          break;
        default:
          return;
      }
    }
  }

  void findMaxThrust(const vector<int> &intCodesOriginal, vector<int> phaseSettings) {
    int thrusterOutput = INT_MIN, aux;
    do {
      vector<int> intCodes = intCodesOriginal;

      cout << phaseSettings[0];
      for (int i = 1; i < phaseSettings.size(); ++i) {
        cout << "," << phaseSettings[i];
      }
      
      defaultInputSignal = 0;
      for (int i = 0 ; i < phaseSettings.size(); ++i) {
        processIntCodes(intCodes, phaseSettings[i]);
      }

      if (thrusterOutput < defaultInputSignal) {
        thrusterOutput = defaultInputSignal;
      }
      cout << (thrusterOutput == defaultInputSignal ? "\tMAX THRUSTER OUTPUR" : "") << endl;
    } while (next_permutation(phaseSettings.begin(), phaseSettings.end()));
    cout << thrusterOutput << endl;
  }

  void solve1() {
    string input;
    cin >> input;
    vector<int> intCodes = getIntCodes(input);
    findMaxThrust(intCodes, {0,1,2,3,4});
  }

  void solve2() {    
    string input;
    cin >> input;
    vector<int> intCodes = getIntCodes(input);
    findMaxThrust(intCodes, {5,6,7,8,9});
  }

  void solve(int part = 1) {
    part = 1;
    using namespace std;
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};

#endif /* _2019_ADVENTOFCODE_07_H_ */
