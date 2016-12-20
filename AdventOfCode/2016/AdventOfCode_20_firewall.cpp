/*
 Description:   You'd like to set up a small hidden computer here so you can use
                it to get back into the network later. However, the corporate
                firewall only allows communication with certain external IP
                addresses.

                You've retrieved the list of blocked IPs from the firewall, but
                the list seems to be messy and poorly maintained, and it's not
                clear which IPs are allowed. Also, rather than being written in
                dot-decimal notation, they are written as plain 32-bit integers,
                which can have any value from 0 through 4294967295, inclusive.

                For example, suppose only the values 0 through 9 were valid, and
                that you retrieved the following blacklist:

                5-8
                0-2
                4-7

                The blacklist specifies ranges of IPs (inclusive of both the
                start and end value) that are not allowed. Then, the only IPs
                that this firewall allows are 3 and 9, since those are the only
                numbers not in any range.

                Given the list of blocked IPs you retrieved from the firewall
                (your puzzle input), what is the lowest-valued IP that is not
                blocked?

                Your puzzle answer was 19449262.

                --- Part Two ---

                How many IPs are allowed by the blacklist?

                Your puzzle answer was 119.
               
  Programmer:   Michael Duarte.

  Date:         12/20/2016
*/
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <vector>
#include <vector>
#include <string>
#include <cctype>
#include <tuple>
#include <regex>
#include <cmath>
#include <deque>

using namespace std;

typedef pair<uint32_t,uint32_t> PairOfInts;
const uint32_t UPPER_LIMIT = 4294967295;


void printExcludedRange(uint32_t lower, uint32_t higher, uint32_t diff,
  uint32_t total) {
  cout << "Exclusive from: " << lower  << " to: " << higher;
      cout << " = " << diff << ". Accum:\t" << total << endl;
}

uint32_t get1stAllowedIp(const vector<PairOfInts> &ranges) {
  const auto *pair = &ranges[0];
  if (pair->first){
    return 0;
  }

  for (uint32_t i = 1 ; i < ranges.size(); ++i) {
    const auto *next = &ranges[i];
    if (next->first-1 > pair->second) {
      return pair->second+1;
    } else {
      if (next->second-1 > pair->second)
        pair = next;
    }
  }
}

vector<PairOfInts> getAllAllowedIps(const vector<PairOfInts> &ranges) {
  vector<PairOfInts> result;
  const auto *pair = &ranges[0];
  if (pair->first){
    result.emplace_back(0, pair->first-1);
  }

  for (uint32_t i = 1 ; i < ranges.size(); ++i) {
    const auto *next = &ranges[i];
    if (next->first-1 > pair->second) {
      result.emplace_back(pair->second, next->first-1);
      pair = next;
    } else {
      if (next->second-1 > pair->second)
        pair = next;
    }
  }
  return result;
}

uint32_t getTotalAllowedIps(const vector<PairOfInts> &ranges) {
  const auto *pair = &ranges[0];
  uint32_t result = pair->first;
  for (uint32_t i = 1 ; i < ranges.size(); ++i) {
    const auto *next = &ranges[i];
    if (next->first-1 > pair->second) {
      uint32_t amount = (next->first - pair->second - 1);
      result += amount;
      printExcludedRange(pair->second, next->first, amount, result);
      pair = next;
    } else {
      if (next->second-1 > pair->second)
        pair = next;
    }
  }

  uint32_t remaining = (UPPER_LIMIT - pair->second);
  if (remaining){
    result += remaining;
    printExcludedRange(pair->second, UPPER_LIMIT, remaining, result);
  }
  return result;
}

bool sortRanges(const PairOfInts &p1, const PairOfInts &p2){
  if (p1.first == p2.first)
    return p1.second < p2.second;
  return p1.first < p2.first;
}

void printRanges(const vector<PairOfInts> &ranges) {
  for (auto &p : ranges) {
    cout << p.first << " - " << p.second << endl; 
  }
}

vector<PairOfInts> fillRanges() {
  vector<PairOfInts> ranges;
  ranges.reserve(1800);
  uint32_t lower, higher;
  char delimiter;
  while (!cin.eof()) {
    cin >> lower >> delimiter >> higher;
    ranges.emplace_back(lower, higher);
  }
  return ranges;
}

int main(){
  auto ranges = fillRanges();
  // printRanges(ranges);
  sort(ranges.begin(), ranges.end(), sortRanges);
  // printRanges(ranges);
  int firstAllowedIp = get1stAllowedIp(ranges);
  cout << "First Allowed Ip: " << firstAllowedIp << endl << endl;

  cout << "Total Allowed Ips: " << getTotalAllowedIps(ranges) << endl;
  return EXIT_SUCCESS;
}