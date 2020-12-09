/*
  Link:         http://adventofcode.com/2020/day/09
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/09/2020
*/

#ifndef _2020_ADVENTOFCODE_09_H_
#define _2020_ADVENTOFCODE_09_H_

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

namespace aoc2020_09 {
using namespace std;

size_t preamble = 5;

vector<unsigned long long> getNumbers() {
  unsigned long long n;
  vector<unsigned long long> numbers;
  stringstream ss;
  while (!cin.eof()) {
    cin >> n;
    numbers.push_back(n);
  }
  return numbers;
}

pair<int,int> findNumbersAddingUpTo(const vector<unsigned long long> numbers, int index) {
  for (int i = index - preamble; i <= index; ++i) {
    int n = numbers[i];
    // cout << "\ti = " << n << endl;
    for (int j = i + 1; j <= index; ++j) {
      // cout << "\t\tj = " << numbers[j] << endl;
      if (n + numbers[j] == numbers[index]) {
        cout << "Found pair adding up to " << numbers[index] << ". i:"
            << numbers[i] << ", j: " << numbers[j] << endl; 
        return make_pair(n, numbers[j]);
      }
    }
  }
  return make_pair(-1, -1);
}

int findFirstNumberNotAddingUp(const vector<unsigned long long> numbers) {
  for (int i = preamble; i < numbers.size(); ++i) {
    // cout << "Trying to find numbers adding to " << numbers[i] << endl;
    pair<int,int> p = findNumbersAddingUpTo(numbers, i);
    if (p.first == -1 || p.second == -1) {
      return numbers[i];
    }
  }
  return -1;
}

void solve1() {
  vector<unsigned long long> numbers = getNumbers();
  cout << findFirstNumberNotAddingUp(numbers) << endl;
}

void solve2() {
  string input;
  cin >> input;
}

void solve(int part = 1) {
  if (part == 1) {
    preamble = 25;
    solve1();
  } else {
    solve2();
  }
}

};  // aoc2020_09

#endif /* _2020_ADVENTOFCODE_09_H_ */
