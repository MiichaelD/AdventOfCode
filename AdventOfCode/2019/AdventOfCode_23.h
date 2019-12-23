/*
  Link:         http://adventofcode.com/2019/day/X
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/XX/2019
*/

#ifndef _2019_ADVENTOFCODE_23_H_
#define _2019_ADVENTOFCODE_23_H_

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

namespace aoc2019_23 {  
  using namespace std;

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1,T2> const &p) const {
      std::size_t h1 = std::hash<T1>()(p.first);
      std::size_t h2 = std::hash<T2>()(p.second);
      return h1 ^ h2;
    }
  };
  
  const int TOTAL_COMPUTERS = 50;

  const int POS_MODE = 0;
  const int IMM_MODE = 1;
  const int REL_MODE = 2;
  const int NAT_ADDRS = 255;

  int relativeBase = 0;

  struct computer{
    int id;
    vector<int64_t> intCode;
    size_t progCount = 0;
    deque<int64_t> outputs;
    computer(const vector<int64_t> &initialCodes) : intCode(initialCodes) {} 

    bool isFinished() {
      return progCount >= intCode.size() || intCode[progCount] == 99;
    }
  };



  template<class T>
  inline void printPair(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }

  template<class T>
  void printVec(const vector<T> &vec) {
    for (const T &i : vec) {
      cout << i << ",";
    }
    cout << endl;
  }

  template<class T>
  void printDeque(const deque<T> &deque) {
    for (const T &ele : deque) {
      cout << ele << ", ";
    }
    cout << endl;
  }

  void printMap(const vector<vector<char>> &map) {
    for (int y = 0; y < map.size(); ++y) {
      for (int x = 0; x < map.back().size(); ++x) {
        cout << ' ' << map[y][x];
      }
      cout << endl;
    }
  }

  template<class T>
  vector<T> getIntCodes(const string &input) {
    vector<T> intCodes;
    T aux = 0;
    bool negative = false;
    for (size_t i = 0 ; i < input.length(); ++i) {
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

  template<class T>
  inline void ensureSpace(vector<T> &intCodes, size_t index) {
    while (index >= intCodes.size()) {
      intCodes.push_back(static_cast<T>(0));
    }
  }

  template<class T>
  inline T getValue(vector<T> &intCodes, size_t index, int paramMode) {
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
  
  template<class T>
  inline void setValue(vector<T> &intCodes, size_t index, int paramMode, T newVal) {
    ensureSpace(intCodes, index);
    if (paramMode == POS_MODE) {
      intCodes[intCodes[index]] = newVal;
    } else if (paramMode == REL_MODE) {
      intCodes[intCodes[index] + relativeBase] = newVal;
    } else {
      intCodes[index] = newVal; // Not possible -- yet;
    }
  }

  bool processOutputs(deque<int64_t> &outputs, vector<deque<int64_t>> &messageQueue) {
    if (outputs.size() % 3 == 0) {
      int aux = outputs[0];
      messageQueue[aux].push_back(outputs[1]);
      messageQueue[aux].push_back(outputs[2]);
      outputs.pop_front();
      outputs.pop_front();
      outputs.pop_front();
      if (outputs[0] == NAT_ADDRS) {
        return true;
      }
    }
    return false;
  }

  template<class T>
  bool processIntCodes(
        computer &comp, vector<deque<int64_t>> &messageQueue) {
    T intCode, aux1, aux2, aux3; 
    vector<T> &intCodes = comp.intCode;
    size_t &pc = comp.progCount;
    
    for (;;) {
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
          if (messageQueue[comp.id].empty()) {
            aux1 = -1;
          } else {
            aux1 = messageQueue[comp.id].front();
            messageQueue[comp.id].pop_front();
            cout << comp.id << " - Input: " << aux1 << endl;
          }
          setValue(intCodes, pc + 1, param[0], aux1);
          pc += 2;
          if (messageQueue[comp.id].empty())
            return true;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          cout << comp.id << " - Output: " << aux1 << endl;
          comp.outputs.push_back(aux1);
          pc += 2;
          if (processOutputs(comp.outputs, messageQueue))
            return true;
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
          setValue(intCodes, pc + 3, param[2], static_cast<T>(aux1 < aux2 ? 1 : 0));
          pc += 4;
          break;
        case 8:  // Equals Jump
          aux1 = getValue(intCodes, pc + 1, param[0]);
          aux2 = getValue(intCodes, pc + 2, param[1]);
          setValue(intCodes, pc + 3, param[2], static_cast<T>(aux1 == aux2 ? 1 : 0));
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

  unordered_set<int64_t> packageSet;
  int64_t lastY = 0;
  bool processNat(vector<deque<int64_t>> &messageQueue, int finishedComputers) {
    if (messageQueue[NAT_ADDRS].size()) {
      cout << "Messages for computer 255: "; printDeque(messageQueue[NAT_ADDRS]);
      while (messageQueue[NAT_ADDRS].size() > 2) {
        messageQueue[NAT_ADDRS].pop_front();
      }
      if (finishedComputers >= TOTAL_COMPUTERS) {
        cout << "Sending: \t"; printDeque(messageQueue[NAT_ADDRS]);
        int64_t x = messageQueue[NAT_ADDRS].front();
        messageQueue[NAT_ADDRS].pop_front();
        int64_t y = messageQueue[NAT_ADDRS].front();
        messageQueue[NAT_ADDRS].pop_front();
        messageQueue[0].push_back(x);
        messageQueue[0].push_back(y);

        if (packageSet.find(y) != packageSet.end()) {
          cout << "Repeated Y package sent to comp 0: " << y << endl;
        }
        packageSet.insert(y);

        if (lastY == y) {
          cout << "2nd Y package sent to comp 0 IN A ROW: " << y << endl;
          return true;
        }
        lastY = y;
      }
    }
    return false;
  }

  void solve(int part = 1) {
    string input;
    cin >> input;
    vector<int64_t> intCodes = getIntCodes<int64_t>(input);
    vector<deque<int64_t>> messageQueue(1000, deque<int64_t>());
    vector<computer> computers(TOTAL_COMPUTERS + 1, computer(intCodes));
    int finishedComputers = 0;
    for (int i = 0; i < TOTAL_COMPUTERS; ++i) {
      messageQueue[i].push_back(i);
      computers[i].id = i;
      // cout << "Adding " << i << " to " << i << ". Size: " << messageQueue[i].size() << endl;
    }
    for (;;) {
      for (int i = 0; i < TOTAL_COMPUTERS; ++i) {
        if (messageQueue[i].size()) {
          // cout << "ID " << i << " running with messages: " << messageQueue[i].size();
          // cout << " and pc = " << computers[i].progCount <<  endl;
          finishedComputers = 0;
        } else {
          ++finishedComputers;
        }
        processIntCodes<int64_t>(computers[i], messageQueue);
      }

      cout << "Finished computers: " << finishedComputers << endl;
      if (processNat(messageQueue, finishedComputers)
          || finishedComputers >= TOTAL_COMPUTERS*2) {
        break;
      }
    }
  }

};

#endif /* _2019_ADVENTOFCODE_23_H_ */
