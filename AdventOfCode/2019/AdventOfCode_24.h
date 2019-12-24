
/*
  Link:         http://adventofcode.com/2019/day/24
  Description:  Based on: https://github.com/MiichaelD/c_cpp/blob/master/tc/GameOfLife.cpp
                Game Of Life: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/24/2019
*/

#ifndef _2019_ADVENTOFCODE_24_H_
#define _2019_ADVENTOFCODE_24_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <stdint.h>
#include <tuple>
#include <unistd.h> // usleep(microseconds);
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc2019_24 {  
  using namespace std;

  const size_t MAX_GRID_SIZE = 100;
const size_t SLEEP_MICROSECS = 0; //500000;

class Cell{
private:
  bool m_futureState = false;
  bool m_currentState = false;
  
public:
  void commit() {
    m_currentState = m_futureState;
  }
  
  void setAlive(bool alive, bool commit = false){
    m_futureState = alive;
    if (commit)
      m_currentState = alive;
  }
  
  inline bool isAlive() const {
    return m_currentState;
  }
  
  inline int8_t isAliveNum() const {
    return isAlive() ? 1 : 0;
  }
  
  inline bool willBeAlive() const {
    return m_futureState;
  }
  
  inline int8_t willBeAliveNum() const {
    return willBeAlive() ? 1 : 0;
  }
};

class GameOfLife{
private:
  size_t ticks = 0;
  size_t m_maxX;
  size_t m_maxY;
  size_t m_maxTicks;
  bool m_run = false;
  unordered_set<size_t> bioRatingsSet;
  
  std::unique_ptr<Cell> m_board[MAX_GRID_SIZE][MAX_GRID_SIZE];
  
  inline size_t getPreviousX(size_t x) const {
    return x == 0 ? m_maxX-1 : x-1;
  }
  
  inline size_t getPreviousY(size_t y) const {
    return y == 0 ? m_maxY-1 : y-1;
  }
  
  inline size_t getNextX(size_t x) const {
    return x == m_maxX-1 ? 0 : x+1;
  }
  
  inline size_t getNextY(size_t y) const {
    return y == m_maxY-1 ? 0 : y+1;
  }
  
  int getLiveNeighborsCount(size_t f, size_t c, bool cyclic = true) const{
    int liveNeighbors = 0;
    if (cyclic) {
      size_t prevF = getPreviousX(f);
      size_t prevC = getPreviousY(c);
      liveNeighbors += m_board[prevF][c]->isAliveNum(); // UP
      liveNeighbors += m_board[prevF][prevC]->isAliveNum(); // UP LEFT
      liveNeighbors += m_board[f][prevC]->isAliveNum(); // LEFT
      
      size_t nextF = getNextX(f);
      size_t nextC = getNextY(c);
      liveNeighbors += m_board[nextF][prevC]->isAliveNum(); // DOWN LEFT
      liveNeighbors += m_board[nextF][c]->isAliveNum(); // DOWN
      liveNeighbors += m_board[nextF][nextC]->isAliveNum(); // DOWN RIGHT
      liveNeighbors += m_board[f][nextC]->isAliveNum(); // RIGHT
      liveNeighbors += m_board[prevF][nextC]->isAliveNum(); // UP RIGHT
    } else {
      liveNeighbors += f == 0 ? 0 :  m_board[f-1][c]->isAliveNum(); // UP
      liveNeighbors += c == 0 ? 0 :  m_board[f][c-1]->isAliveNum(); // LEFT
      liveNeighbors += f == m_maxY - 1 ? 0 :  m_board[f+1][c]->isAliveNum(); // DOWN
      liveNeighbors += c == m_maxX - 1 ? 0 :  m_board[f][c+1]->isAliveNum(); // RIGHT
    }
    return liveNeighbors;
  }
  
  size_t tick(bool cyclic = true){
    size_t aliveCells = 0;
    for (size_t f= 0; f < m_maxY; ++f){
      for (size_t c= 0; c < m_maxX; ++c){
        int liveNeighbors = getLiveNeighborsCount(f, c, cyclic);
        Cell *currentCell = m_board[f][c].get();
        if (cyclic) {
          switch(liveNeighbors) {
          case 2:   // if 2 or 3, stay as they are.
            aliveCells += currentCell->isAliveNum();
            break;
          case 3:  // if exactly 3 become alive by repoduction
            currentCell->setAlive(true);
            ++aliveCells;
            break;
          default:
            currentCell->setAlive(false);  // if < 2 die by underpopulation if > 3 die by overpopulation
            break;
          }
        } else {
          if (currentCell->isAlive()) {
            currentCell->setAlive(liveNeighbors == 1);
            aliveCells += liveNeighbors == 1 ? 1 : 0;
          } else {
            currentCell->setAlive(liveNeighbors == 1 || liveNeighbors == 2);
            aliveCells += liveNeighbors == 1 || liveNeighbors == 2 ? 1 : 0;
          }
        }
      }
    }
    return aliveCells;
  }

  // Returns whether the current board is unique (not seen in any generation before).
  bool commit() {
    for (size_t f= 0; f < m_maxX; ++f){
      for (size_t c= 0; c < m_maxY; ++c){
        m_board[f][c]->commit();
      }
    }
    size_t bioRating = getBiodiversityRating();
    if (bioRatingsSet.find(bioRating) == bioRatingsSet.end()) {
      bioRatingsSet.insert(bioRating);
      return true;
    }
    return false;
    
  }
  
public:
  GameOfLife(size_t sizeX, size_t sizeY, size_t ticks = SIZE_MAX)
      :m_maxX(sizeX), m_maxY(sizeY), m_maxTicks(ticks){
    for (size_t f= 0; f < m_maxX ; ++f){
      for (size_t c= 0; c < m_maxY ; ++c){
        m_board[f][c].reset(new Cell);
      }
    }
  }
  
  void setAlive(size_t f, size_t c, bool alive = true){
    m_board[f][c]->setAlive(alive, true);
  }

  size_t getBiodiversityRating() const {
    size_t result = 0;
    for (size_t f= 0; f < m_maxX; ++f) {
      for (size_t c= 0; c < m_maxY; ++c) {
        if (m_board[f][c]->isAlive()) {
          result += 1 << (f * 5 + c);
        }
      }
    }
    return result;
  }

  size_t getTickNumber() const {
    return ticks;
  }
  
  void start(bool cyclic = true) {
    m_run = true;
    ticks = 0;
    cout << "Starting with: " << endl;
    commit();
    print();
    while(m_run){
      size_t aliveCells = tick(cyclic);
      cout << "Generation: # " << ++ticks << ": \t Alive: " <<  aliveCells << endl;
      bool uniqueBioDiversity = commit();
      print();
      if (ticks == m_maxTicks || !uniqueBioDiversity) {
        stop();
      }
    }
  }
  
  void stop() {
    m_run = false;
  }
  
  void print(){
    for (size_t f= 0; f < m_maxX; ++f){
      for (size_t c= 0; c < m_maxY; ++c){
        Cell *currentCell = m_board[f][c].get();
        cout << (currentCell->isAlive() ? '#' : '.');
      }
      cout << endl;
    }
    cout << endl;
    usleep(SLEEP_MICROSECS);
  }
  
};

void solve(int part = 1) {
  GameOfLife life(5, 5);
  string input;
  int line = -1;
  while(!cin.eof()) {
    cin >> input;
    ++line;
    for (int i = 0; i < input.size(); ++i) {
      if (input[i] == '#') {
        life.setAlive(line, i);
      }
    }
  }
  life.start(false);
  cout << "1st Repeated Biodiversity Rating: " << life.getBiodiversityRating()
       << ", at tick #" << life.getTickNumber() << endl;
}
};

#endif /* _2019_ADVENTOFCODE_24_H_ */
