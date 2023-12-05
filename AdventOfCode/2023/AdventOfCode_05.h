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

unordered_map<uint64_t,uint64_t> getMap() {
  unordered_map<uint64_t,uint64_t> map;
  string input;
  uint64_t dest_start, source_start, range;
  while(getline(cin, input)) {
    if (input.size() == 0) break;
    sscanf(input.c_str(), "%lld %lld %lld", &dest_start, &source_start, &range);
    for (uint64_t i = 0 ; i < range; ++i) {
      map[source_start + i] = dest_start + i;
    }
  }
  cout << "Map Generated with " << map.size() << " entries" << endl;
  return map;
}

void solve1a() {
  string input;
  cin >> input;
  getline(cin, input);
  vector<uint64_t> seeds;
  for (int i = 1; i < input.size(); ++i) {
    seeds.push_back(util::getNumberRef<uint64_t>(input, i));
    cout << "Seeds: " << seeds.back() << endl;
  }
  vector<unordered_map<uint64_t,uint64_t>> maps;
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

bool sortByFirst(
  const tuple<uint64_t, uint64_t, uint64_t>& a, 
  const tuple<uint64_t, uint64_t, uint64_t>& b) {
    return get<0>(a) < get<0>(b);
}

void print(const vector<tuple<uint64_t, uint64_t, uint64_t>>& map) {
  for (const auto& t : map) {
    cout << "source: " <<  get<0>(t) << ", dest: " <<  get<1>(t) << ", range: "
         << get<2>(t) << endl;
  }
  cout << endl;
}

vector<tuple<uint64_t, uint64_t, uint64_t>> getMapTuple() {
  vector<std::tuple<uint64_t, uint64_t, uint64_t>> map;
  string input;
  uint64_t dest_start, source_start, range;
  while(getline(cin, input)) {
    if (input.size() == 0) break;
    sscanf(input.c_str(), "%lld %lld %lld", &dest_start, &source_start, &range);
    map.emplace_back(source_start, dest_start, range);
  }
  sort(map.begin(), map.end(), sortByFirst);
  cout << "Map Generated with " << map.size() << " entries" << endl;
  print(map);
  return map;
}

uint64_t convert(const vector<tuple<uint64_t, uint64_t, uint64_t>>& map, uint64_t in) {
  for (const auto& t : map) {
    if (in >= get<0>(t) && in < (get<0>(t) + get<2>(t))) {
      uint64_t delta = in - get<0>(t);
      // cout << "\t converting: " << in << " to: " << (get<1>(t) + delta) << endl;
      return (get<1>(t) + delta);
    }
  }
  // cout << "\t convertion for " << in << " not found." << endl;
  return in;
}

vector<vector<tuple<uint64_t, uint64_t, uint64_t>>> getMaps() {
  string input;
  vector<vector<tuple<uint64_t, uint64_t, uint64_t>>> maps;
  getline(cin, input);  // Empty line
  getline(cin, input);  // seed-to-soil map:
  maps.emplace_back(getMapTuple());
  getline(cin, input);  // soil-to-fertilizer map:
  maps.emplace_back(getMapTuple());
  getline(cin, input);  // fertilizer-to-water map:
  maps.emplace_back(getMapTuple());
  getline(cin, input);  // water-to-light map:
  maps.emplace_back(getMapTuple());
  getline(cin, input);  // light-to-temp map:
  maps.emplace_back(getMapTuple());
  getline(cin, input);  // temp-to-humidity map:
  maps.emplace_back(getMapTuple());
  getline(cin, input);  // humidity-to-location map:
  maps.emplace_back(getMapTuple());
  return maps;
}

void solve1() {
  string input;
  cin >> input;
  getline(cin, input);
  vector<uint64_t> seeds;
  for (int i = 1; i < input.size(); ++i) {
    seeds.push_back(util::getNumberRef<uint64_t>(input, i));
    cout << "Seeds: " << seeds.back() << endl;
  }
  vector<vector<tuple<uint64_t, uint64_t, uint64_t>>> maps = getMaps();
  cout << "Finding locations." << endl;
  uint64_t solution = UINT_MAX;
  for (auto seed : seeds) {
    uint64_t aux = seed;
    for (const auto& map : maps) {
      aux = convert(map, aux);
    }
    cout << "Seed: " << seed << " has location: " << aux << endl;
    solution = std::min(aux, solution);
  }
  cout << "Lowest location number: " << solution << endl;
}

void solve2() {
  string input;
  cin >> input;
  getline(cin, input);
  vector<pair<uint64_t,uint64_t>> seeds;
  for (int i = 1; i < input.size(); ++i) {
    seeds.emplace_back(
        util::getNumberRef<uint64_t>(input, i),
        util::getNumberRef<uint64_t>(input, ++i));
    cout << "Seeds: " << seeds.back().first << " for range: "
         << seeds.back().second << endl;
  }
  vector<vector<tuple<uint64_t, uint64_t, uint64_t>>> maps = getMaps();
  cout << "Finding locations." << endl;
  uint64_t solution = UINT_MAX;
  for (const auto& seed : seeds) {
    cout << "pair: <" << seed.first << ", " << seed.second << ">" << endl;
    for (uint64_t s = seed.first; s < (seed.first + seed.second); ++s) {
      uint64_t aux = s;
      // cout << "Seed: " << s;
      for (const auto& map : maps) {
        aux = convert(map, aux);
      }
      // cout << " has location: " << aux << endl;
      solution = std::min(aux, solution);
    }
  }
  cout << "Lowest location number: " << solution << endl;
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
