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
using namespace std;

class Player {
public:
  int id;
  deque<int> cards;

  string toString() const {
    stringstream ss;
    ss << "p" << id << ":";
    for (int i = 0 ; i < cards.size(); ++i) {
      ss << (i == 0 ? ":[" : ",") << cards[i];
    }
    ss << "]";
    return move(ss.str());
  }

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

  string toString() const {
    stringstream ss;
    ss << p1.toString() << "-" << p2.toString();
    return move(ss.str());
  }

  static Data fromInput() {
    Data data;
    Player *p = &data.p1;
    string line;
    while (!cin.eof()) {
      getline(cin, line);
      p->id = util::getNumber(line, 7);
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

pair<int,Player*> play(Data &data) {
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
  // Return the number of rounds and the winner player
  return make_pair(i - 1, (data.p1.cards.size() ? &data.p1 : &data.p2));
}

void onlyKeepNCards(Player &p, int n) {
  while (p.cards.size() > n) {
    p.cards.pop_back();
  }
}

pair<int,Player*> playRecursive(Data &data, int depth = 1) {
  unordered_set<string> seenGames;
  int i = 1;
  for (; i < 1000 && data.p1.cards.size() && data.p2.cards.size(); ++i) {
    string p1Deck = data.p1.toString(), p2Deck = data.p2.toString();
    if (seenGames.find(p1Deck) != seenGames.end() ||
        seenGames.find(p2Deck) != seenGames.end()) {
      // Game ends, P1 wins automatically.
      return make_pair(i - 1, &data.p1);
    } else {
      seenGames.insert(p1Deck);
      seenGames.insert(p2Deck);
    }
    int p1Card = data.p1.cards.front(); data.p1.cards.pop_front();
    int p2Card = data.p2.cards.front(); data.p2.cards.pop_front();

    if (p1Card <= data.p1.cards.size() && p2Card <= data.p2.cards.size()) {
      Data dataCopy = data;
      onlyKeepNCards(dataCopy.p1, p1Card);
      onlyKeepNCards(dataCopy.p2, p2Card);
      auto subGameResult = playRecursive(dataCopy, depth + 1);
      if (subGameResult.second->id == data.p1.id) {  // P1 won subgame
        data.p1.cards.push_back(p1Card);
        data.p1.cards.push_back(p2Card);
      } else {                                       // P2 won subgame
        data.p2.cards.push_back(p2Card);
        data.p2.cards.push_back(p1Card);
      }
    } else if (p1Card > p2Card) {
      data.p1.cards.push_back(p1Card);
      data.p1.cards.push_back(p2Card);
    } else {
      data.p2.cards.push_back(p2Card);
      data.p2.cards.push_back(p1Card);
    }
    // cout << "\tAfter " << i << " round(s): " << endl; data.print();
  }
  // Return the number of rounds and the winner player
  return make_pair(i - 1, (data.p1.cards.size() ? &data.p1 : &data.p2));
}

void solve(int part = 1) {
  Data data = Data::fromInput();
  data.print();
  pair<int,Player*> gameResult;
  if (part == 1) {
    gameResult = play(data);
  } else {
    gameResult = playRecursive(data);
  }
  cout << "After " << gameResult.first << " rounds the winner is player ";
  cout << gameResult.second->id << endl;
  gameResult.second->print();
  // data.print(); 
}

};  // aoc2020_22

#endif /* _2020_ADVENTOFCODE_22_H_ */
