/*
  Link:         http://adventofcode.com/2020/day/2
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/01/2020
*/

#ifndef _2020_ADVENTOFCODE_02_H_
#define _2020_ADVENTOFCODE_02_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace aoc2020_02 {
  using namespace std;

  struct Policy {
    pair<int,int> range;
    char letter;
    Policy (pair<int,int> &&range, char letter) : range(range), letter(letter) {}
  };

  template<class T>
  inline void print(const pair<T,T> &p) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }

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
    cout << "Checking '" << pass << "' - letter " << p.letter << ". ";
    print(p.range); cout << ". Found: ";
    for (char c : pass) {
      if (c == p.letter) ++reps;
    }
    bool result = reps >= p.range.first && reps <= p.range.second;
    cout << reps << "\t" << (result ? "valid" : "invalid") << endl;
    return result;
  }

  bool checkPasswordValidity(const Policy &p, const string &pass) {
    int reps = 0;
    cout << "Checking '" << pass << "' - letter " << p.letter << ". ";
    print(p.range); cout << ". Found: ";
    if (pass[p.range.first - 1] == p.letter) ++reps;
    if (pass[p.range.second - 1] == p.letter) ++reps;
    bool result = reps == 1;
    cout << reps << "\t" << (result ? "valid" : "invalid") << endl;
    return result;
  }

  unsigned long long solve1() {
    string rangeInput, letter, password;
    unsigned long long validPasswords = 0;
    while(!std::cin.eof()) {
      cin >> rangeInput >> letter >> password;
      Policy policy(parseRange(rangeInput), letter[0]);
      if (checkOldPasswordValidity(policy, password))
        ++validPasswords;
    }
    return validPasswords;
  }

  unsigned long long solve2() {
    string rangeInput, letter, password;
    unsigned long long validPasswords = 0;
    while(!std::cin.eof()) {
      cin >> rangeInput >> letter >> password;
      Policy policy(parseRange(rangeInput), letter[0]);
      if (checkPasswordValidity(policy, password))
        ++validPasswords;
    }
    return validPasswords;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2020_ADVENTOFCODE_02_H_ */
