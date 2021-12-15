/*
  Link:         http://adventofcode.com/2021/day/15
  Compiling:    g++ -std=c++20 main.cpp -o main && cat 2021/AdventOfCode_15_input.txt | ./main
  Programmer:   Michael Duarte.
  Date:         12/14/2020
*/

#ifndef _2021_ADVENTOFCODE_15_H_
#define _2021_ADVENTOFCODE_15_H_

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

namespace aoc2021_15 {
using namespace std;

typedef vector<vector<pair<int,int>>> Matrix; //Matrix<index, sum>
typedef pair<int,int> Coordinate;
typedef pair<int,int> Tile;

int totalPaths = 0;
size_t minRiskPathTotal = INT_MAX;
vector<int> minRiskPath;

Matrix getInput() {
  string line;
  Matrix result;
  while(!cin.eof()) {
    cin >> line;
    result.emplace_back(vector<pair<int,int>>(line.size()));
    for (int i = 0; i < line.size(); ++i) {
      result.back()[i].first = line.at(i) - '0'; 
      result.back()[i].second = INT_MAX; 
    }
  }
  return result;
}

void print (const Matrix &data) {
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < data[i].size(); ++j) {
      cout << data[i][j].first;
    }
    cout << endl;
  }
}

void compute(Matrix &data, const Coordinate &goal, const Coordinate &coordinate,
    unordered_map<Coordinate, int, util::pair_hash> &memo, vector<int> &path, int &sum) {
  // cout << "Checking coordinate: "; util::printPair(coordinate, true);
  auto &cell = data[coordinate.second][coordinate.first];
  int value = cell.first;
  if (sum < cell.second) {
    cell.second = sum;
  } else {
    return;
  }
  path.push_back(value);
  sum += value;
  if (goal == coordinate) {
    ++totalPaths;
    if (sum < minRiskPathTotal) {
      cout << "\tMin Risk Path found w/ sum: " << sum << endl;
      minRiskPathTotal = sum;
      minRiskPath = path;
      util::printVector(path);
      cout << endl;
    }
  } else {
    Coordinate aux; 
    if (coordinate.first < data[coordinate.second].size() - 1) {  // Left
      aux = {coordinate.first + 1, coordinate.second};
      compute(data, goal, aux, memo, path, sum);
    }
    if (coordinate.second < data.size() - 1) { // Bottom
      aux = {coordinate.first, coordinate.second + 1};
      compute(data, goal, aux, memo, path, sum);
    }
  }
  sum -= value;
  path.pop_back();
}

int findPath(Matrix &data) {
  minRiskPathTotal = INT_MAX;
  Coordinate start(0, 0), goal(data.size() - 1, data.back().size() - 1);
  unordered_map<Coordinate, int, util::pair_hash> memo;
  vector<int> path;
  int sum = 0;
  compute(data, goal, start, memo, path, sum);
  cout << "Total Paths: " << totalPaths << endl;
  return minRiskPathTotal - data[0][0].first;
}

void solve(int part = 1) {
  size_t kTotalTurns = part == 1 ? 2020 : 30000000;
  Matrix input = getInput();
  print(input);
  int result = findPath(input);
  cout << "Part 1: Path with sum of: " << result  << " and size: " << minRiskPath.size() << "." << endl;
  // util::printVector(minRiskPath);
}

};  // aoc2021_15

#endif /* _2021_ADVENTOFCODE_15_H_ */
