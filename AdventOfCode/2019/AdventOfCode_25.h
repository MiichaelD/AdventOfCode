/*
  Link:         http://adventofcode.com/2019/day/25
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/25/2019
*/

#ifndef _2019_ADVENTOFCODE_25_H_
#define _2019_ADVENTOFCODE_25_H_

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

namespace aoc2019_25 {  
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
  int relativeBase = 0;

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

  template<class T>
  bool processIntCodes(vector<T> &intCodes, deque<T> &inputs, deque<T> &outputs, int part = 1) {
    T intCode, aux1, aux2; 
    size_t inputIndex = -1;
    for (size_t pc = 0;;) {
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
          aux1 = 0;
          if (!inputs.empty()) {
            aux1 = inputs.front();
            inputs.pop_front();
          } else {
            cout << "Finished playing - no more commands" << endl;
            return true;
          }
          cout << static_cast<char>(aux1) << std::flush;
          setValue(intCodes, pc + 1, param[0], aux1);
          pc += 2;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          if (aux1 > 127) {
            cout << "\tResult: " << aux1 << std::flush;
          } else {
            cout << static_cast<char>(aux1) << std::flush;
          }
          if (aux1 == 'U') return false;
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

  void generateInput(deque<int> &inputs) {
    string inputStr = "";
    // Hull breach - E,S,W 

    // Observatory - N,E,W - asterisk  - E
    inputStr += "eastE";  
    inputStr += "take asteriskE"; 
    // inputStr += "westE";  
    
    // Gift Wraping Center - S,W  - EE
    // inputStr += "eastE";  
    // inputStr += "westE";  

    // Warp Drive Maintenance  - N,E - escape pod -- E,E,S
    // inputStr += "southE";  
    // inputStr += "take escape podE";  // Launched into space
    // inputStr += "northE";

    // Science Lab - W - photons  - E,E,S,E
    // inputStr += "eastE";   
    // inputStr += "take photonsE";  // Suddently  completely dark - eaten by Grue!
    // inputStr += "westE";   

    // Arcade - N,S,W - giant electromagnet - E,N
    inputStr += "northE"; 
    // inputStr += "take giant electromagnetE";  // The giant electromagnet is stuck to you.  You can't move!!
    // inputStr += "southE";  

    // Passages - N,S,W - hypercube - E,N,N
    inputStr += "northE"; 
    inputStr += "take hypercubeE"; 
    // inputStr += "southE";  

    // Storage - N,S - coin - E,N,N,N
    inputStr += "northE"; 
    inputStr += "take coinE"; 

    // Navigation - S,W - easter egg - E,N,N,N,N
    inputStr += "northE"; 
    inputStr += "take easter eggE"; 

    // hot chocolate fountain - E -  - E,N,N,N,N,W
    // inputStr += "westE"; 
    // inputStr += "eastE"; 

    inputStr += "southE";  // -> Back to storage
    inputStr += "southE"; // -> Back to Pasages

    // Holodeck - E - E,N,N,W
    // inputStr += "westE"; 

    inputStr += "southE"; // -> Back to Arcade

    // Crew quarters - N,E,S
    inputStr += "westE"; 

    // Engineering - molten lava - N
    // inputStr += "southE"; 
    // inputStr += "take molten lavaE"; // The molten lava is way too hot! You melt!
    // inputStr += "northE"; 

    // Corridor - N,S - spool of cat6
    inputStr += "northE"; 
    inputStr += "take spool of cat6E"; 

    // Kitchen - S,W - shell
    inputStr += "northE"; 
    inputStr += "take shellE"; 

    // Security Checkpoint - N,E
    inputStr += "westE"; 

    // Pressure-Sensitive Floor - N,E
    inputStr += "northE"; 


    // inputStr += "westE";  
    // inputStr += "northE"; 
    inputStr += "";
    // inputStr += "";
    inputStr += "invE"; 

    for (char c : inputStr) {
      inputs.push_back(c == 'E' ? 10 : (int) c);
    }
  }

  void solve(int part = 1) {
    string input;
    cin >> input;
    vector<int> intCodes = getIntCodes<int>(input);
    
    // Adding the following extra spaces fixes the following problems:
    // Case 1: No output, straight crash: main: malloc.c:2385: sysmalloc: Assertion `(old_top == initial_top (av) && old_size == 0) || ((unsigned long) (old_size) >= MINSIZE && prev_inuse (old_top) && ((unsigned long) old_end & (pagesize - 1)) == 0)' failed.
    //         Aborted
    // Case 2: Output works as expected but ends with this error: double free or corruption (out)
    //         Aborted
    for (int i = 0; i < 300; ++i) {
      intCodes.push_back((0));
    }
    deque<int> inputs, outputs;
    generateInput(inputs);
    processIntCodes(intCodes, inputs, outputs, part);
    cout << "Outputs size: " << outputs.size() << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_25_H_ */