/*
  Link:         http://adventofcode.com/2022/day/1
 
  Compiling:     g++ -std=c++17 -o main main.cpp 
  Running:       cat 2022/AdventOfCode_01_input.txt | ./main
               
  Programmer:   Michael Duarte.

  Date:         12/01/2022
*/

#ifndef _2022_ADVENTOFCODE_01_H_
#define _2022_ADVENTOFCODE_01_H_

#include <iostream>
#include <deque>
#include <vector>

namespace aoc2022_01 {
  using namespace std;

  unsigned long long solve1() {
    int calories, acum = 0, max_calories = 0;
    string line;
    while(true) {
      if (!getline (cin,line) || line.empty()) {
        if (acum > max_calories) {
          if (max_calories) {
            cout << "This elf has the most calories so far - ";
          }
          max_calories = acum;
        }
        cout << "Total cals: " << acum << endl;
        acum = 0;
        continue;
      }
      calories = stoi(line);
      acum += calories;
    }
    cout << "The elf with most calories has: ";
    return max_calories;
  }

  unsigned long long solve2() {int calories, acum = 0, max_calories = 0;
    string line;
    vector<int> max_calories_per_elf;
    while(true) {
      if (!getline (cin,line) || line.empty()) {
        if (acum > max_calories) {
          if (max_calories) {
            cout << "This elf has the most calories so far - ";
          }
          max_calories = acum;
        }
        max_calories_per_elf.push_back(acum);
        cout << "Total cals: " << acum << endl;
        acum = 0;
        if (!line.empty()) {
          break;
        }
        continue;
      }
      calories = stoi(line);
      acum += calories;
    }
    cout << "The top 3 elf with most calories have: ";
    sort(max_calories_per_elf.begin(), max_calories_per_elf.end());
    acum = 0;
    for (int i = max_calories_per_elf.size() - 1, y = 0; y < 3; ++y, --i) {
      cout << y+1 << ": " << max_calories_per_elf[i] << endl;
      acum += max_calories_per_elf[i];
    }
    cout << "Adding them together: ";
    return acum;
  }

  void solve(int part = 1) {
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }
};
#endif /* _2022_ADVENTOFCODE_01_H_ */
