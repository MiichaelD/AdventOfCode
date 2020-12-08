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

bool findLoop(const vector<Command> &commands) {
  unordered_set<Command, Command::cmd_hash> seenCommands;
  for (int index = 0; index < commands.size(); ++index) {
    const Command &cmd = commands[index];
    if (seenCommands.find(cmd) != seenCommands.end()) {
      cout << "Repeated command found: " << index << endl;
      return true;
    }
    seenCommands.insert(cmd);
    if (strcmp(cmd.ins.c_str(), kAcc) == 0) {
      accumulator += cmd.param;
    } else if (strcmp(cmd.ins.c_str(), kJmp) == 0) {
      index += cmd.param - 1;
    }
  }
  return false;
}

int findCorruptedCommand(vector<Command> commands) {
  int index = commands.size() - 1;
    bool foundLoop = true;
    for (; index >=0 && foundLoop; --index) {
      Command &cmd = commands[index];
      accumulator = 0;
      if (strcmp(cmd.ins.c_str(), kJmp) == 0) {
        cmd.ins = kNop;
        foundLoop = findLoop(commands);
        cmd.ins = kJmp;
      } else if (strcmp(cmd.ins.c_str(), kNop) == 0) {
        cmd.ins = kJmp;
        foundLoop = findLoop(commands);
        cmd.ins = kNop;
      }
    }
    cout << "Loop fixed @ command: "; commands[index + 1].print(); 
    return index;
}

void solve(int part = 1) {
  vector<Command> commands = getCommands(); 
  if (part == 1) {
    findLoop(commands);
  } else {
    findCorruptedCommand(commands);
  }
  cout << "Accumulator: " << accumulator << endl;
}

};  // aoc2020_08

#endif /* _2020_ADVENTOFCODE_08_H_ */
