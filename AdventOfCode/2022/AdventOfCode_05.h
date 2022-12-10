/*
  Link:         http://adventofcode.com/2022/day/5
  Compiling:    g++ -std=c++17 main.cpp -o main && cat 2022/AdventOfCode_05_input.txt | ./main
  Programmer:   Michael Duarte.
  Date:         12/10/2022
*/

#ifndef _2022_ADVENTOFCODE_05_H_
#define _2022_ADVENTOFCODE_05_H_

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
#include <utility>
#include "../util/util.h"

namespace aoc2022_05 {  
using namespace std;

typedef vector<deque<char>> Stacks;

std::optional<char> getCrate(const string& input, int& index) {
  if (index >= input.size() || input[index] == ' ') {
    index += 4;
    return std::nullopt;
  }
  char result = input[index+1];
  index +=4;
  return result;
}

void printStacks(const Stacks& stacks, bool final = false) {
  for (int s = 0; s < stacks.size(); ++s) {
    cout << "Stack [" << s << "] size: " << stacks[s].size() << ". Top crate: " << stacks[s].back() << endl;
  }
  if (final) {
    cout << "Top cranes per stack: ";
    for (int s = 0; s < stacks.size(); ++s) {
      cout << stacks[s].back();
    }
    cout << endl;
  }
}

void solve(int part = 1) {
  string input;
  Stacks stacks;
  // Get Stacks state.
  while (getline(cin, input)) {
    if (input.empty()) {
      cout << "Finishing processing" << endl;
      break;
    }
    if (stacks.empty()) {
      int total_stacks = (input.size() + 1) / 4;
      cout << "Processing: " << total_stacks << " stacks" << endl;
      for (int s = 0; s < total_stacks; ++s) {
        stacks.emplace_back(deque<char>());
      }
    }
    for (int i = 0, s = 0 ; i < input.length(); ++s) {
      std::optional<char> crate = getCrate(input, i);
      if (!crate.has_value()) {
        continue;
      }
      stacks[s].push_front(crate.value());
    }
  }
  printStacks(stacks);
  // Process instructions
  while (getline(cin, input)) {
    int index = 5;
    int cranes_to_move = util::getNumberRef(input, index);
    index += 6;
    int from = util::getNumberRef(input, index) - 1;
    index += 4;
    int to = util::getNumberRef(input, index) - 1;
    cout << "Moving " << cranes_to_move << " cranes from " << from + 1 << " to " << to + 1 << endl;

    deque<char> temp_queue;
    for (int c = 0; c < cranes_to_move; ++c) { 
      if (part == 1) {
        stacks[to].push_back(stacks[from].back());
        stacks[from].pop_back();
      } else {
        temp_queue.push_back(stacks[from].back());
        stacks[from].pop_back();
      }
    }
    if (part != 1) {
      while(!temp_queue.empty()) {
        stacks[to].push_back(temp_queue.back());
        temp_queue.pop_back();
      }
    }
    printStacks(stacks);
    cout << endl << endl;
  }
  printStacks(stacks, true);
}

};

#endif /* _2022_ADVENTOFCODE_05_H_ */