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

typedef vector<vector<int>> Subrule;

struct Rule {
  size_t index;
  char letter = 0;
  Subrule subrules;

  void print() const {
    cout << "\t" << index << ": ";
    if (letter) {
      cout << "'" << letter << "'";
    } else if (subrules.size()) {
      for (int rI = 0; rI < subrules.size(); ++rI) {
        const auto &rr = subrules[rI];
        for (size_t val : rr) {
          cout << " " << val;
        }
        if (rI < subrules.size() - 1) {
          cout << " |"; 
        }
      }
    }
    cout << endl;
  }
};

struct Data {
  vector<Rule> rules;
  vector<string> messages;

  void print() const {
    cout << "Rules: " << rules.size() << endl;
    for (const Rule &r : rules) {
      r.print();
    }
    cout << endl << "Messages: " << messages.size() << endl;
    // for (const string &m : messages) {
    //   cout << "\t'" << m << "'" << endl;
    // }
  }
};

void printSubrule(const vector<int> &subrule) {
  cout << '[';
  for (int s : subrule) {
    cout << ", " << s;
  }
  cout << ']';
}

int getNumber(const string &line, int &index) {
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
  int index = 0;
  Rule result;
  result.index = getNumber(input, index);
  result.subrules.emplace_back(vector<int>{});
  while (index < input.size()) {
    if (isdigit(input[index])) {
      int number = getNumber(input, index);
      result.subrules.back().push_back(number);
    } else if (input[index] == '"') {
      result.letter = input[index + 1];
      break;
    } else if (input[index] == '|') {
      index += 2;
      result.subrules.emplace_back(vector<int>{});
    }
  }
  if (result.subrules.back().empty()) {
    result.subrules.pop_back();
  }
  return result;
}

void fillGaps(vector<Rule> &rules) {
  vector<size_t> missing;
  for (int i = 0 ; i < rules.size(); ++i) {
    if (i == rules[i].index) { continue; }
    for (int j = rules[i - 1].index + 1; j < rules[i].index; ++j) {
      missing.push_back(j);
    }
  }
  for (size_t &m : missing) {
    rules.emplace_back();
    rules.back().index = m;
  }
  sort(rules.begin(), rules.end(),
       [](const Rule &a, const Rule &b) { return a.index < b.index; });
}

Data getInput() {
  Data result;
  string line;
  bool loadRules = true;
  while(!cin.eof()) {
    getline(cin, line);
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
  sort(result.rules.begin(), result.rules.end(),
        [](const Rule &a, const Rule &b) { return a.index < b.index; });
  fillGaps(result.rules);
  return result;
}

inline ostream& printPadded(int depth) {
  for (int i = 0; i < (depth + 1); ++i) cout << "  ";
  return cout;
}

bool isValid(
    const string &msg,const vector<Rule> &rules,
    int &msgInd, int rInd = 0, int srInd = 0, int depth = 0) {
  const Rule &rule = rules[rInd];
  printPadded(depth) << "["<<depth<<"] Validating rule " << rInd;
  if (msgInd >= msg.size()) {
    cout << "\t It exceeds, false" << endl;
    return false;
  }
  cout << " @ '" << msg[msgInd] << "' [" << msgInd << "]";
  if (rule.letter) {
    bool result = msg[msgInd++] == rule.letter;
    cout << (result ? " - Matches!" : " - Fails") << endl;
    return result;
  }
  cout << endl;
  // Try each subrules until a valid one is found
  int tempInd = msgInd;
  vector<pair<int,int>> valids;
  for (; srInd < rule.subrules.size(); ++srInd) {
    const auto &sr = rule.subrules[srInd];
    printPadded(depth) <<  "Subrule " << (srInd + 1) << ": ";
    printSubrule(sr); cout << endl;

    bool valid = true;
    int retries = 0;
    // Try each term in the subrule
    for (int valInd = 0; valInd < sr.size(); ++valInd) {
      int prevMsgInd = msgInd;
      bool wasValid = isValid(msg, rules, msgInd, sr[valInd], retries, depth + 1);
      if (!wasValid) {
        printPadded(depth) << "["<<depth<<"] Not valid, ";
        int prevRule = sr[valInd - 1];
        if (valids.size()) {
          // Try previous partial solution but expand it.
          valInd = valids.back().first;
          msgInd = valids.back().second;
          valids.pop_back();
          ++retries;
          cout << "RETRYINGGGGGGGGGGG" << endl;
        } else {
          valid = false;
          break;
        }
      } else if (sr[valInd] == 8 || sr[valInd] == 11) {
        // Save current partial solution, it might be the case that it could wrap even more chars
        valids.emplace_back(valInd - 1, prevMsgInd);
      }
    }
    if (valid) {
      return true;
    }
    cout << "returning msgInd from " << msgInd << " to " << tempInd << endl;
    msgInd = tempInd;
  }
  return false;
}

void solve(int part = 1) {
  Data data = getInput();
  if (part == 2 && data.rules.size() > 11) {
    // for (int i = 2; i < 40; ++i) {
    //   vector<int> toAdd(i, 42);
    //   data.rules[8].subrules.push_back(toAdd);
    // }
    // for (int i = 2; i < 40; ++i) {
    //   vector<int> toAdd(2 * i, 42);
    //   for (int j = toAdd.size() / 2; j < toAdd.size(); ++j) {
    //     toAdd[j] = 31;
    //   }
    //   data.rules[11].subrules.push_back(toAdd);
    // }
    data.rules[8].subrules.push_back({42, 8});
    data.rules[11].subrules.push_back({42, 11, 31});
    // for (size_t val : {8, 11}) {
    //   sort(data.rules[val].subrules.begin(),
    //       data.rules[val].subrules.end(),
    //       [](const vector<size_t> &a, const vector<size_t> &b) {
    //         return a.size() > b.size();
    //         });
    // }
  }
  data.print(); cout << endl;
  size_t validMessages = 0;
  for (const string &msg : data.messages) {
    int msgInd = 0, rInd = 0;
    cout << "Validating message: " << msg << endl;
    if (isValid(msg, data.rules, msgInd, rInd) && msgInd == msg.size()) {
      cout << "Message: " << msg << " is valid and fully consumed" << endl;
      cout << "Len: " << msg.size() << " msgInd: " << msgInd << ". Delta: ";
      cout << (msg.size() - msgInd) << endl;
      ++validMessages;
    }
    cout << endl;
  }
  cout << "Valid messages: " << validMessages << endl;
  data.print();
}

};  // aoc2020_19

#endif /* _2020_ADVENTOFCODE_19_H_ */
