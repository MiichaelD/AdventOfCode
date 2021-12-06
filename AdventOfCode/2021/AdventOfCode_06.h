/*
  Link:         http://adventofcode.com/2020/day/X
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/06/2020
*/

#ifndef _2021_ADVENTOFCODE_06_H_
#define _2021_ADVENTOFCODE_06_H_

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

namespace aoc2021_06 {  
using namespace std;

const int kWaitPerCreation = 7;
const int kInitialWaitPerCreation = kWaitPerCreation + 1;
const vector<int> daysOfInterest = {18, 80, 256};

size_t SimulateLanternFishesDay(const vector<int> &lanternFishes, int days) {
  uint64_t totalLF;
  vector<uint64_t> fishesPerDay(10, 0);
  for (int fishPeriod : lanternFishes) {
    ++fishesPerDay[fishPeriod];
  }
  for (int day = 0; day <= days; ++day) {
    bool interest = (day == daysOfInterest[0] || day == daysOfInterest[1] || day == daysOfInterest[2]);
    if (interest) cout << endl << "Day " << day << ":\t";
    uint64_t previousZero = fishesPerDay[0];
    totalLF = previousZero;
    for (int i = 1; i < fishesPerDay.size(); ++i) {
      totalLF += fishesPerDay[i];
      fishesPerDay[i - 1] = fishesPerDay[i];
      if (interest && fishesPerDay[i]) cout << fishesPerDay[i] <<  " x '" << i <<  "', ";
    }
    if (previousZero) {
      fishesPerDay[6] += previousZero;
      fishesPerDay[8] = previousZero;
      if (interest) cout << previousZero <<  " x '0', ";
    }
    if (interest) cout << endl << "Total Lantern Fishes in " << day << " days: " << totalLF << endl;
  }
  return totalLF;
}

void solve(int part = 1) {
  string input;
  cin >> input;
  vector<int> counters;
  for (int index = 0; index < input.length(); ++index) {
    int n = util::getNumberRef(input, index);
    counters.push_back(n);
  }
  cout << "Initial state:\t";
  util::printVector(counters);
  SimulateLanternFishesDay(counters, part == 1 ? 80 : 256);
}

};

#endif /* _2021_ADVENTOFCODE_06_H_ */