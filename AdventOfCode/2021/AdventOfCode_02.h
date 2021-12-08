/*
  Link:         http://adventofcode.com/2021/day/2
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2021/AdventOfCode_01_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/01/2021
*/

#ifndef _2021_ADVENTOFCODE_02_H_
#define _2021_ADVENTOFCODE_02_H_

#include <string>
#include "../util/util.h"

namespace aoc2021_02 {
  using namespace std;

  int64_t solve1() {
    string command;
    int64_t forward = 0, depth = 0, delta;
    while(!cin.eof()) {
      getline(cin, command);
      if (command.empty()) break;
      switch(command[0]) {
        case 'f':
          delta = util::getNumber(command, 8);
          forward += delta;
          cout << "\tForward: " << forward << endl;
          break;
        case 'd': 
          delta = util::getNumber(command, 5);
          depth += delta;
          cout << "\tdepth down: " << depth << endl;
          break;
        case 'u': 
          delta = util::getNumber(command, 3);
          depth -= delta;
          cout << "\tdepth up: " << depth << endl;
          break;
      }
    }
    return forward * depth;
  }

  int64_t solve2() {
    string command;
    int64_t forward = 0, depth = 0, aim = 0, delta;
    while(!cin.eof()) {
      getline(cin, command);
      if (command.empty()) break;
      switch(command[0]) {
        case 'f':
          delta = util::getNumber(command, 8);
          forward += delta;
          depth += (aim * delta);
          cout << "\tForward: " << forward << ", Depth: " << depth << endl;
          break;
        case 'd': 
          delta = util::getNumber(command, 5);
          aim += delta;
          cout << "\taim down: " << aim << endl;
          break;
        case 'u': 
          delta = util::getNumber(command, 3);
          aim -= delta;
          cout << "\taim up: " << aim << endl;
          break;
      }
    }
    return forward * depth;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << "Solution: " << (part == 1 ? solve1() : solve2()) << endl;
  }

}

#endif /* _2021_ADVENTOFCODE_02_H_ */
