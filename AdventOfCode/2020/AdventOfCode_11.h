/*
  Link:         http://adventofcode.com/2020/day/11
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/11/2020
*/

#ifndef _2020_ADVENTOFCODE_11_H_
#define _2020_ADVENTOFCODE_11_H_

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

namespace aoc2020_11 {
using namespace std;

constexpr char kAvailable = 'L';
constexpr char kOccupied = '#';
constexpr char kFloor = '.';

class Cell {
public:
 Cell(char c) : currentState{c}, futureState{c} { }

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

 inline int isAvailableNum() const {
   return isAvailable() ? 1 : 0;
 }

 inline bool isAvailable() const {
   return currentState == kAvailable;
 }

 inline bool isOccupied() const {
   return currentState == kOccupied;
 }

 inline bool isFloorTile() const {
   return currentState == kFloor;
 }

private:
  char futureState = kFloor;
  char currentState = kFloor;
};

typedef vector<vector<Cell>> Matrix;

class GameOfLife {

public:
  GameOfLife() : run{false} {
    string word;
    while(!cin.eof()) {
      cin >> word;
      grid.emplace_back(vector<Cell>(word.size(), Cell(kFloor)));
      for (int i = 0 ; i < word.size(); ++i) {
        grid.back()[i].setValue(word[i]);
      }
    }
    maxY = grid.size();
    maxX = grid.back().size();
    cout << "Grid size: " << maxX  << " x " << maxY << endl;
  }

  bool hasPrevious(size_t x) const {
    return x > 0;
  }
  
  bool hasNextX(size_t x) const {
    return x < (maxX - 1);
  }
  bool hasNextY(size_t y) const {
    return y < (maxY - 1);
  }
  
  size_t getPreviousX(size_t x) const {
    return hasPrevious(x) ? x - 1 : maxX - 1;
  }
  
  size_t getPreviousY(size_t y) const {
    return hasPrevious(y) ? y - 1 : maxY - 1;
  }
  
  size_t getNextX(size_t x) const {
    return hasNextX(x) ?  x + 1 : 0;
  }
  
  size_t getNextY(size_t y) const {
    return hasNextY(y) ? y + 1 : 0;
  }
  
  pair<size_t,size_t> getNeighborsCount(size_t f, size_t c) const{
    size_t availableNeighborsCount = 0;
    size_t occupiedNeighborsCount = 0;
    size_t prevC = getPreviousX(c);
    size_t prevF = getPreviousY(f);
    size_t nextC = getNextX(c);
    size_t nextF = getNextY(f);
    if (hasPrevious(f)) {
      availableNeighborsCount += grid[prevF][c].isAvailableNum(); // UP
      occupiedNeighborsCount += grid[prevF][c].isOccupied() ? 1 : 0;
      if (hasPrevious(c)) {
        availableNeighborsCount += grid[prevF][prevC].isAvailableNum(); // UP LEFT
        occupiedNeighborsCount += grid[prevF][prevC].isOccupied() ? 1 : 0; // UP LEFT
      }
      if (hasNextX(c)) {
        availableNeighborsCount += grid[prevF][nextC].isAvailableNum(); // UP RIGHT
        occupiedNeighborsCount += grid[prevF][nextC].isOccupied() ? 1 : 0; // UP RIGHT
      }
    }
    if (hasPrevious(c)) {
      availableNeighborsCount += grid[f][prevC].isAvailableNum(); // LEFT
      occupiedNeighborsCount += grid[f][prevC].isOccupied() ? 1 : 0; // LEFT
    }
    if (hasNextX(c)) {
      availableNeighborsCount += grid[f][nextC].isAvailableNum(); // RIGHT
      occupiedNeighborsCount += grid[f][nextC].isOccupied() ? 1 : 0; // RIGHT
    }
    if (hasNextY(f)) {
      availableNeighborsCount += grid[nextF][c].isAvailableNum(); // DOWN
      occupiedNeighborsCount += grid[nextF][c].isOccupied() ? 1 : 0; // DOWN
      if (hasPrevious(c)) {
        availableNeighborsCount += grid[nextF][prevC].isAvailableNum(); // DOWN LEFT
        occupiedNeighborsCount += grid[nextF][prevC].isOccupied() ? 1 : 0; // DOWN LEFT
      }
      if (hasNextX(c)) {
        availableNeighborsCount += grid[nextF][nextC].isAvailableNum(); // DOWN RIGHT
        occupiedNeighborsCount += grid[nextF][nextC].isOccupied() ? 1 : 0; // DOWN RIGHT
      }
    }
    return make_pair(availableNeighborsCount, occupiedNeighborsCount);
  }

size_t tick(){
  cout << "\tTicking. ";
  availableSeats = 0;
  floorCount = 0;
  occupiedSeats = 0;
  for (size_t f = 0; f < maxY; ++f){
    for (size_t c = 0; c < maxX; ++c){
      pair<size_t, size_t> neighbors = getNeighborsCount(f, c);
      Cell *currentCell = &grid[f][c];
      /*
      If a seat is empty (L) and there are no occupied seats adjacent to it, the seat becomes occupied.
      If a seat is occupied (#) and four or more seats adjacent to it are also occupied, the seat becomes empty.
      */
      if (currentCell->isFloorTile()) {
        ++floorCount;
        continue;
      }
      if (currentCell->isAvailable()) {
        if (neighbors.second == 0) {
          currentCell->setValue(kOccupied);
          ++occupiedSeats;
        } else {
          ++availableSeats;
        }
      } else if (currentCell->isOccupied()) {
        if (neighbors.second >= 4) {
          currentCell->setValue(kAvailable);
          ++availableSeats;
        } else {
          ++occupiedSeats;
        }
      } 
      // Otherwise, the seat's state does not change.
    }
  }
  size_t total = availableSeats + occupiedSeats + floorCount;
  cout << "Available: " << availableSeats << ", Occupied: " << occupiedSeats
      << ", Floor tiles: " << floorCount << ". Total: " <<  total << endl;
  return availableSeats;
}
  
  bool print() {
    size_t changes = 0;
    for (size_t f = 0; f < maxY; ++f){
      for (size_t c = 0; c < maxX; ++c){
        Cell *currentCell = &grid[f][c];
        changes += currentCell->commit() ? 1 : 0;
        cout << currentCell->getValue();
      }
      cout << endl;
    }
    cout << "Changes: " << changes << endl << endl;
    return changes != 0;
  }

  inline size_t getAvailableSeatCount() const {
    return availableSeats;
  }

private:
  size_t maxY, maxX, maxTicks;
  bool run;
  Matrix grid;
  size_t availableSeats, occupiedSeats, floorCount;
};

void solve1() {
  GameOfLife game;
  game.print();
  cout << endl;
  game.print();
  cout << endl;
  for (int i = 0; true; ++i) {
    cout << i << ":\t";
    game.tick();
    if (!game.print()) {
      cout << "Available seats at stabilization: " << game.getAvailableSeatCount() << endl;
      break;
    }
  }
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } 
}

};  // aoc2020_11

#endif /* _2020_ADVENTOFCODE_11_H_ */
