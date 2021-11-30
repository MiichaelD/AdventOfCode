/*
  Link:         http://adventofcode.com/2020/day/2
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/01/2020
*/

#ifndef _2021_ADVENTOFCODE_02_H_
#define _2021_ADVENTOFCODE_02_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "../util/util.h"

namespace aoc2021_02 {
  using namespace std;

  struct Policy {
    pair<int,int> range;
    char letter;
    Policy (pair<int,int> &&range, char letter) : range(range), letter(letter) {}
  };

  pair<int,int> parseRange(const string &input) {
    int i = 0, j = 0;
    int acum = 0;
    for (int index = 0; index < input.size(); ++index) {
      if (isdigit(input[index])) {
        acum *= 10;
        acum += input[index] - '0';
      } else if (input[index] == '-') {
        i = acum;
        acum = 0;
        continue;
      }
    }
    j = acum;
    return std::make_pair(i,j);
  }

  bool checkOldPasswordValidity(const Policy &p, const string &pass) {
    int reps = 0;
    // cout << "Checking '" << pass << "' - letter " << p.letter << ". ";
    // print(p.range); cout << ". Found: ";
    for (char c : pass) {
      if (c == p.letter) ++reps;
    }
    bool result = reps >= p.range.first && reps <= p.range.second;
    // cout << reps << "\t" << (result ? "valid" : "invalid") << endl;
    return result;
  }

  bool checkPasswordValidity(const Policy &p, const string &pass) {
    int reps = 0;
    // cout << "Checking '" << pass << "' - letter " << p.letter << ". ";
    // print(p.range); cout << ". Found: ";
    if (pass[p.range.first - 1] == p.letter) ++reps;
    if (pass[p.range.second - 1] == p.letter) ++reps;
    bool result = reps == 1;
    // cout << reps << "\t" << (result ? "valid" : "invalid") << endl;
    return result;
  }

  pair<int,int> solveBoth() {
    string rangeInput, letter, password;
    int oldValid = 0, newValid = 0;
    while(!std::cin.eof()) {
      cin >> rangeInput;
      if (std::cin.eof()) break; // In case there is an extra line
      cin >> letter >> password;
      Policy policy(parseRange(rangeInput), letter[0]);
      if (checkOldPasswordValidity(policy, password))
        ++oldValid;
      if (checkPasswordValidity(policy, password))
        ++newValid;
    }
    return std::make_pair(oldValid, newValid);
  }

  void solve(int part = 1) {
    using namespace std;
    util::printPair(solveBoth(), false);
  }

}

#endif /* _2021_ADVENTOFCODE_02_H_ */
