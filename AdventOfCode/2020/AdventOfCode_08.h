/*
  Link:         http://adventofcode.com/2020/day/08
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2020
*/

#ifndef _2020_ADVENTOFCODE_08_H_
#define _2020_ADVENTOFCODE_08_H_

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

namespace aoc2020_08 {
using namespace std;

constexpr char kAcc[] = "acc";
constexpr char kJmp[] = "jmp";
constexpr char kNop[] = "nop";

long long accumulator = 0;

class Command {
public:
  Command() = default;
  Command(size_t i, const string &in, const int &pa)
      : index{i}, ins{in}, param{pa} {}

  size_t index;
  string ins;
  int param;

  void print() const {
    cout << "Command [" << index << "] :" << ins << ": " << param << endl;
  }

  bool operator==(const Command &cmd) const {
    return index == cmd.index;
  }

  struct cmd_hash {
    size_t operator() (const Command& cmd) const {
      return cmd.index;
    }
  };
};

vector<Command> getCommands() {
  string ins, param;
  vector<Command> instructions;

  while(!cin.eof()) {
    cin >> ins >> param;
    if (ins.empty()) break;
    instructions.emplace_back(instructions.size(), ins, atoi(param.c_str()));
    instructions.back().print();
  }
  return instructions;
}

long long getAccumulatorValueOnLoop(const vector<Command> &commands) {
  unordered_set<Command, Command::cmd_hash> seenCommands;
  for (int index = 0; index < commands.size(); ++index) {
    const Command &cmd = commands[index];
    cout << "Computing: "; cmd.print();
    if (seenCommands.find(cmd) != seenCommands.end()) {
      cout << "Repeated command found: " << index << endl;
      return accumulator;
    }
    seenCommands.insert(cmd);
    if (strcmp(cmd.ins.c_str(), kAcc) == 0) {
      accumulator += cmd.param;
    } else if (strcmp(cmd.ins.c_str(), kJmp) == 0) {
      index += cmd.param - 1;
    } else if (strcmp(cmd.ins.c_str(), kNop) == 0) {
      // Nothing
    } else {
      cout << "THIS SHOULDN'T HAPPEN!!" << endl;
    }
  }
  return -1;
}

void solve1() {
  vector<Command> commands = getCommands(); 
  cout << "Accumulator on loop: " << getAccumulatorValueOnLoop(commands) << endl;
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

};  // aoc2020_08

#endif /* _2020_ADVENTOFCODE_08_H_ */
