/*
  Link:         http://adventofcode.com/2022/day/2
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2022/AdventOfCode_02_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/02/2022
*/

#ifndef _2022_ADVENTOFCODE_02_H_
#define _2022_ADVENTOFCODE_02_H_

#include <string>
#include <iostream>
#include "../util/util.h"

namespace aoc2022_02 {
  using namespace std;

  // A = X = Rock    = Lose
  // B = Y = Paper   = Draw
  // C = Z = Scissor = Win

  int getPointsForChoice(char c) {
    switch(c) {
      case 'X':
      return 1;
      case 'Y':
      return 2;
      case 'Z':
      return 3;
      default:
      break; 
    }
    return 0;
  }

  int getPointsForOutcome(const pair<char, char> &entry) {
    switch(entry.first) {
      case 'A':
        switch(entry.second) {
          case 'X':
            return 3;
          case 'Y':
            return 6;
          case 'Z':
            return 0;
          default:
            break; 
        }
        break;
      case 'B':
        switch(entry.second) {
          case 'X':
            return 0;
          case 'Y':
            return 3;
          case 'Z':
            return 6;
          default:
            break; 
        }
        break;
      case 'C':
        switch(entry.second) {
          case 'X':
            return 6;
          case 'Y':
            return 0;
          case 'Z':
            return 3;
          default:
            break; 
        }
        break;
      default:
      break; 
    }
    return 0;
  }

int getPointsForExpectedOutcome(char c) {
    switch(c) {
      case 'X':
      return 0;
      case 'Y':
      return 3;
      case 'Z':
      return 6;
      default:
      break; 
    }
    return 0;
  }

  int getPointsForExpectedOutcome(const pair<char, char> &entry) {
    switch(entry.first) {
      case 'A':
        switch(entry.second) {
          case 'X':  // Lose - scissor - Z
            return getPointsForChoice('Z');
          case 'Y':  // Draw - rock - X
            return getPointsForChoice('X');
          case 'Z':  // Win - paper - Y
            return getPointsForChoice('Y');
          default:
            break; 
        }
        break;
      case 'B':
        switch(entry.second) {
          case 'X':  // Lose
            return getPointsForChoice('X');
          case 'Y':  // Draw
            return getPointsForChoice('Y');
          case 'Z':  // Win
            return getPointsForChoice('Z');
          default:
            break; 
        }
        break;
      case 'C':
        switch(entry.second) {
          case 'X':  // Lose
            return getPointsForChoice('Y');
          case 'Y':  // Draw
            return getPointsForChoice('Z');
          case 'Z':  // Win
            return getPointsForChoice('X');
          default:
            break; 
        }
        break;
      default:
      break; 
    }
    return 0;
  }

  int64_t solve1() {
    pair<char, char> entry;
    string input;
    int64_t acum = 0;
    while(!cin.eof()) {
      getline(cin, input);
      entry.first = input[0];
      entry.second = input[2];
      util::printPair(entry, true);
      acum += getPointsForChoice(entry.second);
      acum += getPointsForOutcome(entry);
    }
    return acum;
  }

  int64_t solve2() {
    pair<char, char> entry;
    string input;
    int64_t acum = 0;
    while(!cin.eof()) {
      getline(cin, input);
      entry.first = input[0];
      entry.second = input[2];
      util::printPair(entry, true);
      acum += getPointsForExpectedOutcome(entry.second);
      acum += getPointsForExpectedOutcome(entry);
    }
    return acum;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

}

#endif /* _2022_ADVENTOFCODE_02_H_ */
