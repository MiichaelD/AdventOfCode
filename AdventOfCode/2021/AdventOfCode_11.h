/*
  Link:         http://adventofcode.com/2020/day/11
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/11/2021
*/

#ifndef _2021_ADVENTOFCODE_11_H_
#define _2021_ADVENTOFCODE_11_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
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

namespace aoc2021_11 {
using namespace std;


class Cell {
public:
 Cell(int c = 0) : currentState{c}, futureState{c} { }

 // Returns whether the cell flashed.
 inline bool commit() {
   bool result = flashed;
   currentState = flashed ? 0 : futureState;
   futureState = currentState;
   flashed = false;  // Reset flash state.
   return result;
 }

 inline int getValue() const {
   return currentState;
 }

 inline void setValue(int state) {
   futureState = state;
 }

 // Returns whether the cell flashed or not.
 inline bool onTick() {
   // Increment future state by one
   if (++futureState == 10) {
      flashed = true;
      return true;
   };
   return false;
 }

 inline bool onNeighborFlash() {
   return onTick();
 }

private:
  int futureState = 0;
  int currentState = 0;
  bool flashed = false;
};

typedef vector<vector<Cell>> Matrix;
const size_t kMatrixSize = 10;

class GameOfLife {

public:
  GameOfLife() : run{false}, maxY{kMatrixSize}, maxX{kMatrixSize}, totalFlashes {0} {
    int num = 0;
    string line;
    while(!cin.eof()) {
      grid.reserve(kMatrixSize);
      for (int i = 0; i < kMatrixSize; ++i) {
        grid.emplace_back(vector<Cell>(kMatrixSize));
        cin >> line;
        for (int j = 0 ; j <line.size(); ++j) {
          grid.back()[j].setValue(line[j] - '0');
          grid.back()[j].commit();
          cout << grid.back()[j].getValue();
        }
        cout << endl;
      }
    }
    cout << "Grid size: " << maxX  << " x " << maxY << endl;
  }

  inline bool hasPrevious(size_t x) const {
    return x > 0;
  }
  
  inline bool hasNextX(size_t x) const {
    return x < (maxX - 1);
  }
  inline bool hasNextY(size_t y) const {
    return y < (maxY - 1);
  }
  
  inline size_t getPreviousX(size_t x) const {
    return hasPrevious(x) ? x - 1 : maxX - 1;
  }
  
  inline size_t getPreviousY(size_t y) const {
    return hasPrevious(y) ? y - 1 : maxY - 1;
  }
  
  inline size_t getNextX(size_t x) const {
    return hasNextX(x) ?  x + 1 : 0;
  }
  
  inline size_t getNextY(size_t y) const {
    return hasNextY(y) ? y + 1 : 0;
  }
  
  // Returns the amount of neighbors that flashed.
  size_t onNeighborFlash(vector<pair<int,int>> &flashedCellIndeces, size_t f, size_t c) {
    size_t prevC = getPreviousX(c);
    size_t prevF = getPreviousY(f);
    size_t nextC = getNextX(c);
    size_t nextF = getNextY(f);
    size_t flashes = 0;
    if (hasPrevious(f)) {
      if (grid[prevF][c].onNeighborFlash()) {// UP
        ++flashes;
        flashedCellIndeces.push_back({prevF, c});
      } 
      if (hasPrevious(c)) {
        if (grid[prevF][prevC].onNeighborFlash()) {// UP LEFT
          ++flashes;
          flashedCellIndeces.push_back({prevF, prevC});
        } 
      }
      if (hasNextX(c)) {
        if (grid[prevF][nextC].onNeighborFlash()) {// UP RIGHT
          ++flashes;
          flashedCellIndeces.push_back({prevF, nextC});
        } 
      }
    }
    if (hasPrevious(c)) {
      if (grid[f][prevC].onNeighborFlash()) {// LEFT
        ++flashes;
        flashedCellIndeces.push_back({f, prevC});
      }
    }
    if (hasNextX(c)) {
      if (grid[f][nextC].onNeighborFlash()) {// RIGHT
        ++flashes;
        flashedCellIndeces.push_back({f, nextC});
      }
    }
    if (hasNextY(f)) {
      if (grid[nextF][c].onNeighborFlash()) {// DOWN
        ++flashes;
        flashedCellIndeces.push_back({nextF, c});
      }
      if (hasPrevious(c)) {
        if (grid[nextF][prevC].onNeighborFlash()) {// DOWN LEFT
          ++flashes;
          flashedCellIndeces.push_back({nextF, prevC});
        }
      }
      if (hasNextX(c)) {
        if (grid[nextF][nextC].onNeighborFlash()) {// DOWN RIGHT
          ++flashes;
          flashedCellIndeces.push_back({nextF, nextC});
        }
      }
    }
    return flashes;
  }

  size_t tick(bool longSight = false){
    size_t flashes = 0;
    vector<pair<int,int>> flashedCellIndeces;
    // Tick
    for (size_t f = 0; f < maxY; ++f){
      for (size_t c = 0; c < maxX; ++c){
        Cell *currentCell = &grid[f][c];
        if (currentCell->onTick()) {
          ++flashes;
          cout << "\tf: " << f << ", c: " << c << " flashed" << endl;
          flashedCellIndeces.push_back({f,c});
        }
      }
    }
    // Process Flashes
    for (int i = 0; i < flashedCellIndeces.size(); ++i){
      const pair<int,int> &entry = flashedCellIndeces[i];
      // Cell *currentCell = &grid[entry.first][entry.second];
      flashes += onNeighborFlash(flashedCellIndeces, entry.first, entry.second);
    }
    totalFlashes += flashes;
    return flashes;
  }

  void printStatus() {
    cout << "Total flashes: " << totalFlashes << endl;
  }

  size_t getTotalFlashes() const {
    return totalFlashes;
  } 
  
  size_t commit(bool print = false) {
    size_t changes = 0;
    for (size_t f = 0; f < maxY; ++f){
      for (size_t c = 0; c < maxX; ++c){
        Cell *currentCell = &grid[f][c];
        changes += currentCell->commit() ? 1 : 0;
        if (print) cout << currentCell->getValue();
      }
      if (print) cout << endl;
    }
    if (print) {
      cout << "Flashes: " << changes << "\t"; printStatus();
      cout << endl;
    }
    return changes;
  }


private:
  size_t maxY, maxX, maxTicks;
  size_t totalFlashes;
  bool run;
  Matrix grid;
};


void solve(int part = 1) {
  GameOfLife game;
  size_t totalFlashesAt100 = 0;
  size_t stepAllFlashes = 0;
  for (int i = 1; true; ++i) {
    // cout << i << ":\t";
    game.tick(part != 1);
    cout << "Step: " << i << endl;
    if (game.commit(true) == 100) {
      stepAllFlashes = i;
      break;
    }
    if (i == 100) {
      totalFlashesAt100 = game.getTotalFlashes();
    }
  }
  cout << "Part 1: Total flashes at step 100: " << totalFlashesAt100 << endl;
  cout << "Part 2: All octopus flash at step: " << stepAllFlashes << endl;
}

};  // aoc2021_11

#endif /* _2021_ADVENTOFCODE_11_H_ */
