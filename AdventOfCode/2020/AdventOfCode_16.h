/*
  Link:         http://adventofcode.com/2020/day/16
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/16/2020
*/

#ifndef _2020_ADVENTOFCODE_16_H_
#define _2020_ADVENTOFCODE_16_H_

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

namespace aoc2020_16 {
using namespace std;

struct Range {
  string name;
  vector<pair<int,int>> rules;

  bool isInRange(int val) const {
    for (const auto &p : rules) {
      if (p.first <= val && val <= p.second) {
        return true;
      }
    }
    return false;
  }

  void print() const {
    cout << "\tRange '" << name << "':" << endl;
    for (const auto &r : rules) {
      cout << "\t\t"; util::printPair(r, true);
    }
  }
};

struct Ticket {
  vector<size_t> values;
  bool discarded = false;
}

struct Data{
  vector<Range> ranges;
  vector<Ticket> tickets;

  void print() const {
    cout << "Ranges: " << ranges.size() << endl;
    for (const auto &r : ranges) {
      r.print();
    }
    cout << "Tickets: " << tickets.size() << endl;
    // for (const auto &ticket : tickets) {
    //   cout << "\t";
    //   for (size_t t : ticket) {
    //     cout << ", " << t;
    //   }
    //   cout << endl;
    // }
  }
};

pair<int,int> getRangeRule(const string &line, size_t &index) {
  pair<int,int> result{-1, -1};
  int accum = 0;
  for (; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == '-') {
      result.first = accum;
      accum = 0;
    } else if (line[index] == ' ') {
      result.second = accum;
      break;
    }
  }
  if (result.second == -1) {
    result.second = accum;
  }
  return result;
}

vector<size_t> getTicketData() {
  vector<size_t> input;
  string line;
  cin >> line;
  uint64_t accum = 0;
  size_t bInd = 0;
  for (size_t index = 0; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ',') {
      input.push_back(accum);
      accum = 0;
    }
  }
  if (accum != 0) {
    input.push_back(accum);
    accum = 0;
  }
  return input;
}

Data getInput() {
  Data result;
  string str, line;
  vector<string> lines;
  stringstream ss;
  do {
    getline(cin,line);
    if (line.empty()) break;
    result.ranges.push_back({});
    size_t index = 0;
    // Getting Range name
    for (; index < line.size(); ++index) {
      if (line[index] == ':') {
        result.ranges.back().name = line.substr(0, index);
        break;
      }
    }
    index += 2;
    result.ranges.back().rules.push_back(getRangeRule(line, index));
    index += 4;
    result.ranges.back().rules.push_back(getRangeRule(line, index));
  } while(!line.empty());

  getline(cin,line);
  result.tickets.emplace_back(getTicketData());
  getline(cin,line);
  getline(cin,line);
  getline(cin,line);
  while(!cin.eof()) {
    result.tickets.emplace_back(getTicketData());
  }
  return result;
}

void solve1() {
  Data input = getInput();
  input.print();

  uint64_t result = 0;
  for (auto &ticket : input.tickets) {
    for (auto &t : ticket.values) {
      bool isValid = false;
      for (const auto &r: input.ranges) {
        if (r.isInRange(t)) {
          isValid = true;
          break;
        }
      }
      if (!isValid) {
        cout << "Ticket value " << t << " not in any valid range" << endl;
        ticket.discarded = true;
        result += t.val;
      }
    }
  }
  cout << "Ticket staning error rate: " << result << endl;
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

};  // aoc2020_16

#endif /* _2020_ADVENTOFCODE_16_H_ */
