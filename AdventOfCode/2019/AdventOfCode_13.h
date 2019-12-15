/*
  Link:         http://adventofcode.com/2019/day/13
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/XX/2019
*/

#ifndef _2019_ADVENTOFCODE_13_H_
#define _2019_ADVENTOFCODE_13_H_

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

namespace aoc2019_13 {  
  
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

  const int BLACK = 0;
  const int WHITE = 1;
  
  const int EMPTY_TILE = 0;
  const int WALL_TILE = 1;
  const int BLOCK_TILE = 2;
  const int HOR_PADDLE_TILE = 3;
  const int BALL_TILE = 4;

  template<class T>
  inline void print(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }

  void printTiles(const unordered_map<pair<int,int>, int, pair_hash>  &tiles) {
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (const auto &entry : tiles) {
      minX = min(minX, entry.first.first);
      maxX = max(maxX, entry.first.first);
      minY = min(minY, entry.first.second);
      maxY = max(maxY, entry.first.second);
      // cout << "Tile " << i++ << ": @ "; print(entry.first); cout << " => " << entry.second << endl; 
    }
    cout << "Printing: [" << maxX - minX << ", " << maxY - minY << "]" << endl;
      for (int j = minY; j <= maxY; ++j) {
        for (int i = minX; i <= maxX; ++i) {
          pair<int,int> pos = make_pair(i,j);
          auto tile = tiles.find(pos);
          if (tile == tiles.end() || tile->second == EMPTY_TILE) {
            cout << "  ";
            continue;
          }
          switch (tile->second) {
            case WALL_TILE:
            cout << "\033[1;30m\033[1;47m  \033[0m"; break;
            case BLOCK_TILE:
            cout << "\033[1;30m\033[1;46m  \033[0m"; break;
            case HOR_PADDLE_TILE:
            cout << "\033[1;30m\033[1;43m P\033[0m"; break;
            case BALL_TILE:
            cout << "\033[1;34m\033[1;41m B\033[0m"; break;
          }
          // cout << (color == BLACK ? " X" : " _");
        }
        cout << endl;
      }
  }

  struct Tile {
    int tileType = EMPTY_TILE;
    int x, y;

    Tile(int x1, int y1, int type) : x(x1), y(y1), tileType(type) { }
  };

  long long relativeBase = 0;
  int ballX, paddleX, score = 0;
  unordered_map<pair<int,int>, int, pair_hash> tiles;

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

  void processOutputs(deque<long long> &outputs) {
    int n = outputs.size();
    if ((n % 3) == 0) {
      int x = outputs[n - 3];
      int y = outputs[n - 2];
      int aux = outputs[n - 1];
      if (x == -1 && y == 0) {
        score = aux;
        // printTiles(tiles);
      } else {
        tiles[make_pair(x, y)] = aux;
        if (aux == BALL_TILE) {
          ballX = x;
        } else if (aux == HOR_PADDLE_TILE) {
          paddleX = x;
        }
      }
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
          if (ballX < paddleX) aux1 = -1;
          else if (ballX > paddleX) aux1 = 1;
          else aux1 = 0;
          // cout << "Input: " << aux1 << ", BallX: " << ballX << ", PaddleX: " << paddleX << endl;
          setValue(intCodes, pc + 1, param[0], aux1);
          // outputs.pop_front();
          pc += 2;
          break;
        case 4:  // Output
          aux1 = getValue(intCodes, pc + 1, param[0]);
          outputs.push_back(aux1);
          // cout << aux1 << endl;
          processOutputs(outputs);
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

  size_t countTiles(const unordered_map<pair<int,int>, int, pair_hash> &tiles, const int type) {
    size_t tileCount = 0;
    int i = 0; 
    for (const auto &entry : tiles) {
      // cout << "Tile " << i++ << ": @ "; print(entry.first); cout << " => " << entry.second << endl; 
      if (entry.second == type) {
        ++tileCount;
      }
    }
    return tileCount;
  }

  void solve(int part = 1) {
    using namespace std;
    string input;
    cin >> input;
    vector<int64_t> intCodes = getIntCodes(input);
    deque<int64_t> outputs;
    if (part == 2) {
      intCodes[0] = 2;
    }
    processIntCodes(intCodes, outputs);
    printTiles(tiles);
    cout << "Block tiles: " << countTiles(tiles, BLOCK_TILE) << endl;
    cout << "Player's score: " << score << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_13_H_ */