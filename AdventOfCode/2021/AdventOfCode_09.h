/*
  Link:         http://adventofcode.com/2021/day/9
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2021
*/

#ifndef _2021_ADVENTOFCODE_09_H_
#define _2021_ADVENTOFCODE_09_H_

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

namespace aoc2021_09 {
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

pair<int,int> findNumbersAddingUpTo(
    const vector<unsigned long long> numbers, int index) {
  for (size_t i = index - preamble; i <= index; ++i) {
    int n = numbers[i];
    // cout << "\ti = " << n << endl;
    for (size_t j = i + 1; j <= index; ++j) {
      // cout << "\t\tj = " << numbers[j] << endl;
      if (n + numbers[j] == numbers[index]) {
        // cout << "Found pair adding up to " << numbers[index] << ". i:"
        //     << numbers[i] << ", j: " << numbers[j] << endl; 
        return make_pair(n, numbers[j]);
      }
    }
  }
  return make_pair(-1, -1);
}

size_t findFirstNumberIndexNotAddingUp(
    const vector<unsigned long long> numbers) {
  for (size_t i = preamble; i < numbers.size(); ++i) {
    // cout << "Trying to find numbers adding to " << numbers[i] << endl;
    pair<int,int> p = findNumbersAddingUpTo(numbers, i);
    if (p.first == -1 || p.second == -1) {
      return i;
    }
  }
  return -1;
}

pair<unsigned long long, unsigned long long> getMinAndMaxFromRange(
    const vector<unsigned long long> numbers,
    unsigned long long  i,
    unsigned long long  j) {
  unsigned long long minimum = SIZE_MAX, maximum = 0;
  for (;i <= j; ++i) {
    minimum = min(minimum, numbers[i]);
    maximum = max(maximum, numbers[i]);
  }
  return make_pair(minimum, maximum);
}

pair<unsigned long long, unsigned long long> setRangeToInvalidNumber(
    const vector<unsigned long long> numbers, size_t invalidIndex) {
  unsigned long long target = numbers[invalidIndex];
  unsigned long long i = 0, j = 1, result = numbers[i] + numbers[j];
  while (true) {
    if (result == target) {
      return getMinAndMaxFromRange(numbers, i, j);
    }
    if (result > target) {
      result -= numbers[i];
      ++i;
    } else {
      result += numbers[++j];
    }
  }
  return make_pair(0, 0);
}

void solve(int part = 1) {
  preamble = 25;
  vector<unsigned long long> numbers = getNumbers();
  size_t invalidNumberIndex = findFirstNumberIndexNotAddingUp(numbers);
  cout << "Part 1 - Invalid number: " << numbers[invalidNumberIndex]  << endl;
  pair<unsigned long long, unsigned long long> minAndMaxInSet =
      setRangeToInvalidNumber(numbers, invalidNumberIndex);
  cout << "Part 2 - min & max: "; util::printPair(minAndMaxInSet);
  cout << "Adds up: " << (minAndMaxInSet.first + minAndMaxInSet.second) << endl;
}

};  // aoc2021_09

#endif /* _2021_ADVENTOFCODE_09_H_ */
