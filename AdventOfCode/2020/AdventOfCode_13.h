/*
  Link:         http://adventofcode.com/2020/day/13
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/13/2020
*/

#ifndef _2020_ADVENTOFCODE_13_H_
#define _2020_ADVENTOFCODE_13_H_

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

namespace aoc2020_13 {
using namespace std;

typedef long long int lli;

lli gcd(lli a, lli b) {
  while(a != b)  {
    if(a > b)
        a -= b;
    else
        b -= a;
  }
  return a;
}

double lcm(lli a, lli b) {
  return ((double) a) * b / gcd(a, b);
}

bool fillBusses(vector<lli> *input, bool includeX=false) {
  string line;
  cin >> line;
  lli accum = 0;
  for (int index = 0; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ',') {
      if (accum != 0) {
        input->push_back(accum);
        accum = 0;
      }
      continue;
    } else if (line[index] == 'x') {
      if (includeX) {
        input->push_back(0ll);
      }
      continue;
    }
  }
  if (accum != 0) {
    input->push_back(accum);
    accum = 0;
  }
  return true;
}

lli getEarliestPickup(lli earliestPickup, lli bus) {
  return 0ll;
}

lli getEarliestShuttlePickup(const vector<lli> &busses, lli earliestPickup) {
  lli nextPickup = INT64_MAX;
  int earliestBus = INT_MAX;
  for (lli bus : busses) {
    lli result = earliestPickup / bus;
    if ((result * bus) < earliestPickup) {
      ++result;
    }
    if (result * bus < nextPickup) {
      // cout << "\t bus: " << bus <<  " @ " << result * bus << endl;
      nextPickup = result * bus;
      earliestBus = bus;
    }
  }
  return (nextPickup - earliestPickup) * earliestBus;
}

lli getTimeT(const vector<lli> &busses) {
  lli mInd = 0;
  for (int i = 1; i < busses.size(); ++i) {
    if (busses[i] > busses[mInd]) { 
      mInd = i;
    }
  }
  lli maxBus = busses[mInd];
  cout << "Max Bus: " << maxBus << " @ index: " << mInd << endl;

  for (int i = 0; i < busses.size(); ++i) {
    if (busses[i] == 0) continue;
    lli b = busses[i];
    cout << "\t" << b << " @ " << i << "\tGCD: " << gcd(b, maxBus) << ", LCM: ";
    cout << lcm(b, maxBus) << endl;
  }

  bool found;
  for (lli i = maxBus; true; i += maxBus) {
    found = true;
    for (size_t j = 0; j < busses.size(); ++j) {
      if (busses[j] == 0) continue;
      lli target = i - mInd + j;
      // cout << "Target: " << target << " @ " << busses[j] << endl;;
      if ((target % busses[j]) > 0) {
        found = false;
        break;
      }
    }
    if (found) {
      cout << "After " << i / maxBus << " iterations... Done" << endl;
      return i - mInd;
    }
  }
  return 0ll;
}

void solve(int part = 1) {
  lli earliest, result = 0ll;
  cin >> earliest;  
  vector<lli> busses;
  fillBusses(&busses, part != 1);
  if (part == 1) {
    result = getEarliestShuttlePickup(busses, earliest);
  } else {
    result = getTimeT(busses);
  }
  cout << "Result: " << result << endl;
}

};  // aoc2020_13

#endif /* _2020_ADVENTOFCODE_13_H_ */
