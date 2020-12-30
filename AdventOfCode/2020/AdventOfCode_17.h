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
const int kMaxIt = 6;
const int kMaxNeigh = 3;
int kMax = 8;

struct Limits {
  int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
  int minZ = INT_MAX, maxZ = INT_MIN, minW = INT_MAX, maxW = INT_MIN;
  void expand() {
    --minX; --minY; --minZ; --minW;
    ++maxX; ++maxY; ++maxZ; ++maxW;
  }
};

class Cell {
public:
  Cell(char c) : currentState{c}, futureState{c}, x{0}, y{0}, z{0}, w{0}  { }
  Cell(char c, int xx, int yy, int zz, int ww)
      : currentState{c}, futureState{c}, x{xx}, y{yy}, z{zz}, w{ww} {
        // cout << "Cell('" << c <<"' @ " << x << ',' << y << ',' << z << ')' << endl;
        // cout << "Created '" << toString() << "'" << endl;
      }

  static string computeId(int x, int y, int z, int w) {
    stringstream output;
    output << x << "," << y << "," << z << "," << w;
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

 tuple<int,int,int,int> getPosition() const {
   return {x, y, z, w};
 }

 vector<tuple<int, int, int, int>> getNeighbors() const {
   vector<tuple<int, int, int, int>> result;
   for (int i = -1; i < kMaxNeigh - 1; ++i) {
     for (int j = -1; j < kMaxNeigh - 1; ++j) {
        for (int k = -1; k < kMaxNeigh - 1; ++k) {
          if (i == 0 && j == 0 && k == 0) {
            continue;
          }
          result.emplace_back(x - i, y - j, z - k, 0);
        }
      }
   }
   assert(result.size() == 26);
   return result;
 }

 vector<tuple<int, int, int, int>> getNeighbors4() const {
   static const int kMaxNeigh = 3;
   vector<tuple<int, int, int, int>> result;
   for (int i = -1; i < kMaxNeigh - 1; ++i) {
     for (int j = -1; j < kMaxNeigh - 1; ++j) {
        for (int k = -1; k < kMaxNeigh - 1; ++k) {
          for (int l = -1; l < kMaxNeigh - 1; ++l) {
            if (i == 0 && j == 0 && k == 0 && l == 0) {
              continue;
            }
            result.emplace_back(x - i, y - j, z - k, w - l);
          }
        }
      }
   }
   assert(result.size() == 80);
   return result;
 }

  bool operator==(const Cell &other) const {
    return (x == other.getX() && y == other.getY()
            && z == other.getZ() && w == other.getW());
  }

  size_t operator () (Cell const &other) const {
    auto p = other.getPosition();
    return get<0>(p) ^ get<1>(p) ^ get<2>(p) ^ get<3>(p);
  }

  const string& toString() const {
    if (cachedStr.empty()) {
      cachedStr = computeId(x, y, z, w);
    }
    return cachedStr;
  }

 inline int getX() const { return x; }
 inline int getY() const { return y; }
 inline int getZ() const { return z; }
 inline int getW() const { return w; }

private:
  char futureState = kInactive;
  char currentState = kInactive;
  int x, y, z, w;
  mutable string cachedStr = "";
};

//http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
struct CellHasher {
  std::size_t operator()(const Cell& c) const {
    return hash<string>()(c.toString());
  }
};

Limits getLimits(const unordered_map<string, Cell> &cells, bool expand = true) {
  Limits r;
  for (const auto &entry : cells) {
    const auto &cell = entry.second;
    if (!cell.isActive()) continue;
    if (cell.getX() < r.minX) {r.minX = cell.getX();}
    if (cell.getY() < r.minY) {r.minY = cell.getY();}
    if (cell.getZ() < r.minZ) {r.minZ = cell.getZ();}
    if (cell.getW() < r.minW) {r.minW = cell.getW();}
    if (cell.getX() > r.maxX) {r.maxX = cell.getX();}
    if (cell.getY() > r.maxY) {r.maxY = cell.getY();}
    if (cell.getZ() > r.maxZ) {r.maxZ = cell.getZ();}
    if (cell.getW() > r.maxW) {r.maxW = cell.getW();}
  }
  if (expand) r.expand();
  return r;
}

bool isCellActive(const unordered_map<string,Cell> &cells, const string &id) {
  const auto cell = cells.find(id);
  return cell != cells.end() && cell->second.isActive();
}

Cell* getCell(unordered_map<string,Cell> &cells, const string &id) {
  auto cellIt = cells.find(id);
  return cellIt != cells.end() ? &(cellIt->second) : nullptr;
}

Cell* insertCell(
  unordered_map<string,Cell> &cells, char c, int x, int y, int z, int w) {
  Cell cell(c, x, y, z, w);
  auto res = cells.insert({cell.toString(), cell});
  return &(res.first->second);
}

void printCells(unordered_map<string,Cell> &cells) {
  Limits limits = getLimits(cells, false);
  cout << "printing: x from " <<  limits.minX << ", to " << limits.maxX;
  cout << ". y from " <<  limits.minY << ", to " << limits.maxY;
  cout << ". z from " << limits.minZ << ", to " << limits.maxZ;
  cout << ". w from " << limits.minW << ", to " << limits.maxW << endl;
  for (int l = limits.minW; l <= limits.maxW; ++l) {
    cout << "W: " << l << endl;
    for (int k = limits.minZ; k <= limits.maxZ ; ++k) {
      cout << "Z: " << k << endl;
      for (int j = limits.minY; j <= limits.maxY; ++j) {
        for (int i = limits.minX; i <= limits.maxX; ++i) {
          const Cell *c = getCell(cells, Cell::computeId(i,j,k,l));
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

void updateIteratively(
    unordered_map<string, Cell> &cells, bool d4) {
  Limits limits = getLimits(cells);
  if (!d4) {limits.minW = 0; limits.maxW = 0;}
  cout << "Updating: x from " <<  limits.minX << ", to " << limits.maxX;
  cout << ". y from " <<  limits.minY << ", to " << limits.maxY;
  cout << ". z from " << limits.minZ << ", to " << limits.maxZ;
  cout << ". w from " << limits.minW << ", to " << limits.maxW << endl;

  for (int l = limits.minW; l <= limits.maxW; ++l) {
    for (int k = limits.minZ; k <= limits.maxZ ; ++k) {
      for (int j = limits.minY; j <= limits.maxY; ++j) {
        for (int i = limits.minX; i <= limits.maxX; ++i) {
          string cellId = Cell::computeId(i,j,k,l);
          Cell *cell = getCell(cells, cellId);
          if (cell == nullptr) {
            cell = insertCell(cells, kInactive, i, j, k, l);
          }
          const auto neighbors = d4 ? cell->getNeighbors4() : cell->getNeighbors();
          size_t activeNeighbors = 0; 
          for (const auto &n : neighbors) {
            const int nX = get<0>(n), nY = get<1>(n), nZ =get<2>(n), nW =get<3>(n); 
            const string nId = Cell::computeId(nX, nY, nZ, nW);
            activeNeighbors += isCellActive(cells, nId) ? 1 : 0;
          }
          if (cell->isActive()) {
            if (activeNeighbors < 2 || activeNeighbors > 3) {
              cell->setValue(kInactive);
            }
          } else if (activeNeighbors == 3) {
            cell->setValue(kActive);
          }
        }
      }
    }
  }
}

 size_t tickIt(unordered_map<string, Cell> &cells, bool d4) {
  updateIteratively(cells, d4);
  size_t active = getActiveCells(cells);
  if (!d4) printCells(cells);
  cout << "Cells created: " << cells.size() << ", Active: " << active << endl;
  return active;
 }

void solve(int part = 1) {
  bool d4 = part != 1;
  unordered_map<string,Cell> cells;
  string input;
  int y = 0;
  while(!cin.eof()) {
    getline(cin, input);
    if (input.empty()) break;
    for (int i = 0; i < input.size(); ++i) {
      insertCell(cells, input[i], i, y, 0, 0);
    }
    ++y;
  }
  kMax = input.size();
  printCells(cells);
  size_t active = getActiveCells(cells);
  cout << "Cells created: " << cells.size() << ", Active: " << active << endl;
  for (int i = 1; i <= kMaxIt; ++i) {
    cout << "Iteration: " << i << endl;
    tickIt(cells, d4);
  }
}

};  // aoc2020_17 - test: 112 - pt 1: 252

#endif /* _2020_ADVENTOFCODE_17_H_ */
