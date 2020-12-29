/*
  Link:         http://adventofcode.com/2020/day/25
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/25/2020
*/

#ifndef _2020_ADVENTOFCODE_25_H_
#define _2020_ADVENTOFCODE_25_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <cstring>  // strcmp()
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

namespace aoc2020_25 {
using namespace std;

const uint32_t kRemainder= 20201227;
const int kInitialSubject = 7;

uint64_t transform(uint64_t subject = kInitialSubject, int loopSize = 1) {
  uint64_t result = 1l;
  for (int i = 1; i <= loopSize; ++i) {
    result *= subject;
    result %= kRemainder;
    // cout << "Result " << i << ": " << result << endl;
  }
  return result;
}

pair<uint64_t,int> transformUntilFound(uint64_t target) {
  pair<uint64_t,int> result{1l,0};
  for (int i = 1; true; ++i) {
    result.first *= kInitialSubject;
    result.first %= kRemainder;
    if (result.first == target) {
      result.second = i;
      return result;
    }
    // cout << "Result " << i << ": " << result << endl;
  }
  cout << "Not found" << endl;
  return result;
}


void solve(int part = 1) {
  uint64_t cardPK, doorPK;
  cin >> cardPK >> doorPK; 

  auto cardLS = transformUntilFound(cardPK);
  auto doorLS = transformUntilFound(doorPK);
  cout << "Card LS: " << cardLS.second << ", Door LS: " << doorLS.second << endl;

  uint64_t res = 0;
  if (cardLS.second < doorLS.second) {
    res = transform(doorPK, cardLS.second);
  } else {
    res = transform(cardPK, doorLS.second);
  }
  cout << "Encryption Key = " << res << endl;
}

};  // aoc2020_25 - 16457981

#endif /* _2020_ADVENTOFCODE_25_H_ */
