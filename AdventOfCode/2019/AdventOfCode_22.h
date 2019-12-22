/*
  Link:         http://adventofcode.com/2019/day/22
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/22/2019
*/

#ifndef _2019_ADVENTOFCODE_22_H_
#define _2019_ADVENTOFCODE_22_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc2019_22 {  
  using namespace std;

  const size_t TOTAL_CARDS_PT1 = 10007;
  const size_t TOTAL_CARDS_PT2 = 119315717514047;
  const size_t TOTAL_SHUFFLES = 101741582076661;
  
  const int CUT = 0;
  const int STA = 1;
  const int INC = 2;

  template<class T>
  void printCards(const deque<T> &vec) {
    for (const T &ele : vec) {
      cout << ele <<", ";
    }
    cout << endl;
  }
  
  int parseNumber(const string &input, size_t startInd) {
    int aux = 0;
    bool negative = false;
    for (size_t i = startInd ; i < input.length(); ++i) {
      if (input[i] == '-') {
        negative = true;
      } else {
        aux *= 10;
        aux += (input[i] - '0');
      }
    }
    return (negative ? -aux : aux);
  }

  string getOperationName(const int &op) {
    switch(op) {
      case CUT: return "cut";
      case STA: return "sta";
      case INC:
      default: return "inc";
    }
  }

  pair<int,int> parseInputString(const string &input) {
    pair<int,int> result;
    // cout << "Parsing: " << input << " => ";
    if (input[0] == 'c' && input[1] == 'u' && input[2] == 't') {  // cut
      result = make_pair(CUT, parseNumber(input, 4));
    } else if (input[0] == 'd' && input[5] == 'i' && input[10] == 'n') { // new stack
      result = make_pair(STA, 0);
    } else if (input[0] == 'd' && input[5] == 'w' && input[10] == 'i') { // increment
      result = make_pair(INC, parseNumber(input, 20));
    }
    // cout << getOperationName(result.first) << " - " << result.second << endl;c
    return result;
  }

  vector<pair<int,int>> parseInput() {
    vector<pair<int,int>> result;
    string input;
    while (!cin.eof()) {
      getline(cin, input);
      result.push_back(parseInputString(input));
    }
    return result;
  }

  void cutCards(deque<int> &cards, int n) {
    if (n > 0) {
      for (int i = 0; i < n; ++i) {
        cards.push_back(cards.front());
        cards.pop_front();
      }
    } else {
      int absN = abs(n);
      for (int i = 0; i < absN; ++i) {
        cards.push_front(cards.back());
        cards.pop_back();
      }
    }
  }

  deque<int> incCards(deque<int> &cards, int n) {
    deque<int> result(cards.size(), 0);
    int aux = 0;
    for (int i = 0; i < cards.size(); ++i) {
      result[aux] = cards[i];
      aux = (aux + n) % cards.size();
    }
    return result;
  }

  inline void staCards(deque<int> &cards) {
    std::reverse(cards.begin(),cards.end());
  }

  void processInput(deque<int> &cards, const vector<pair<int,int>> &input) {
    for (const pair<int,int> &in : input) {
      switch (in.first) {
        case CUT: cutCards(cards, in.second); break;
        case STA: staCards(cards); break;
        case INC: cards = incCards(cards, in.second); break;
      }
    }
  }

  size_t findCardIndex(const deque<int> &cards, int targetCard) {
    for (size_t i = 0; i < cards.size(); ++i) {
      if (cards[i] == targetCard) {
       cout << "Card #" << targetCard << ": " << i << endl;
        return i;
      }
    }
    return -1;
  }


  inline deque<int> fillDeck(size_t cardAmount) {
    deque<int> cards(cardAmount, 0);
    for (int i = 0; i < cardAmount; ++i) {
      cards[i] = i;
    }
    return cards;
  }

  void solve(int part = 1) {
    vector<pair<int,int>> input = parseInput();
    if (part == 1) {
      deque<int> cards = fillDeck(TOTAL_CARDS_PT1);
      processInput(cards,input);
      findCardIndex(cards, 2019);
    } else {
      deque<int> cards = fillDeck(TOTAL_CARDS_PT2);
      for (size_t i = 0 ; i < TOTAL_SHUFFLES; ++i) {
        processInput(cards,input);
      }
      findCardIndex(cards, 2020);
    }
    // printCards(cards);
  }
};

#endif /* _2019_ADVENTOFCODE_22_H_ */