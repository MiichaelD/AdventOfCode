/*
  Link:         http://adventofcode.com/2020/day/13
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/13/2020
*/

#ifndef _2021_ADVENTOFCODE_13_H_
#define _2021_ADVENTOFCODE_13_H_

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

namespace aoc2021_13 {
using namespace std;

typedef uint64_t lli;
typedef pair<lli,size_t> Bus;

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

bool fillBusses(vector<Bus> *input) {
  string line;
  cin >> line;
  lli accum = 0;
  size_t bInd = 0;
  for (int index = 0; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ',') {
      if (accum != 0) {
        input->push_back({accum, bInd++});
        accum = 0;
      }
      continue;
    } else if (line[index] == 'x') {
      ++bInd;
      continue;
    }
  }
  if (accum != 0) {
    input->push_back({accum, bInd});
    accum = 0;
  }
  return true;
}

lli getEarliestShuttlePickup(const vector<Bus> &busses, lli earliestPickup) {
  lli nextPickup = INT64_MAX;
  int earliestBus = INT_MAX;
  for (const auto &busPair : busses) {
    lli bus = busPair.first;
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

lli getTimeT(const vector<Bus> &busses) {
  lli mInd = 0;
  for (int i = 1; i < busses.size(); ++i) {
    if (busses[i] > busses[mInd]) { 
      mInd = i;
    }
  }
  const Bus &maxBus = busses[mInd];
  lli maxVal = maxBus.first;
  cout << "Max Bus: " << maxVal << " @ index: " << busses[mInd].second << endl;

  for (int i = 0; i < busses.size(); ++i) {
    if (busses[i].first == 0) continue;
    const Bus &b = busses[i];
    cout << "\t" << b.first << " @ " << b.second << "\tGCD: ";
    cout << gcd(b.first, maxVal) << ", LCM: " << lcm(b.first, maxVal) << endl;
  }

  bool found;
  for (lli i = maxVal; true; i += maxVal) {
    found = true;
    for (size_t j = 0; j < busses.size(); ++j) {
      lli target = i - busses[mInd].second + busses[j].second;
      // cout << "Target: " << target << " @ " << busses[j] << endl;;
      if ((target % busses[j].first) > 0) {
        found = false;
        break;
      }
    }
    if (found) {
      cout << "After " << i / maxVal << " iterations... Done" << endl;
      return i - maxBus.second;
    }
  }
  return 0ll;
}

// https://github.com/Chrinkus/advent-of-code-2020/blob/main/src/day13.cpp
// Manually find a stride that is common for current bus and adjacent bus index.
lli solveChallenge(const vector<Bus> &boffs) {
  auto it = boffs.begin();
  lli mins = it->first, stride = mins;
  cout << "Bus [" << it->second << "]: Every :" << it->first << " mins" << endl;
  cout << "\tMins: " <<  mins << ", Stride: " << stride << endl;
  for (it = next(it); it != boffs.end(); ++it) {
    cout << "Bus [" << it->second << "]: " << it->first << endl;
    size_t times = 0;
    while ((mins + it->second) % it->first != 0) {
      cout << "\t\t" << (mins + it->second) << " % " << it->first << " = ";
      cout << (mins + it->second) % it->first << endl;
      mins += stride;
      ++times;
    }
    stride *= it->first;
    cout << "\t\t" << (mins + it->second) << " % " << it->first << " = ";
    cout << (mins + it->second) % it->first << endl;
    cout << "\tMins: " <<  mins << ", Stride: " << stride << ", Times: " << times << endl;
  }
  return mins;
}

void solve(int part = 1) {
  lli earliest, result = 0ll;
  cin >> earliest;  
  vector<Bus> busses;
  fillBusses(&busses);
  // sort(busses.begin(), busses.end(),
  //      [](const Bus &a, const Bus &b) {return a.first < b.first;});
  if (part == 1) {
    result = getEarliestShuttlePickup(busses, earliest);
  } else {
    result = solveChallenge(busses);
  }
  cout << "Result: " << result << endl;
}

};  // aoc2021_13

#endif /* _2021_ADVENTOFCODE_13_H_ */

/*

tuple<lli,lli,lli> egcd(lli a, lli b) {
  if (a == 0) {
    return {b, 0ll, 1ll};
  } else {
    tuple<lli,lli,lli> res = egcd(b % a, a);
    // const auto [g, x, y] = egcd(b % a, a);  // C++-17
    lli g = get<0>(res), x = get<1>(res), y = get<2>(res);
    return {g, y - (b / a) * x, x};
  }
}

lli mod_inv(lli a, lli n) {
  // x = gcd(a, n);
  lli x = get<1>(egcd(a, n));
  // const auto [g, x, y] = egcd(a, n);  // C++-17
  // x = lcm(x, n);
  return (x % n + n) % n;
}

// https://en.wikipedia.org/wiki/Chinese_remainder_theorem#Search_by_sieving
// https://www.youtube.com/watch?v=0dbXaSkZ-vc
lli chineseRemainderTheorem(const vector<Bus> &busses) {
  lli product = 1;
  for (size_t j = 0; j < busses.size(); ++j) {
    product *= busses[j].first;
  }
  lli accum = 0;

  for (size_t j = 0; j < busses.size(); ++j) {
    const Bus &bus = busses[j];
    lli p = product / bus.first;
    accum = accum +  bus.second  + mod_inv(p, bus.first) * p;
  }
  return accum % product;;
}

*/