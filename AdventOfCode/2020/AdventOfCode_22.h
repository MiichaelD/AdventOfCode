/*
  Link:         http://adventofcode.com/2020/day/22
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/22/2020
*/

#ifndef _2020_ADVENTOFCODE_22_H_
#define _2020_ADVENTOFCODE_22_H_

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

namespace aoc2020_22 {
using namespace std;int getNumber(const string &line, int index) {
  int accum = 0;
  for (; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ':') {
      break;
    }
  }
  return accum;
}

class Player {
public:
  int id;
  deque<int> cards;

  uint64_t getScore() const {
    int i = 0;
    uint64_t score = 0;
    for (auto it = cards.rbegin(); it != cards.rend(); ++it) {
      score += (*it * ++i);
    }
    return score;
  }

  void print() const {
    cout << "Player " << id << " w/ score: " << getScore() << endl;
    for (int n : cards) {
      cout << "    " << n << endl;
    }
    cout << endl;
  }
};

class Data {
public:
  Player p1, p2;

  void print() const {
    p1.print();
    p2.print();
  }

  static Data fromInput() {
    Data data;
    Player *p = &data.p1;
    string line;
    while (!cin.eof()) {
      getline(cin, line);
      p->id = getNumber(line, 7);
      while(!cin.eof()) {
        getline(cin, line);
        if (line.empty()) {
          break;
        }
        p->cards.push_back(atoi(line.c_str()));
      }
      if (p == &data.p2) {
        break;
      } else {
        p = &data.p2;
      }
    }
    return data;
  }
};

int play(Data &data) {
  int i = 1;
  for (; data.p1.cards.size() && data.p2.cards.size(); ++i) {
    int p1Card = data.p1.cards.front(); data.p1.cards.pop_front();
    int p2Card = data.p2.cards.front(); data.p2.cards.pop_front();

    if (p1Card > p2Card) {
      data.p1.cards.push_back(p1Card);
      data.p1.cards.push_back(p2Card);
    } else {
      data.p2.cards.push_back(p2Card);
      data.p2.cards.push_back(p1Card);
    }
    // cout << "\tAfter " << i << " round(s): " << endl; data.print();
  }
  return i - 1;
}

void solve1() {
  Data data = Data::fromInput();
  data.print(); 
  cout << "After " << play(data) << " rounds ... " << endl;
  data.print(); 
}

void solve2() {
  string input;
  cin >> input;
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } else {
    solve2();
  }
}

};  // aoc2020_22

#endif /* _2020_ADVENTOFCODE_22_H_ */
