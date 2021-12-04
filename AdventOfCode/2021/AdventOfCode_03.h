/*
  Link:         http://adventofcode.com/2020/day/3
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/03/2020
*/

#ifndef _2021_ADVENTOFCODE_03_H_
#define _2021_ADVENTOFCODE_03_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "../util/util.h"


namespace aoc2021_03 {
  using namespace std;

  pair<string,string> getGammaAndEpsilon(const vector<int> &reps, size_t total) {
    stringstream gamma, epsilon;
    size_t common = total / 2;
    for (const int element : reps) {
      bool popularBit = element >= common;
      gamma << (popularBit ? '1' : '0');
      epsilon << (popularBit ? '0' : '1');
    }
    return {gamma.str(), epsilon.str()};
  }

  size_t solve1() {
    string input;
    vector<int> repetitions;
    int totalEntries = 0;
    while(!cin.eof()) {
      cin >> input;
      if (input.empty()) {
        break;
      }
      ++totalEntries;
      if (repetitions.size() < input.size()) {
        repetitions.resize(input.size());
      }
      // for (int i = input.size() - 1; i >= 0; --i) {
      for (int i = 0; i < input.size(); ++i) {
        repetitions[i] += input[i] == '1' ? 1 : 0;
      }
    }
    util::printVector(repetitions);
    auto gammaAndEpsilon = getGammaAndEpsilon(repetitions, totalEntries);
    util::printPair(gammaAndEpsilon, true);
    uint64_t gamma = util::binaryToDecimal(gammaAndEpsilon.first);
    uint64_t epsilon = util::binaryToDecimal(gammaAndEpsilon.second);
    cout << "Gamma: " << gamma << ". Epsilon: " << epsilon << endl;
    return gamma * epsilon;
  }

  size_t solve2() {string input;
    vector<int> repetitions;
    int totalEntries = 0;
    while(!cin.eof()) {
      cin >> input;
      if (input.empty()) {
        break;
      }
      ++totalEntries;
      if (repetitions.size() < input.size()) {
        repetitions.resize(input.size());
      }
      // for (int i = input.size() - 1; i >= 0; --i) {
      for (int i = 0; i < input.size(); ++i) {
        repetitions[i] += input[i] == '1' ? 1 : 0;
      }
    }
    util::printVector(repetitions);
    auto gammaAndEpsilon = getGammaAndEpsilon(repetitions, totalEntries);
    util::printPair(gammaAndEpsilon, true);
    uint64_t gamma = util::binaryToDecimal(gammaAndEpsilon.first);
    uint64_t epsilon = util::binaryToDecimal(gammaAndEpsilon.second);
    cout << "Gamma: " << gamma << ". Epsilon: " << epsilon << endl;
    return gamma * epsilon;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2021_ADVENTOFCODE_03_H_ */
