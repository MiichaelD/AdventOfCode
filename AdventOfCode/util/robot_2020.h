
#ifndef _2020_ROBOT_H_
#define _2020_ROBOT_H_

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

namespace aoc2020 {
using namespace std;

constexpr char kAcc[] = "acc";
constexpr char kJmp[] = "jmp";
constexpr char kNop[] = "nop";

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

class Robot {
public:
  long long accumulator;
  vector<Command> commands;
  size_t control;

  vector<Command> getCommands() {
    string ins, param;
    commands.clear();
    while(!cin.eof()) {
      cin >> ins >> param;
      if (ins.empty()) break;
      commands.emplace_back(commands.size(), ins, atoi(param.c_str()));
      commands.back().print();
    }
    return commands;
  }

  bool processCommand(const Command cmd) {
    if (strcmp(cmd.ins.c_str(), kAcc) == 0) {
      accumulator += cmd.param;
      return true;
    } else if (strcmp(cmd.ins.c_str(), kJmp) == 0) {
      control += cmd.param - 1;
      return true;
    } else if (strcmp(cmd.ins.c_str(), kNop) == 0) {
      return true;
    }
    return false;
  }

  inline bool processCommandAtControl() {
    return processCommand(commands[control]);
  }

  bool findLoop() {
    unordered_set<Command, Command::cmd_hash> seenCommands;
    for (; control < commands.size(); ++control) {
      const Command &cmd = commands[control];
      if (seenCommands.find(cmd) != seenCommands.end()) {
        cout << "Repeated command found: " << control << endl;
        return true;
      }
      seenCommands.insert(cmd);
      processCommand(cmd);
    }
    return false;
  }

  int findCorruptedCommand() {
    control = commands.size() - 1;
      bool foundLoop = true;
      for (; control >=0 && foundLoop; --control) {
        Command &cmd = commands[control];
        accumulator = 0;
        if (strcmp(cmd.ins.c_str(), kJmp) == 0) {
          cmd.ins = kNop;
          foundLoop = findLoop();
          cmd.ins = kJmp;
        } else if (strcmp(cmd.ins.c_str(), kNop) == 0) {
          cmd.ins = kJmp;
          foundLoop = findLoop();
          cmd.ins = kNop;
        }
      }
      cout << "Loop fixed @ command: "; commands[control + 1].print(); 
      return control;
  }

};

};

#endif /* _2020_ROBOT_H_ */
