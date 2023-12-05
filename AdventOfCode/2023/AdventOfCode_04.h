/*
  Link:         http://adventofcode.com/2023/day/04
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/04/2023
*/

#ifndef _2023_ADVENTOFCODE_04_H_
#define _2023_ADVENTOFCODE_04_H_

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

namespace aoc2023_04 {
using namespace std;

struct Card {
  int id = 0;
  unordered_set<int> winning;
  vector<int> matches;
  int64_t value = 0;
};

Card processCard() {
  string input;
  cin >> input >> input;
  Card c;
  c.id = atoi(input.c_str());

  while (true) {
    cin >> input;
    if (input == "|") break;
    c.winning.insert(atoi(input.c_str()));
  }
  std::getline(cin, input);
  for (int i = 0; i < input.size(); ++i) {
    while (!isdigit(input[i])) ++i;
    int aux = util::getNumberRef(input, i);
    if (c.winning.find(aux) != c.winning.end()) {
      c.matches.push_back(aux);
    }
  }
  if (c.matches.size()) {
    c.value = 1;
    c.value = c.value << (c.matches.size() - 1);
  }
  return c;
}

int64_t solve1() {
  int64_t aux = 0;
  while (!cin.eof()) {
    Card c = processCard();
    cout << "Card: " << c.id << " has a value: " << c.value << ". matches: "
        << c.matches.size() << endl;
    aux += c.value;
  }
  return aux;
}

int64_t getCopies(const vector<int>& copies) {
  int64_t cardsCount = 0;
  for (int i = 0; i < copies.size(); ++i) {
    cout << "Card " << (i + 1) << ": Has " << copies[i] << " copies." << endl;
    cardsCount += copies[i];
  }
  return cardsCount;
}

int64_t solve2(vector<int>& copies) {
  size_t originals = 0;
  while (!cin.eof()) {
    Card c = processCard();
    cout << "Card: " << c.id << " has a value: " << c.value << ". matches: "
        << c.matches.size() << endl;
    int cardCount = copies[originals] + 1;
    for (int i = 1 ; i <= c.matches.size(); ++i) {
      if (copies.size() <= (originals + i)) {
        copies.push_back((0));
      } 
      copies[originals + i] += (cardCount);
      cout << "\tAdding " << (cardCount) << " to " << (1 + originals + i)
            << ". New copy count: " <<  copies[originals + i] << endl;
    
    }
    ++originals;
  }
  cout << "Total original cards: " << originals << endl;
  auto copiesCount = getCopies(copies);
  cout << "Total copies: " << copiesCount << endl;
  return originals + copiesCount;
}

void solve(int part = 1) {
  int64_t ans = 0;
  if (part == 1) {
    ans = solve1();
  } else {
    // vector<int> copies{0}; - not sure why this doesn't work for part 2.
    vector<int> copies(200);
    ans = solve2(copies);
  }
  cout << "Cards value: "  << ans << endl;
}

};  // aoc2023_04

#endif /* _2023_ADVENTOFCODE_04_H_ */
