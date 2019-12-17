/*
  Link:         http://adventofcode.com/2019/day/16
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/16/2019
*/

#ifndef _2019_ADVENTOFCODE_16_H_
#define _2019_ADVENTOFCODE_16_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc2019_16 {  
  using namespace std;

  const int TOTAL_PHASES = 100;

  template<class T>
  void printVector(const vector<T> &vec) {
    cout << "[";
    for (auto &elem : vec) {
      cout << elem << ",";
    }
    cout << "]";
  }

  template<class T>
  inline void printFirstKelements(const vector<T> &vec, int k) {
    for (int i = 0 ; i < k && i < vec.size(); ++i) {
      cout << vec[i];
    }
  }

  vector<int> getSignal(const string &input) {
    vector<int> signal;
    for (int i = 0 ; i < input.length(); ++i) {
      signal.push_back(input[i] - '0');
    }
    return signal;
  }

  int getPatternElement(const vector<int> &pattern, int index) {
    // Skip the first one, cyclic iterations.
    return pattern.at((index + 1) % pattern.size());
  }

  vector<int> generatePattern(const vector<int> &pattern, int signalIndex) {
    vector<int> output;
    for (int p : pattern) {
      for (int i = 0 ; i <= signalIndex; ++i) {
        output.push_back(p);
      }
    }
    return output;
  }

  int computePhaseDigit(const vector<int> &signal, const vector<int> &pattern) {
    int aux = 0, total = 0;
    for (int i = 0; i < signal.size(); ++i) {
      aux = signal[i] * getPatternElement(pattern, i);
      total += aux;
      // cout << signal[i] << " * "  << getPatternElement(pattern, i) << " = " << aux << "\t + ";
    }
    // cout << " = " << total << endl;
    return abs(total) % 10;
  }

  vector<int> computePhase(
      const vector<int> &signal, const vector<int> &initialPattern) {
    vector<int> output;
    for (int signalIndex = 0; signalIndex < signal.size(); ++signalIndex) {
      vector<int> pattern = generatePattern(initialPattern, signalIndex);
      output.push_back(computePhaseDigit(signal, pattern));
    }
    return output;
  }

  vector<int> computePhases(
      const vector<int> &signal, const vector<int> &initialPattern, int phaseCount) {
    vector<int> output = signal;
    for (int i = 0; i < phaseCount; ++i) {
      output = computePhase(output, initialPattern);
      // cout << "Phase " << i << ": "; printVector(output); cout << endl;
    }
    return output;
  }
  

  void solve1() {   
    const vector<int> initialPattern = {0, 1, 0, -1};
    string input;
    cin >> input;
    vector<int> signal = getSignal(input);
    vector<int> result = computePhases(signal, initialPattern, TOTAL_PHASES);

    cout << "Result for "; printVector(signal); cout << " after " << TOTAL_PHASES;
    cout << " iterations is: " << endl; printVector(result);
    cout << endl << "First 8 digits: "; printFirstKelements(result, 8); cout << endl;
    
  }

  void solve2() {    
    
  }

  void solve(int part = 1) {
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};

#endif /* _2019_ADVENTOFCODE_16_H_ */