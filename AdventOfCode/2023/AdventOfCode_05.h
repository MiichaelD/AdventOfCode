/*
  Link:         http://adventofcode.com/2023/day/05
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/05/2023
*/

#ifndef _2023_ADVENTOFCODE_05_H_
#define _2023_ADVENTOFCODE_05_H_

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

namespace aoc2023_05 {
using namespace std;

  // string soil, fertilizer;
  // getline(cin, soil);
  // getline(cin, fertilizer);
  // unordered_map<int,int> seedToSoil;
  // for (int i = 0, j = 0; i < soil.size() && j < fertilizer.size(); ++i, ++j) {
  //   seedToSoil[util::getNumberRef(soil, i)] = util::getNumberRef(fertilizer, j);
  // }


unordered_map<size_t,size_t> getMap() {
  unordered_map<size_t,size_t> map;
  string input;
  size_t dest_start, source_start, range;
  while(getline(cin, input)) {
    if (input.size() == 0) break;
    sscanf(input.c_str(), "%zd %zd %zd", &dest_start, &source_start, &range);
    for (size_t i = 0 ; i < range; ++i) {
      map[source_start + i] = dest_start + i;
    }
  }
  cout << "Map Generated with " << map.size() << " entries" << endl;
  return map;
}

void solve1() {
  string input;
  cin >> input;
  getline(cin, input);
  vector<size_t> seeds;
  for (int i = 1; i < input.size(); ++i) {
    seeds.push_back(util::getNumberRef<size_t>(input, i));
    cout << "Seeds: " << seeds.back() << endl;
  }
  vector<unordered_map<size_t,size_t>> maps;
  getline(cin, input);  // Empty line
  getline(cin, input);  // seed-to-soil map:
  maps.emplace_back(getMap());
  getline(cin, input);  // soil-to-fertilizer map:
  maps.emplace_back(getMap());
  getline(cin, input);  // fertilizer-to-water map:
  maps.emplace_back(getMap());
  getline(cin, input);  // water-to-light map:
  maps.emplace_back(getMap());
  getline(cin, input);  // light-to-temp map:
  maps.emplace_back(getMap());
  getline(cin, input);  // temp-to-humidity map:
  maps.emplace_back(getMap());
  getline(cin, input);  // humidity-to-location map:
  maps.emplace_back(getMap());

  int solution = 1000;
  for (auto seed : seeds) {
    int aux = seed;
    for (const auto& map : maps) {
      const auto it = map.find(aux);
      if (it != map.end()) {
        aux = it->second;
      }
    }
    solution = std::min(aux, solution);
  }
  cout << "Lowest location number: " << solution << endl;
}

void solve2() {
  string input;
  cin >> input;
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } else {
    solve2();
  }
}

};  // aoc2023_05

#endif /* _2023_ADVENTOFCODE_05_H_ */
