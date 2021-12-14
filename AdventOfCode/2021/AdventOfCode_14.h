/*
  Link:         http://adventofcode.com/2020/day/14
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/13/2021
*/

#ifndef _2021_ADVENTOFCODE_14_H_
#define _2021_ADVENTOFCODE_14_H_

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

namespace aoc2021_14 {
using namespace std;

typedef pair<unordered_map<string,uint64_t>,vector<uint64_t>> RepsPair;

void print(const RepsPair &repsPair) {
  for (const auto &entry : repsPair.first) {
    util::printPair(entry);
  }
  uint64_t len = 0;
  cout << endl << "\tchars: ";
  for (int i = 0; i < repsPair.second.size(); ++i) {
    if (repsPair.second[i] == 0) {
      continue;
    } 
    cout << static_cast<char>(i + 'A') << " - " << repsPair.second[i] << ", ";
    len += repsPair.second[i];
  }
  cout << endl << "\tstring length: "<< len << endl << endl;
}

pair<string,unordered_map<string,char>> getInput() {
  pair<string,unordered_map<string,char>> result;
  cin >> result.first;
  while(!cin.eof()) {
    string s, line;
    cin >> s;
    getline(cin, line);
    result.second[s] = line.at(4);
    cout << "\t" << s << ": " << line.at(4) << endl;
  }
  return result;
}

RepsPair processInput(const string &tmpl) {
  unordered_map<string,uint64_t> stringReps;
  vector<uint64_t> charsReps(26);
  for (int i = 0; i < tmpl.size() - 1; ++i) {
    char c = tmpl.at(i);
    string aux = tmpl.substr(i, 2); // start and size
    ++stringReps[aux];
    ++charsReps[c - 'A'];
  }
  ++charsReps[tmpl.back() - 'A'];
  return {stringReps, charsReps};
}

RepsPair processReps(const RepsPair &rp, const unordered_map<string,char> &insertionRules) {
  RepsPair result;
  result.second = rp.second;
  for (const pair<string,uint64_t>& entry : rp.first) {
    const string &str = entry.first;
    uint64_t reps = entry.second;
    if (insertionRules.contains(str)) {
      cout << "\t\tRemoving: " << str << ". Adding: ";
      char c = insertionRules.at(str);
      // add the inserted char
      result.second[c - 'A'] += reps;

      // insert new prefix;
      stringstream prefix;
      prefix << str.front() << c;
      result.first[prefix.str()] += reps;

      // insert new suffix;
      stringstream suffix;
      suffix << c << entry.first.back();
      result.first[suffix.str()] += reps;
      cout << prefix.str() << ", " << suffix.str() << " x " << reps << " times" << endl;

      // Remove all this entry as it has been processed {entry.second} times.
      // result.first.erase(str);
    }
  }
  return result;
}

RepsPair process(const string &tmpl, const unordered_map<string,char> &insertionRules, int steps) {
  RepsPair repsPair = processInput(tmpl);
  cout << "step 0:\t";
  print(repsPair);
  for (int i = 1; i <= steps; ++i) {
    repsPair = processReps(repsPair, insertionRules);
    cout << endl << "step " << i << ":\t";
    print(repsPair);
  }
  return repsPair;
}

uint64_t resolve(const vector<uint64_t> &charsReps) {
  uint64_t minimum = ULLONG_MAX, maximum = 0; 
  for (uint64_t letterIndex = 0; letterIndex < 26; ++letterIndex) {
    uint64_t reps = charsReps[letterIndex];
    if (reps == 0) {
      continue;
    }
    if (reps > maximum) {
      maximum = reps;
    }
    if (reps < minimum) {
      minimum = reps;
    }
  }
  return maximum - minimum;
}

void solve(int part = 1) {
  int totalSteps = part == 1 ? 10 : 40;
  pair<string,unordered_map<string,char>> input = getInput();
  string aux = input.first;
  RepsPair repsPair = process(input.first, input.second, totalSteps);
  uint64_t result = resolve(repsPair.second);
  cout << "Part " << part << " (after " << totalSteps << "): " << result << endl;
}

};  // aoc2021_14

#endif /* _2021_ADVENTOFCODE_14_H_ */
