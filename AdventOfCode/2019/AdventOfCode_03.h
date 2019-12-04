/*
  Link:         http://adventofcode.com/2019/day/3
 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/03/2019
*/

#ifndef _2019_ADVENTOFCODE_03_H_
#define _2019_ADVENTOFCODE_03_H_

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <vector>
#include <utility>

namespace aoc2019_03 {  
  using namespace std;

  struct tuple_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1,T2> const &p) const {
      std::size_t h1 = std::hash<T1>()(p.first);
      std::size_t h2 = std::hash<T2>()(p.second);
      return h1 ^ h2;
    }
  };

  inline vector<string> processInput(const string &input) {
    vector<string> result;
    stringstream ss;
    for (int i = 0 ; i < input.length(); ++i) {
      if (input[i] == ',') {
        result.push_back(ss.str());
        ss.str(std::string());
      } else {
        ss << input[i];
      }
    }
    result.push_back(ss.str());
    return result;
  }

  inline pair<vector<string>, vector<string>> processInput(const string &i1, const string &i2) {
    return make_pair(processInput(i1), processInput(i2));
  }

  int getSteps(const string &path) {
    int steps = 0;
    for (int j = 1; j < path.length(); ++j) {
      steps *= 10;
      steps += path[j] - '0';
    }
    return steps;
  }

  pair<int,int> getOverlapInfo(
      unordered_map<pair<int,int>, int, tuple_hash> &coordenates, pair<int,int> &p, int steps) {
    auto coord = coordenates.find(p);
    if (coord != coordenates.end()) {
      auto paresito = make_pair(abs(p.first) + abs(p.second), steps + coord->second); 
      cout << "Overlap found: " << paresito.first << " & "<< paresito.second << endl;
      return paresito;
    }
    return make_pair(INT_MAX, INT_MAX);
  }

  void doOverlapStuff(
      unordered_map<pair<int,int>, int, tuple_hash> &coordenates,
      pair<int,int> &point,
      int steps,
      int &minDistance,
      int &minSteps) {
    pair<int,int> result = getOverlapInfo(coordenates, point, steps);
    if (result.first < minDistance) {
      cout << result.first << " < " << minDistance << ", updating dist [" << get<0>(point) << ", ";
      cout << get<1>(point) << "]" << endl;
      minDistance = result.first;
    }
    if (result.second < minSteps) {
      cout << result.second << " < " << minSteps << ", updating steps ["  << get<0>(point) << ", ";
      cout << get<1>(point) << "]" << endl;
      minSteps = result.second;
    }
  }

  inline void layCableDown(
      unordered_map<pair<int,int>, int, tuple_hash> &coordenates,
      pair<int,int> &&point,
      int steps) {
    if (coordenates.find(point) == coordenates.end()) {
      coordenates.insert(make_pair(point, steps));
      // cout << "Inserted point: [" << get<0>(point) << ", " << get<1>(point);
      // cout << "] - Total coordenates: " << coordenates.size() << endl;
    }
  }

  pair<int,int> getMinOverlap(const pair<vector<string>,vector<string>> &cables) {
    unordered_map<pair<int,int>, int, tuple_hash> coordenates;
    int x = 0, y = 0, totalSteps1 = 0;
    for (string path : cables.first) {
      int steps = getSteps(path);
      switch (path[0]) {
        case 'L':
          for (int s = 0; s < steps; ++s) {
            layCableDown(coordenates, make_pair(--x, y), ++totalSteps1);
            }
        break;
        case 'U':
          for (int s = 0; s < steps; ++s) {
            layCableDown(coordenates, make_pair(x, ++y), ++totalSteps1);
            }
        break;
        case 'R':
          for (int s = 0; s < steps; ++s) {
            layCableDown(coordenates, make_pair(++x, y), ++totalSteps1);
            }
        break;
        case 'D':
          for (int s = 0; s < steps; ++s) {
            layCableDown(coordenates, make_pair(x, --y), ++totalSteps1);
            }
          break;
        default:
        break;
      }
    }
    x = y = 0;
    int minDistance = INT_MAX, minSteps = INT_MAX, totalSteps2 = 0;
    for (string path : cables.second) {
      int steps = getSteps(path);
      cout << "Path: " << path << ". Steps: " << steps << endl;
      switch (path[0]) {
        case 'L':
          for (int s = 0; s < steps; ++s) { 
            auto p = make_pair(--x, y);
            doOverlapStuff(coordenates, p, ++totalSteps2, minDistance, minSteps);
          }
        break;
        case 'U':
          for (int s = 0; s < steps; ++s) { 
            auto p = make_pair(x, ++y);
            doOverlapStuff(coordenates, p, ++totalSteps2, minDistance, minSteps);
           }
        break;
        case 'R':
          for (int s = 0; s < steps; ++s) {
            auto p = make_pair(++x, y);
            // cout << "[" << x << ", " << y << "]" << endl;
            doOverlapStuff(coordenates, p, ++totalSteps2, minDistance, minSteps);
          }
        break;
        case 'D':
          for (int s = 0; s < steps; ++s) {
            auto p = make_pair(x, --y);
            doOverlapStuff(coordenates, p, ++totalSteps2, minDistance, minSteps);
          }
        break;
        default:
        break;
      }
    }
    return make_pair(minDistance, minSteps);
  }

  void solve(int part = 1) {
    string input1, input2;
    cin >> input1 >> input2;
    pair<vector<string>,vector<string>> cables = processInput(input1, input2);
    auto result = getMinOverlap(cables);
    cout << "Distance: " << result.first << ", Steps: " << result.second << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_03_H_ */
