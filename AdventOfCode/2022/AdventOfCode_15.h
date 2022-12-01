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

const size_t dimensionMultiplier = 5;

typedef vector<vector<pair<int,int>>> Matrix; //Matrix<index, sum>
typedef pair<int,int> Coordinate;
typedef pair<int,int> Tile;

int totalPaths = 0;
size_t minRiskPathTotal = INT_MAX;
vector<int> minRiskPath;
unordered_set<Coordinate, util::pair_hash> minRiskPathCoordinates;

inline pair<int,int>& getValue(Matrix &data, const Coordinate &coordinate) {
  return data[coordinate.second][coordinate.first];
}

Matrix getInput(int part = 1) {
  string line;
  Matrix result;
  int lines = 0;
  while(!cin.eof()) {
    cin >> line;
    int matrixDimension = (part == 1 ? line.size() : line.size() * dimensionMultiplier);
    int dimMultiplier = part == 1 ? 1 : matrixDimension;
    // result.reserve(matrixDimension);
    if (part == 1) {
      result.emplace_back(vector<pair<int,int>>(matrixDimension));
    } else if (part == 2 && lines == 0) {
      for (int i = 0 ; i < matrixDimension; ++i) {
        result.emplace_back(vector<pair<int,int>>(matrixDimension));
      }
    }
    cout << "Matrix size: rows: " << result.size() << ", cols: " << result.back().size() << endl; 
    for (int i = 0; i < line.size(); ++i) {
      int value = line.at(i) - '0';
      result[lines][i].first = value;
      result[lines][i].second = INT_MAX; 
      if (part == 1) {
        continue;
      }
      for (int tRow = 0; tRow < dimensionMultiplier; ++tRow) {
        for (int tCol = tRow == 0 ? 1 : 0; tCol < dimensionMultiplier; ++tCol) {
          int delta = tRow + tCol;
          int temp = (value + delta);
          if (temp >= 10) {
            temp -= 9;
          }
          int tileRow = (tRow * line.size());
          int tileCol = (tCol * line.size());
          result[lines + tileRow][i + tileCol].first = temp;
          result[lines + tileRow][i + tileCol].second = INT_MAX;
        } 
      }
    }
    ++lines;
  }
  return result;
}

void print(const Matrix &data) {
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < data[i].size(); ++j) {
      cout << data[i][j].first;
    }
    cout << endl;
  }
}

void printDots(const Matrix &data) {
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < data[i].size(); ++j) {
      cout << data[i][j].first;
      cout << (minRiskPathCoordinates.contains({j,i}) ? "█" : ".");
    }
    cout << endl;
  }
}

void printSum(const Matrix &data) {
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < data[i].size(); ++j) {
      cout << data[i][j].second<< "\t";
    }
    cout << endl;
  }
}

// This actually gets the path, works for test cases and part1 but not part 2.
// Very slow, no memoization. E.g. Updates all steps in pre-computed paths when finding a shorter path.
void compute(Matrix &data, const Coordinate &goal, const Coordinate &coordinate,
    unordered_set<Coordinate, util::pair_hash> &memo, vector<int> &path, int &sum) {
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
  memo.insert(coordinate);
  if (goal == coordinate) {
    ++totalPaths;
    if (sum < minRiskPathTotal) {
      cout << "\tMin Risk Path found w/ sum: " << sum << ". Size: " << path.size() << endl;
      minRiskPathTotal = sum;
      minRiskPath = path;
      minRiskPathCoordinates = memo;
      // util::printVector(path);
      cout << endl;
    }
  } else {
    Coordinate aux; 
    if (coordinate.second < data.size() - 1) { // Bottom
      aux = {coordinate.first, coordinate.second + 1};
      compute(data, goal, aux, memo, path, sum);
    }
    if (coordinate.first < data[coordinate.second].size() - 1) {  // Right
      aux = {coordinate.first + 1, coordinate.second};
      compute(data, goal, aux, memo, path, sum);
    }
    if (coordinate.first > 0) {  // Left
      aux = {coordinate.first - 1, coordinate.second};
      compute(data, goal, aux, memo, path, sum);
    }
    if (coordinate.second > 0) { // Top
      aux = {coordinate.first, coordinate.second - 1};
      compute(data, goal, aux, memo, path, sum);
    }
  }
  sum -= value;
  memo.erase(coordinate);
  if (path.size()) path.pop_back();
}

