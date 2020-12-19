/*
  Link:         http://adventofcode.com/2020/day/17
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/17/2020
*/

#ifndef _2020_ADVENTOFCODE_17_H_
#define _2020_ADVENTOFCODE_17_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <cstring>  // strcmp()
#include <ctype.h>  // char related functions
#include <climits>  // INT_MAX and such numeric limits
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/util.h"

namespace aoc2020_17 {


using namespace std;

constexpr char kActive = '#';
constexpr char kInactive = '.';
const int kMaxIt = 1;

int kMax = 8;

class Cell {
public:
  Cell(char c) : currentState{c}, futureState{c}, x{0}, y{0}, z{0}  { }
  Cell(char c, int xx, int yy, int zz)
      : currentState{c}, futureState{c}, x{xx}, y{yy}, z{zz} {
        // cout << "Cell('" << c <<"' @ " << x << ',' << y << ',' << z << ')' << endl;
        // cout << "Created '" << toString() << "'" << endl;
      }

  static string computeId(int x, int y, int z) {
    stringstream output;
    output << x << "," << y << "," << z;
    return output.str();
  }

 inline bool commit() {
   bool result = currentState != futureState;
   currentState = futureState;
   return result;
 }

 inline char getValue() const {
   return currentState;
 }

 inline void setValue(char state) {
   futureState = state;
 }

 inline int isActiveNum() const {
   return isActive() ? 1 : 0;
 }

 inline bool isActive() const {
   return currentState == kActive;
 }

 inline int isInactiveNum() const {
   return isInactive() ? 1 : 0;
 }

 inline bool isInactive() const {
   return currentState == kInactive;
 }

 tuple<int64_t,int64_t,int64_t> getPosition() const {
   return {x, y, z};
 }

 inline int64_t getX() const { return x; }
 inline int64_t getY() const { return y; }
 inline int64_t getZ() const { return z; }

 vector<tuple<int,int, int>> getNeighbors() const {
   vector<tuple<int,int, int>> result;
   for (int i = -1; i < kMax - 1; ++i) {
     for (int j = -1; j < kMax - 1; ++j) {
        for (int k = -1; k < kMax - 1; ++k) {
          if (i == 0 && j == 0 && k == 0) {
            continue;
          }
          result.emplace_back(x - i, y - j, z - k);
        }
      }
   }
   return result;
 }

  bool operator==(const Cell &other) const {
    return (x == other.getX() && y == other.getY() && z == other.getZ());
  }

  size_t operator () (Cell const &other) const {
    auto p = other.getPosition();
    return get<0>(p) ^ get<1>(p) ^ get<2>(p);
  }

  const string& toString() const {
    if (cachedStr.empty()) {
      cachedStr = computeId(x, y, z);
    }
    return cachedStr;
  }

private:
  char futureState = kInactive;
  char currentState = kInactive;
  int64_t x, y, z;
  mutable string cachedStr = "";

  string computeId() const {
    stringstream output;
    output << x << "," << y << "," << z;
    return output.str();
  }
};

//http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
struct CellHasher {
  std::size_t operator()(const Cell& c) const {
    return hash<string>()(c.toString());
  }
};

bool isCellActive(const unordered_map<string,Cell> &cells, const string &id) {
  const auto cell = cells.find(id);
  if (cell != cells.end()) {
    return cell->second.isActive();
  }
  return false;
}

Cell* getCell(unordered_map<string,Cell> &cells, const string &id) {
  auto cellIt = cells.find(id);
  if (cellIt != cells.end()) {
    return &(cellIt->second);
  }
  return nullptr;
}

Cell* insertCell(unordered_map<string,Cell> &cells, char c, int x, int y, int z) {
  Cell cell(c, x, y, z);
  auto res = cells.insert({cell.toString(), cell});
  return &(res.first->second);
}

void printCells(unordered_map<string,Cell> &cells, size_t iterations = 0) {
  int kStart = 0, kEnd = 0;
  int xyStart = 0, xyEnd = kMax;
  for (int i = 0 ; i < iterations; ++i) {
    --kStart; ++kEnd;
    --xyStart; ++xyEnd;
  }
  cout << "Printing: xy from " <<  xyStart << ", to " << xyEnd;
  cout << ". z from " << kStart << ", to " << kEnd << endl;

  for (int k = kStart; k <= kEnd ; ++k) {
    cout << "Z: " << k << endl;
    for (int j = xyStart; j <= xyEnd; ++j) {
      for (int i = xyStart; i <= xyEnd; ++i) {
        const Cell *c = getCell(cells, Cell::computeId(i,j,k));
        if (c) {
          cout << c->getValue();
        } else {
          cout << ".";
        }
      }
      cout << endl;
    }
    cout << endl;
  }
}

size_t getActiveCells(unordered_map<string,Cell> &cells) {
  size_t active = 0;
  for (auto &entry : cells) {
    entry.second.commit();
    active += entry.second.isActiveNum();
  }
  return active;
}

