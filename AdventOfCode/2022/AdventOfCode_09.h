/*
  Link:         http://adventofcode.com/2021/day/9
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2021
*/

#ifndef _2021_ADVENTOFCODE_09_H_
#define _2021_ADVENTOFCODE_09_H_

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

namespace aoc2021_09 {
using namespace std;

int getMinPointsSum(const vector<int> &mins) {
  int result = 0;
  for (int val : mins) {
    result += val + 1;
  }
  return result;
}

int top = 0;
int left = 1;
int bottom = 2;
int right = 3;
void fillBasins(
  vector<int> &result,
  unordered_set<pair<int,int>, util::pair_hash> &seen,
  const vector<vector<int>> &values, int i, int j) {
  int min = values[i][j];
  pair<int,int> point = {i, j};
  if (seen.find(point) != seen.end()) {
    // this point was seen before.
    return;
  }
  seen.insert(point);
  result.push_back(min);
  // cout << "Trying " << min << endl;
  if (i > 0 && values[i-1][j] >= min && values[i-1][j] != 9) { // top
    // result.push_back(values[i-1][j]);
    fillBasins(result, seen, values, i-1, j);
  }
  // cout << "\tleft" << endl;
  if (j > 0 && values[i][j-1] >= min && values[i][j-1] != 9) { // left
    // result.push_back(values[i][j-1]);
    fillBasins(result, seen, values, i, j-1);
  }
  // cout << "\tBottom" << endl;
  if (i < (values.size() - 1) && values[i+1][j] >= min && values[i+1][j] != 9) { // bottom
    // result.push_back(values[i+1][j]);
    fillBasins(result, seen, values, i+1, j);
  }
  // cout << "\tRight" << endl;
  if (j < (values[i].size() - 1) && values[i][j+1]>= min && values[i][j+1] != 9) { // bottom
    // result.push_back(values[i][j+1]);
    fillBasins(result, seen, values, i, j+1);
  }
  // cout << "\tThis is part of a basin: " << min << endl;
}

pair<vector<int>, vector<vector<int>>> getMinPoints(const vector<vector<int>> &values) {
  vector<int> mins;
  vector<vector<int>> basins;
  for (int i = 0; i < values.size(); ++i) {
    for (int j = 0; j < values[i].size(); ++j) {
      int value = values[i][j];
      // cout << "Trying " << value << " top" << endl;
      if (i > 0 && values[i-1][j] <= value) { // top
        continue;
      }
      // cout << "\tleft" << endl;
      if (j > 0 && values[i][j-1] <= value) { // left
        continue;
      }
      // cout << "\tBottom" << endl;
      if (i < (values.size() - 1) && values[i+1][j] <= value) { // bottom
        continue;
      }
      // cout << "\tRight" << endl;
      if (j < (values[i].size() - 1) && values[i][j+1] <= value) { // right
        continue;
      }
      // cout << "\t\tThis is a good one: " << value << endl;
      mins.push_back(value);
      vector<int> basin;
      unordered_set<pair<int,int>, util::pair_hash> seen;
      fillBasins(basin, seen, values, i, j);
      basins.push_back(basin);
    }
    // cout << endl;
  }
  sort(basins.begin(), basins.end(), 
      [](const vector<int>& a, const vector<int>& b) {
          return a.size() > b.size();
      }
  );
  return {mins, basins};
}

void solve(int part = 1) {
  vector<vector<int>> values;
  int lowest = 0, highest = 9;
  while(!cin.eof()) {
    string line;
    cin >> line;
    vector<int> vi;
    for (char c : line) { 
      cout << c;
      int val = c - '0';
      vi.push_back(val);
    }
    values.push_back(vi);
    cout << endl;
  }
  pair<vector<int>, vector<vector<int>>> minPoints = getMinPoints(values);
  // cout << "Total Basins: " << minPoints.second.size() << endl;
  // for (vector<int> &basin : minPoints.second) {
  //   sort(basin.begin(), basin.end());
  //   cout << endl << "Bassin size: " << basin.size() << ". " << endl << "\t";
  //   util::printVector(basin);
  // }

  size_t part2 = 1;
  for (int i = 0; i < 3; ++i) {
    part2 *= minPoints.second[i].size();
  }
  cout << "Total min spots: " << minPoints.first.size() << endl;
  cout << "Part 1 - Sum of min spots: " << getMinPointsSum(minPoints.first) << endl;
  cout << "Part 2 - Multiplying the 3 largest basin's sizes: " << part2 << endl;
}

};  // aoc2021_09

#endif /* _2021_ADVENTOFCODE_09_H_ */
