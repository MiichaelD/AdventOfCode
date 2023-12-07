/*
  Link:         http://adventofcode.com/2023/day/07
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/07/2023
*/

#ifndef _2023_ADVENTOFCODE_07_H_
#define _2023_ADVENTOFCODE_07_H_

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

namespace aoc2023_07 {
using namespace std;

inline int value(char c, bool jokersLowValue = false) {
  switch(c) {
    case 'T': return 10;
    case 'J': return jokersLowValue ? 1 : 11;
    case 'Q': return 12;
    case 'K': return 13; 
    case 'A': return 14;
    default: return c - '0';
  }
  return 0;
}

inline char value(int i) {
  switch(i) {
    case 1: 
    case 11: return 'J';
    case 10: return 'T';
    case 12: return 'Q';
    case 13: return 'K'; 
    case 14: return 'A';
    default: return (i + '0');
  }
  return 0;
}

bool sortByCardValue(const char a, const char b) {
  return value(a, true) > value(b, true);
}

class PuzzleInput {
public:
  PuzzleInput() {
    repsToCards.reserve(6);
    for (int i = 0 ; i < 6; ++i){
      repsToCards.emplace_back();
    }
  }
  string hand;
  int64_t bid = 0;
  unordered_map<char, int> cardsToReps;
  vector<vector<char>> repsToCards;
  int maxRepsSingleCard = 0;

  int jokersCount() const {
    auto it = cardsToReps.find('J');
    return it != cardsToReps.end() ? it->second : 0;
  }

  void processHand(int part = 1) {
    cin >> hand >> bid;
    for (char c : hand) {
      maxRepsSingleCard = std::max(++cardsToReps[c], maxRepsSingleCard);
    }
    cout << "Hand: " << hand << ". ";
    if (part == 1){
      cout << endl;
      return;
    }
    if (!jokersCount()) {
      cout << "Has no Jokers" << endl;
      return;
    }
    // To find the most valuable card, lets have them in buckets of repetitions.
    // We will look for the highest value bucket (more reps) and find the most
    // valuable card in ther by sorting the bucket based on the card value.
    for (const auto& entry : cardsToReps) {
      repsToCards[entry.second].push_back(entry.first);
    }
    for (int i = 1; i < repsToCards.size(); ++i) {
      sort(repsToCards[i].begin(), repsToCards[i].end(), sortByCardValue);
    }
    // Assing Js to card with max cardsToReps or higher value  if not J.
    char mostValuableCard = cardWithMoreRepsAndValue();
    if (mostValuableCard == 'J') {
      cout << "Most valuable card is J, leaving as is" << endl;
      return;
    }
    cout << "Assigning " << jokersCount() << " Jokers to ";
    maxRepsSingleCard = (cardsToReps[mostValuableCard] += jokersCount());
    cardsToReps.erase('J');
    cout << mostValuableCard << ".  repeated: " << maxRepsSingleCard << endl;
  }

  char cardWithMoreRepsAndValue() {
    for (int i = repsToCards.size() - 1; i > 0; --i) {
      // cout << "\tTrying bucket value: " << i << endl;
      for (char c : repsToCards[i]) {
        // cout << "\t\tCard value: " << c << endl;
        if (c == 'J') continue;
        return c;
      }
    }
    return '-';  // This shouldn't happen
  }

  void print() const {
    cout << "Hand: " << hand << " Diff Cards: " << cardsToReps.size() << " maxReps: "
         << maxRepsSingleCard << " Bid: " << bid << endl;
  }
};

// The whole purpose of this class is to add data to the sorting function.
// Based on the parameter passed to the constructor, we define the behavior.
class SortPerPart{
  bool jokersLowValue = false;
public:
  int part = 1;
  SortPerPart(int part) : part(part) {
    jokersLowValue = part == 2;
  }
  bool sortByHand(const PuzzleInput& a, const PuzzleInput& b) const {
    if (a.maxRepsSingleCard > b.maxRepsSingleCard) { // more of the same kind.
        cout << "\t " << a.hand << " beats " << b.hand << " by same kind (" ;
        cout << a.maxRepsSingleCard << ")" << endl;
      return true;
    }
    if (a.maxRepsSingleCard < b.maxRepsSingleCard) { // more of the same kind.
        cout << "\t " << b.hand << " beats " << a.hand << " by same kind (" ;
        cout << b.maxRepsSingleCard << ")" << endl;
      return false;
    }
    if (a.cardsToReps.size() < b.cardsToReps.size()) {  // Less cards
        cout << "\t " << a.hand << " beats " << b.hand << " less diff cards (" ;
        cout << a.cardsToReps.size() << " vs " << b.cardsToReps.size() << ")" << endl;
      return true;
    }
    if (a.cardsToReps.size() > b.cardsToReps.size()) {  // Less cards
        cout << "\t " << b.hand << " beats " << a.hand << " less diff cards (" ;
        cout << b.cardsToReps.size() << " vs " << a.cardsToReps.size() << ")" << endl;
      return false;
    }
    for (int i = 0 ; i < a.hand.size(); ++ i) {
      if (value(a.hand[i], jokersLowValue) > value(b.hand[i], jokersLowValue)) {
        cout << "\t " << a.hand << " beats " << b.hand << " in pos: " << i << endl;
        return true;
      }
      if (value(a.hand[i], jokersLowValue) < value(b.hand[i], jokersLowValue)) {
        cout << "\t " << b.hand << " beats " << a.hand << " in pos: " << i << endl;
        return false;
      }
    }
    return false;
  }
};

void solve(int part = 1) {
  string aux;
  vector<PuzzleInput> input;
  while (!cin.eof()) {
    input.emplace_back().processHand(part);
  }
  SortPerPart sorter(part);  // Sorter to pass the correct Joker processing.
  // Bind to member function by its instance, placeholders for parameters.
  auto sorterFn = std::bind(&SortPerPart::sortByHand, sorter,
                            std::placeholders::_1, std::placeholders::_2);
  sort(input.begin(), input.end(), sorterFn);
  int multiplier = input.size();
  int64_t acum = 0;
  for (const auto& i : input) {
    i.print();
    acum += i.bid * multiplier;
    --multiplier;
  }
  cout << "Solution = " << acum << endl;
}

};  // aoc2023_07

#endif /* _2023_ADVENTOFCODE_07_H_ */
