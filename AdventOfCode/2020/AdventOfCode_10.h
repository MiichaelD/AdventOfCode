/*
  Link:         http://adventofcode.com/2020/day/10
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/10/2020
*/

#ifndef _2020_ADVENTOFCODE_10_H_
#define _2020_ADVENTOFCODE_10_H_

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

namespace aoc2020_10 {
using namespace std;


vector<size_t> getNumbers(bool initWithZero = false) {
  vector<size_t> numbers;
  if (initWithZero) {
    numbers.push_back(0);
  }
  size_t number;
  while(!cin.eof()){
    cin >> number;
    numbers.push_back(number);
  }
  sort(numbers.begin(), numbers.end());
  size_t deviceJolt = numbers.back() + 3;
  numbers.push_back(deviceJolt);
  return numbers;
}

bool populateJoltDeltas(const vector<size_t> &adapters, vector<int> &deltaCount) {
  size_t accum = 0;
  for (int i = 1 ; i < adapters.size(); ++i) {
    int delta = adapters[i] - accum;
    cout << adapters[i] << ", delta: " << delta << endl;
    if (delta > 3) {
      return false;
    }
    accum += delta;
    ++deltaCount[delta];
  }
  return true;
}

void print(const vector<int> &deltaCount) {
  for (int i = 0 ; i < deltaCount.size(); ++i) {
    if (deltaCount[i]) {
      cout << i << " :"  << deltaCount[i] << endl;
    }
  }
}

vector<size_t> getPossibleConnectionsByAdapter(const vector<size_t> &adapters) {
  vector<size_t> posConnections(adapters.size(), -1);
  for (size_t start = 0; start < adapters.size(); ++start) {
    for (size_t j = start + 1; j < posConnections.size(); ++j) {
      if (adapters[j] - adapters[start] > 3) {
        break;
      }
      ++posConnections[start];
    }
  }
  ++posConnections[adapters.size() - 1];
  return posConnections;
}

size_t getAdapterCombinations(
    const vector<size_t> &adapters,
    int start = 0,
    size_t accum = 0) {
  vector<size_t> conn = getPossibleConnectionsByAdapter(adapters);
  size_t last = conn[0], diffs = 0;
  int64_t delta = 0;
  for (size_t i = 0; i < conn.size(); ++i) {
    size_t nextInt = 0;
    cout << "conn: " << conn[i];
    if (i + 1 < conn.size()) {
      nextInt = conn[i + 1];
      delta = nextInt - conn[i];
      if (nextInt > 0 && delta == 0) {
        ++delta;
      }
      if (delta > 0) {
        diffs += delta;
        cout << "\tDiffs: " << diffs << ". Delta added: " << delta;
      }
    }
    cout << endl;
  }
  return 1 << diffs;
}

void printPermutations(
    const vector<size_t> &adapters,
    const vector<size_t> &conn,
    vector<size_t> &permutation,
    // vector<size_t> &memo,
    size_t &total,
    size_t start = 0) {
  if (start == adapters.size() - 1) {
    cout << "Permutation: [" << permutation[0];
    for (int i = 1; i < permutation.size(); ++i) {
      cout << ", " << permutation[i];
    }
    cout << ']' <<  endl;
    ++total;
    return;
  }
  permutation.push_back(adapters[start]);
  for (size_t j = start + 1; (j - start) <= conn[start] + 1; ++j) {
    // cout << adapters[start] << " -> " << adapters[j];
    printPermutations(adapters, conn, permutation, total, j);
  }
  permutation.pop_back();
}

void solve(int part = 1) {
  vector<size_t> adapters = getNumbers(true);
  if (part == 1) {
    vector<int> deltaCount = {0, 0, 0, 0};
    populateJoltDeltas(adapters, deltaCount);
    cout << "Part 1 - Finding Jolt Deltas count:" << endl;
    print(deltaCount);
  } else {
    vector<size_t> permutation;
    vector<size_t> conn = getPossibleConnectionsByAdapter(adapters);
    size_t total = 0;
    printPermutations(adapters, conn, permutation, total);
    // auto magic = getAdapterCombinations(adapters);
    cout << "Part 2 - permutations: " << total << endl;
  }
}

};  // aoc2020_10

#endif /* _2020_ADVENTOFCODE_10_H_ */
