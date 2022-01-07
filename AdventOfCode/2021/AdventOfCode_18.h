/*
  Link:         http://adventofcode.com/2021/day/18
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/18/2021
*/

#ifndef _2021_ADVENTOFCODE_18_H_
#define _2021_ADVENTOFCODE_18_H_

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

namespace aoc2021_18 {
using namespace std;

uint64_t doMath(const string &, size_t &, bool);

uint64_t getNumber(const string &line, size_t &index, bool isAdvanced) {
  uint64_t accum = 0;
  // cout << "\tgetNumber: " << line[index] << endl;
  for (; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ' ') {
      break;
    } else if (line[index] == '(') {
      ++index;
      accum = doMath(line, index, isAdvanced);
      break;
    } else if (line[index] == ')') {
      break;
    } else if (isAdvanced && line[index] == '*') {
      index += 2;
      accum = doMath(line, index, isAdvanced);
      break;
    } 
  }
  // cout << "\tAccum: " << accum << endl;
  return accum;
}

uint64_t doMath(const string &line, size_t &index, bool isAdvanced) {
  uint64_t result = 0;
  for (; index < line.size(); ++index) {
    // cout << "Evaluating: " << line[index] << endl;
    if (isdigit(line[index])) {
      result += getNumber(line, index, isAdvanced);
      // cout << "Result: " << result << endl;
    } else if (line[index] == '+') {
      index += 2;
      result += getNumber(line, index, isAdvanced);
      // cout << "Result: " << result << endl;
    } else if (line[index] == '*') {
      if (!isAdvanced) {
        index += 2;
      }
      result *= getNumber(line, index, isAdvanced);
      // cout << "Result: " << result << endl;
      if (isAdvanced) {
        break;
      }
    } else if (line[index] == '(') {
      // ++index;
      result += getNumber(line, index, isAdvanced);
      // cout << "Result: " << result << endl;
    } else if (line[index] == ')') {
      break;
    } else if (line[index] == ' ') {
      break;
    }
  }
  // cout << "Result: " << result << endl;
  return result;
}

void solve(int part = 1) {
  uint64_t res = 0, aux;
  string line;
  size_t cases = 0;
  while(!cin.eof()) {
    getline(cin, line);
    size_t index = 0;
    aux = doMath(line, index, part != 1);
    res += aux;
    cout << "Result " << ++cases << ": " << aux << endl;// << endl;
  }
  cout << "Total Result: " << res << endl;
}

};  // aoc2021_18

#endif /* _2021_ADVENTOFCODE_18_H_ */
