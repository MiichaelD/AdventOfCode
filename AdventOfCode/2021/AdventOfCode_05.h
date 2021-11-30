/*
  Link:         http://adventofcode.com/2020/day/5
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/05/2019
*/

#ifndef _2021_ADVENTOFCODE_05_H_
#define _2021_ADVENTOFCODE_05_H_

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

namespace aoc2021_05 {  
using namespace std;


class BiSearch {
public:
  BiSearch(size_t start, size_t end) {
    reset(start, end);
  }

  void update(char c) {
    // cout << "processing: "<< c<< endl;
    size_t mid = (start + end) / 2;
    if (c == 'F' || c == 'L') {
      end = mid; 
    } else if (c == 'B' || c == 'R') {
      start = mid;
    } else {
      cout << "\tThis shouldn't happen" << endl;
    }
  }

  void reset(size_t start, size_t end) {
    this->start = start;
    this->end = end;
  }

  size_t getResult() {
    return start;
  }

  void print() {
    cout << "Binary: " << start << " ==> " << end << endl;
  }

private:
  // Start is inclusive, End is exclusive.
  size_t start,end;  
};

pair<int,int> parseInput(const string &line) {
  pair<int,int> result;
  BiSearch seatSearch(0, 128);
  for (int i = 0; i < 7; ++i) {
    seatSearch.update(line[i]);

  }
  result.first = seatSearch.getResult();
  seatSearch.reset(0, 8);
  for (int i = 7; i < line.length(); ++i) {
    seatSearch.update(line[i]);
  }
  result.second = seatSearch.getResult();
  return result;
}

inline size_t getSeatId(const pair<int,int> &seat) {
  return seat.first * 8 + seat.second;
}

size_t getMissingSeat(const vector<bool> &seats, size_t maxSeatIndex) {
  for (int i = 1; i <= maxSeatIndex; ++i) {
    if (!seats[i] && seats[i - 1] && seats[i + 1]) {
      return i;
    }
  }
  return 0;
}

inline void printSeatPosition(size_t seatId) {
  cout << " @ [" << seatId / 8 << "," << seatId % 8 << "]" << endl;
}

void solve(int part = 1) {
  string line;
  size_t maxSeatId = 0;
  vector<bool> seats(128*8);
  while (!cin.eof()) {
    getline(cin, line);
    if (line.empty()) {
      break;
    }

    pair<int,int> seat = parseInput(line);
    size_t seatId = getSeatId(seat);
    seats[seatId] = true;

    if (seatId > maxSeatId) {
      maxSeatId = seatId;
    }
    // util::printPair(seat); cout << "Seat ID: " << seatId << endl;
  }
  cout << "Part 1 - Max seat ID: " << maxSeatId; printSeatPosition(maxSeatId);

  size_t mySeatId = getMissingSeat(seats, maxSeatId);
  cout << "Part 2 - My seat ID:  " << mySeatId; printSeatPosition(mySeatId);
}

};

#endif /* _2021_ADVENTOFCODE_05_H_ */