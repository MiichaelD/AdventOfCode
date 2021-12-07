/*
  Link:         http://adventofcode.com/2020/day/07
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/06/2021
*/

#ifndef _2021_ADVENTOFCODE_07_H_
#define _2021_ADVENTOFCODE_07_H_

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

namespace aoc2021_07 {
using namespace std;


void solve(int part = 1) {
  string input;
  cin >> input;
  vector<int> positions;
  int sum = 0;
  int min = INT_MAX, max = INT_MIN;
  for (int index = 0; index < input.length(); ++index) {
    int position = util::getNumberRef(input, index);
    positions.push_back(position);
    sum += position;
    if (position < min) min = position;
    if (position > max) max = position;
  }
  // cout << "Initial state:\t";
  // sort(positions.begin(), positions.end());
  // util::printVector(positions);
  // cout << "Sum: " << sum << ". Total items: " << positions.size() << endl;

 size_t minIndex1 = 0, minIndex2 = 0;
//  vector<uint64_t> memo(1 + max - min);  // I thought we were gonna need memoization.
  int min1 = INT_MAX, min2 = INT_MAX;
  for (int i = min; i <= max; ++i) {
    int totalEnergy = 0;
    int totalEnergy2 = 0;
    for (int pos : positions) {
      int energy = abs(pos - i);
      totalEnergy += energy;
      int energy2 = (energy * (energy + 1)) / 2;
      totalEnergy2 += energy2;
    }
    // cout << "Energy needed to reach position " << i << ": " << energy;
    if (totalEnergy < min1) {
      // cout << "\t <-- This is the position with the current minimum energy needed! ";
      min1 = totalEnergy;
      minIndex1 = i;
    }
    if (totalEnergy2 < min2) {
      min2 = totalEnergy2;
      minIndex2 = i;
    }
    // cout << endl;
  }
  cout << "For part 1: \t";
  cout << minIndex1 << " Is the position w/ the minimum energy consumption of: " << min1 << endl;
  cout << "For part 2: \t";
  cout << minIndex2 << " Is the position w/ the minimum energy consumption of: " << min2 << endl;
}

};  // aoc2021_07

#endif /* _2021_ADVENTOFCODE_07_H_ */
