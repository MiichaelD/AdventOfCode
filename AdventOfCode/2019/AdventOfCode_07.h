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

  void processIntCodes(vector<int> &intCodes, int &pc, deque<int> &outputs) {
    int intCode, aux1, aux2; 
    while (true) {
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
          setValue(intCodes, pc + 3, POS_MODE, intCode == 1 ? (aux1 + aux2) : (aux1 * aux2));
          pc += 4;
          break;
        case 3: // Input
          setValue(intCodes, pc + 1, POS_MODE, outputs.front());
          outputs.pop_front();
          pc += 2;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          outputs.push_back(aux1);
          cout << aux1 << endl;
          pc += 2;
          return;
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
          setValue(intCodes, pc + 3, POS_MODE, (aux1 < aux2 ? 1 : 0));
          pc += 4;
          break;
        case 8:  // Equals Jump
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          setValue(intCodes, pc + 3, POS_MODE, (aux1 == aux2 ? 1 : 0));
          pc += 4;
          break;
        default:
          return;
      }
    }
  }

  int countAmpsRunning(const vector<vector<int>> &intCodes, const vector<int> &pc) {
    int running = 0;
    for (int i = 0; i < intCodes.size(); ++i) {
      if (intCodes[i][pc[i]] != 99) {
        ++running;
      } 
    }
    return running;
  }

  void findMaxThrust(const vector<int> &intCodes, vector<int> phaseSettings) {
    int thrusterOutput = INT_MIN, aux;
    do {
      vector<vector<int>> intCodesVec =  {intCodes, intCodes, intCodes, intCodes, intCodes};
      vector<int> pc {0,0,0,0,0};

      cout << "Permutation: " << phaseSettings[0];
      for (int i = 1; i < phaseSettings.size(); ++i) {
        cout << "," << phaseSettings[i];
      }
      cout << endl;
      
      // Initialization with Phase Settings + 0 as initial input for first Amp.
      deque<int> outputs;
      for (int i = 0 ; i < phaseSettings.size(); ++i) {
        outputs.push_front(phaseSettings[i]);
        if (i == 0) {
          outputs.push_back(0);
        }
        processIntCodes(intCodesVec[i], pc[i], outputs);
        thrusterOutput = max(thrusterOutput, outputs.back());
      }
      for (int i = 0; outputs.size() && countAmpsRunning(intCodesVec, pc); i = (i+1) % 5) {
        processIntCodes(intCodesVec[i], pc[i], outputs);
        if (thrusterOutput < outputs.back()) {
          thrusterOutput = outputs.back();
          cout << "^^^^^^^^^ MAX THRUSTER OUTPUT!" << endl;
        }
      }
    } while (next_permutation(phaseSettings.begin(), phaseSettings.end()));
    cout << "Max thurster output: " << thrusterOutput << endl;
  }

  void solve(int part = 1) {
    using namespace std;
    string input;
    cin >> input;
    vector<int> intCodes = getIntCodes(input);
    vector<int> initialPhaseSettings;
    if (part == 1) {
      initialPhaseSettings = {0,1,2,3,4};
    } else {
      initialPhaseSettings = {5,6,7,8,9};
    }
    findMaxThrust(intCodes, initialPhaseSettings);
  }
};

#endif /* _2019_ADVENTOFCODE_07_H_ */