int findPath(Matrix &data) {
  minRiskPathTotal = INT_MAX;
  Coordinate start(0, 0), goal(data.size() - 1, data.back().size() - 1);
  unordered_set<Coordinate, util::pair_hash> memo;
  vector<int> path;
  int sum = 0;
  compute(data, goal, start, memo, path, sum);
  cout << "Total Paths: " << totalPaths << endl;
  printDots(data);
  return minRiskPathTotal - data[0][0].first;
}


// Dijkstra-ish solution, visiting closer neighboors and keeping track of minimum cost.
void compute2(Matrix &data, const Coordinate &goal, const Coordinate &coordinate,
    int sum, deque<pair<int,Coordinate>> &nextCoordinates) {
  if (coordinate.second >= data.size()
     || coordinate.first >= data[coordinate.second].size()) {
    return;
  }

  // If new path here is shorter than previously known, update. Else return.
  auto &cell = getValue(data, coordinate);
  int value = cell.first;
  // sum += value;
  if (sum < cell.second) {
    cell.second = sum;
    // cout << "\tUpdating: ";
    // util::printPair(coordinate);
    // cout << " Value: " << value << " Sum: " << sum << endl;
  } else {
    return;
  }

  if (goal == coordinate) {
    if (sum < minRiskPathTotal) {
      cout << "\tMin Risk Path found w/ sum: " << sum << endl;
      minRiskPathTotal = sum;
      cout << endl;
    }
  } else {
    Coordinate aux; 
    if (coordinate.second < data.size() - 1) { // Bottom
      aux = {coordinate.first, coordinate.second + 1};
      nextCoordinates.emplace_back(sum + getValue(data, aux).first, aux);
    }
    if (coordinate.first < data[coordinate.second].size() - 1) {  // Right
      aux = {coordinate.first + 1, coordinate.second};
      nextCoordinates.emplace_back(sum + getValue(data, aux).first, aux);
    }
    if (coordinate.first > 0) {  // Left
      aux = {coordinate.first - 1, coordinate.second};
      nextCoordinates.emplace_back(sum + getValue(data, aux).first, aux);
    }
    if (coordinate.second > 0) { // Top
      aux = {coordinate.first, coordinate.second - 1};
      nextCoordinates.emplace_back(sum + getValue(data, aux).first, aux);
    }
  }
}

int calculatePath(Matrix &data) {
  minRiskPathTotal = INT_MAX;
  Coordinate start(0, 0);
  Coordinate goal(data.size() - 1, data.back().size() - 1);
  Coordinate test(2, 2);
  deque<pair<int,Coordinate>> nextCoordinates;
  int sum = 0;
  nextCoordinates.emplace_back(0, start);
  while (nextCoordinates.size()) {
    pair<int,Coordinate> p1 = nextCoordinates.front();
    nextCoordinates.pop_front();
    // cout << "Next coordinate w/ cost of: " << p1.first << ". "; util::printPair(p1.second);
    // cout <<endl;
    compute2(data, goal, p1.second, p1.first, nextCoordinates);
    sort(nextCoordinates.begin(), nextCoordinates.end(),
         [](const pair<int,Coordinate> &p1, const pair<int,Coordinate> &p2) {
           return p1.first < p2.first;
         });
  }
  // return minRiskPathTotal - data[0][0].first;
  // return data[test.second][test.first].second;
  return data[goal.second][goal.first].second;
}

void solve(int part = 1) {
  Matrix input = getInput(part);
  print(input);
  int result = calculatePath(input);
  // printSum(input);
  // util::printVector(minRiskPath);
  cout << endl;
  // printDots(input);
  cout << endl << "Part 1: Path with sum of: " << result  << "." << endl;
  // util::printVector(minRiskPath);
}

};  // aoc2021_15

#endif /* _2021_ADVENTOFCODE_15_H_ */
