/*
  Link:         http://adventofcode.com/2023/day/2
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2023/AdventOfCode_02_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/02/2023
*/

#ifndef _2023_ADVENTOFCODE_02_H_
#define _2023_ADVENTOFCODE_02_H_

#include <algorithm>
#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include "../util/util.h"

namespace aoc2023_02 {
  using namespace std;

  constexpr int kMaxRed = 12, kMaxGreen = 13,  kMaxBlue = 14;

  struct GameRound {
    int red = 0;
    int green = 0;
    int blue = 0;
    bool isValid() const {
      return red <= kMaxRed && green <= kMaxGreen && blue <= kMaxBlue;
    }
    void print() const {
      cout << "\tRed: " << red << ". Green: " << green << ". Blue: " << blue;
      if (!isValid()) {
        cout << ". INVALID!";
      }
      cout << endl;
    }
  };

  struct Game {
    int id = 0;
    vector<GameRound> rounds;
    bool isValid() const {
      for (const GameRound& round : rounds) {
        if (!round.isValid()) {
          return false;
        }
      }
      return true;
    }
    void print() const {
      cout << "Game " << id << ". Rounds: " << rounds.size() << ".";
      if (!isValid()) {
        cout << " INVALID";
      }
      cout << endl;
      for (const GameRound& round : rounds) {
         round.print();
      }
    }
    int maxRed() {
      int max = 0;
       for (const GameRound& round : rounds) {
         max = std::max(round.red, max);
      }
      return max;
    }

    int maxGreen() {
      int max = 0;
       for (const GameRound& round : rounds) {
         max = std::max(round.green, max);
      }
      return max;
    }

    int maxBlue() {
      int max = 0;
       for (const GameRound& round : rounds) {
         max = std::max(round.blue, max);
      }
      return max;
    }
    int64_t power() {
      int64_t result = maxRed();
      result *= maxGreen();
      result *= maxBlue();
      return result;
    }
  };

  Game parseGame(const std::string& input) {
    Game g;
    std::string strTemp;
    int intTemp, index = 5;
    g.id = util::getNumberRef(input, index);
    index += 2; // skip colon + space.
    g.rounds.emplace_back();

    while (index < input.size()) {
      intTemp = util::getNumberRef(input, index);
      switch(input[++index]) {
        case 'r': g.rounds.back().red += intTemp; break;
        case 'g': g.rounds.back().green += intTemp; break;
        case 'b': g.rounds.back().blue += intTemp; break;
      }
      for (++index; index < input.size(); ++index) {
        if (input[index] == ' ') {
          if (input[index - 1] == ';') {
            g.rounds.emplace_back();
          }
          ++index;
          break;
        }
      }
    }
    g.print();
    return g;
  }

  int64_t solve1(const std::string& input){
    Game g = parseGame(input);
    return g.isValid() ? g.id : 0;
  }

  int64_t solve2(const std::string& input) {
    Game g = parseGame(input);
    return g.power();
  }

  void solve(int part = 1) {
    std::string input;
    int64_t acum = 0;
    while (std::getline(cin, input)) {
      acum += (part == 1 ? solve1(input) : solve2(input));
    }
    cout << "Solution: " << acum << endl;
  }
};
#endif /* _2023_ADVENTOFCODE_01_H_ */
