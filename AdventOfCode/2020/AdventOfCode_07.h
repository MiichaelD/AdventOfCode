/*
  Link:         http://adventofcode.com/2020/day/07
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/07/2020
*/

#ifndef _2020_ADVENTOFCODE_07_H_
#define _2020_ADVENTOFCODE_07_H_

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

namespace aoc2020_07 {
using namespace std;

constexpr char kMyBag[] = "shiny_gold";

typedef unordered_map<string,unordered_map<string,int>> BagRules;

inline string composeId(const string &adj, const string &color) {
  return adj + "_" + color; 
}

BagRules getInput() {
  string adjective, color, id, skip, bag, maybeQuantity;
  int quantity;
  vector<string> lines;
  stringstream idStream;
  BagRules bagRules;
  while(!cin.eof()) {
    cin >> adjective;
    if (adjective.empty()) break; 
    cin >> color >> bag >> skip;
    id = composeId(adjective, color);
    bagRules[id];
    cin >> maybeQuantity;
    if (!isdigit(maybeQuantity[0])) {
      cin >> skip >> skip;  // consume
      continue;
    }
    cin >>adjective >> color >> bag;
    int quantity = atoi(maybeQuantity.c_str());
    bagRules[id][composeId(adjective, color)] = quantity;
    while (bag[bag.size()-1] != '.') {
      cin >> maybeQuantity;
      if (!isdigit(maybeQuantity[0])) {
        cin >> skip >> skip;  // consume
        continue;
      }
      cin >>adjective >> color >> bag;
      int quantity = atoi(maybeQuantity.c_str());
      bagRules[id][composeId(adjective, color)] = quantity;
    }
  }
  return bagRules;
}

void printBagRules(const BagRules& bagRules) {
  cout << "Printing bag rules: " << bagRules.size() << endl;
  for (const auto &rule : bagRules) {
    cout << "Contents: " << rule.first << ":" << endl; 
    for (const auto &contents : rule.second) {
      cout << "\t" << contents.first << ": " << contents.second << endl;
    }
  }
}

size_t findBagsContainingMyBag(const BagRules &bagRules) {
  unordered_set<string> checked;
  deque<string> toCheck({kMyBag});
  while (!toCheck.empty()) {
    // Find all bags that may contain this bag;
    for (const auto &rule : bagRules) {
      if (rule.second.find(toCheck.front()) != rule.second.end()) {
        toCheck.push_back(rule.first);
      }
    }
    checked.insert(toCheck.front());
    toCheck.pop_front();
  }
  return checked.size() - 1;
}

void solve1() {
  BagRules bagRules = getInput();
  cout << "Total bags: " << findBagsContainingMyBag(bagRules) << endl;
  // printBagRules(bagRules);
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

};  // aoc2020_07

#endif /* _2020_ADVENTOFCODE_07_H_ */
