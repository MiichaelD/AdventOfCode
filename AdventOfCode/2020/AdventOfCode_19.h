/*
  Link:         http://adventofcode.com/2020/day/19
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/19/2020
*/

#ifndef _2020_ADVENTOFCODE_19_H_
#define _2020_ADVENTOFCODE_19_H_

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

namespace aoc2020_19 {
using namespace std;

struct Rule {
  size_t index;
  char letter = 0;
  vector<vector<size_t>> subrules;
};

struct Data {
  vector<Rule>rules;
  vector<string> messages;

  void print() const {
    cout << "Rules:" <<endl;
    for (const Rule &r : rules) {
      cout << "\t" << r.index << ": ";
      if (r.letter) {
        cout << "'" << r.letter << "'" << endl;
      } else if (r.subrules.size()) {
        // for (const auto &rr : r.rules) {
        for (int rI = 0; rI < r.subrules.size(); ++rI) {
          const auto &rr = r.subrules[rI];
          for (size_t val : rr) {
            cout << " " << val;
          }
          if (rI < r.subrules.size() - 1) {
            cout << " |"; 
          }
        }
        cout << endl;
      }
    }
    cout << endl << "Messages: "<< endl;
    for (const string &m : messages) {
      cout << "\t" << m << endl;
    }
  }
};

int getNumber(const string &line, size_t &index) {
  int accum = 0;
  for (; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ' ') {
      ++index;
      break;
    } else if (line[index] == ':') {
      index += 2;
      break;
    } 
  }
  // cout << "\tGet number: " << accum << endl;
  return accum;
}

Rule parseRule(const string &input) {
  size_t index = 0, orTerms = 0;
  Rule result;
  result.index = getNumber(input, index);
  result.subrules.emplace_back(vector<size_t>{});
  while (index < input.size()) {
    if (isdigit(input[index])) {
      int number = getNumber(input, index);
      result.subrules.back().push_back(number);
    } else if (input[index] == '"') {
      result.letter = input[index + 1];
      break;
    } else if (input[index] == '|') {
      index += 2;
      result.subrules.emplace_back(vector<size_t>{});
    }
  }
  if (result.subrules.back().empty()) {
    result.subrules.pop_back();
  }
  return result;
}

Data getInput() {
  Data result;
  string line;
  bool loadRules = true;
  while(!cin.eof()) {
    getline(cin, line);
    // cout << "Processing: " << line << endl;
    if (line.empty()) {
      loadRules = false;
      continue;
    }
    if (loadRules) {
      result.rules.push_back(parseRule(line));
    } else {
      result.messages.push_back(line);
    }
  }
  return result;
}

bool isValid(
    const string &msg,const vector<Rule> &rules,
    size_t &msgInd, size_t rInd = 0, size_t srInd = 0) {
  for (int i = 0; i < msgInd; ++i) cout << "  ";
  cout << "Validating rule " << rInd << endl;
  const Rule &rule = rules[rInd];
  if (msgInd >= msg.size()) {
    return true;
  }
  if (rule.letter) {
    return msg[msgInd++] == rule.letter;
  }
  for (; srInd < rule.subrules.size(); ++srInd) {
    const auto &sr = rule.subrules[srInd];
    bool valid = true;
    size_t tempInd = msgInd;
    for (size_t val : sr) {
      if (!isValid(msg, rules, msgInd, val, 0)) {
        valid = false;
        break;
      }
    }
    if (valid) {
      return true;
    } else {
      msgInd = tempInd;
    }
  }
  return false;
}

void solve1() {
  Data data = getInput();
  sort(data.rules.begin(), data.rules.end(), [](const Rule &a, const Rule &b) {
    return a.index < b.index;
  });
  data.print(); cout << endl;
  size_t validMessages = 0;
  for (const string &msg : data.messages) {
    size_t msgInd = 0, rInd = 0, srInd = 0;
    if (isValid(msg, data.rules, msgInd, rInd, srInd)
        && msgInd == msg.size()) {
      cout << "Message: " << msg << " is valid" << endl;
      ++validMessages;
    }

  }
  cout << "Valid messages: " << validMessages << endl;
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

};  // aoc2020_19

#endif /* _2020_ADVENTOFCODE_19_H_ */
