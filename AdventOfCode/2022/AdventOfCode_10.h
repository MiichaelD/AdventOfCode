/*
  Link:         http://adventofcode.com/2021/day/10
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/10/2021
*/

#ifndef _2021_ADVENTOFCODE_10_H_
#define _2021_ADVENTOFCODE_10_H_

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

namespace aoc2021_10 {
using namespace std;

int getCharPenalty(char c) {
  switch(c) {
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
    default:
      break;
  }
  return 0;
}


pair<string,size_t> getMissingChars(deque<char> &charStack) {
  stringstream ss;
  size_t score = 0;
  while(charStack.size()) {
    switch(charStack.back()) {
      case '(':
       ss << ')'; 
       score *= 5;
       score += 1;
       break;
      case '[':
       ss << ']';
       score *= 5;
       score += 2;
       break;
      case '{':
       ss << '}';
       score *= 5;
       score += 3;
       break;
      case '<':
       ss << '>';
       score *= 5;
       score += 4;
       break;
      default:
        break;
    }
    charStack.pop_back();
  }
  return {ss.str(), score};
}

void solve(int part = 1) {
  string line;
  deque<char> charStack;
  size_t penalty = 0;
  size_t illegalChars = 0;
  vector<size_t> completionScores;
  while (!cin.eof()) {
    cin >> line;
    char illegalChar = 0;
    for (int i = 0; i < line.size(); ++i) {
      switch(line[i]){
        case '(':
        case '[':
        case '{':
        case '<':
          charStack.push_back(line[i]);
          break;
        case ')':
          if (charStack.back() != '(') {
            illegalChar = line[i];
          } else {
            charStack.pop_back();
          }
          break;
        case ']':
          if (charStack.back() != '[') {
            illegalChar = line[i];
          } else {
            charStack.pop_back();
          }
          break;
        case '}':
          if (charStack.back() != '{') {
            illegalChar = line[i];
          } else {
            charStack.pop_back();
          }
          break;
        case '>':
          if (charStack.back() != '<') {
            illegalChar = line[i];
          } else {
            charStack.pop_back();
          }
          break;

      }
      if (illegalChar) {
        cout << "Found illegal char: "  << illegalChar << endl;
        ++illegalChars;
        penalty += getCharPenalty(illegalChar);
        break;
      }
    }
    if (!illegalChar) {
      pair<string,size_t> complement = getMissingChars(charStack);
      cout << "Completed " << line << " with: " << complement.first << ". Score: " << complement.second << endl;
      completionScores.push_back(complement.second);
    }
    charStack.clear();
  }
  sort(completionScores.begin(), completionScores.end());
  cout << "Part 1 - " << illegalChars << " syntax errors. Total error score: " << penalty << endl;
  cout << "Part 2 - " << completionScores.size() << " completed strings.";
  cout << "Middle score: " << completionScores[completionScores.size() / 2] << endl;
}

};  // aoc2021_10

#endif /* _2021_ADVENTOFCODE_10_H_ */