 void updateIteratively(unordered_map<string, Cell> &cells, size_t iteration) {
  int kStart = 0, kEnd = 0;
  int xyStart = 0, xyEnd = kMax ;
  for (int i = 0 ; i < iteration; ++i) {
    --kStart; ++kEnd;
    --xyStart; ++xyEnd;
  }
  cout << "Updating: xy from " <<  xyStart << ", to " << xyEnd;
  cout << ". z from " << kStart << ", to " << kEnd << endl;

  for (int k = kStart; k <= kEnd ; ++k) {
    for (int j = xyStart; j <= xyEnd; ++j) {
      for (int i = xyStart; i <= xyEnd; ++i) {
        string cellId = Cell::computeId(i,j,k);
        Cell *cell = getCell(cells, cellId);
        if (cell == nullptr) {
          cell = insertCell(cells, kInactive, i, j, k);
        }
        const auto neighbors = cell->getNeighbors();
        size_t activeNeighbors = 0; 
          // cout << "Updating cell: " << cell->toString() << ", n: " << neighbors.size();
        for (const auto &n : neighbors) {
          const int nX = get<0>(n), nY = get<1>(n), nZ =get<2>(n); 
          const string nId = Cell::computeId(nX, nY, nZ);
          // cout << "\tn: " << nId;
          Cell *neighbor = getCell(cells, nId);
          // cout << ", exists ? " << (neighbor == nullptr ? "False\t" : "True\t");
          if (neighbor) {
            if (neighbor->isActive()) {
              ++activeNeighbors;
              // cout << "and Active" << endl;
            } else {
              // cout << "but Inactive" << endl;
            }
          } else {
            // neighbor = insertCell(cells, kInactive, nX, nY, nZ);
          }
        }
        // cout << "\tWith " << activeNeighbors << " neighbor(s). From: '" << cell->getValue() << "' to '";
        if (cell->isActive()) {
          if (activeNeighbors < 2 || activeNeighbors > 3) {
            cell->setValue(kInactive);
            // cout << kInactive << "'" << endl;
          }  else {
            // cout << kActive << "'" << endl;
          }
        } else if (activeNeighbors == 3) {
          cell->setValue(kActive);
          // cout << kActive << "'" << endl;
        } else {
          // cout << kInactive << "'" << endl;
          }
      }
    }
  }
}

 size_t tickIt(unordered_map<string, Cell> &cells, size_t iteration) {
  updateIteratively(cells, iteration);
  size_t active = getActiveCells(cells);
  printCells(cells, iteration);
  cout << "Cells created: " << cells.size() << ", Active: " << active << endl;
  return active;
 }

void solve1() {
  unordered_map<string,Cell> cells;
  string input;
  int y = 0;
  while(!cin.eof()) {
    getline(cin, input);
    for (int i = 0; i < input.size(); ++i) {
      insertCell(cells, input[i], i, y, 0);
    }
    ++y;
  }
  kMax = input.size();
  printCells(cells);
  size_t active = getActiveCells(cells);
  cout << "Cells created: " << cells.size() << ", Active: " << active << endl;
  for (int i = 1; i <= kMaxIt; ++i) {
    tickIt(cells, i);
  }
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } 
}

};  // aoc2020_17

#endif /* _2020_ADVENTOFCODE_17_H_ */
