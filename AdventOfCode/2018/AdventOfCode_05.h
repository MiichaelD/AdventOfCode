/*
  Link:         http://adventofcode.com/2018/day/3
 
  Description:  --- Day 5: Alchemical Reduction ---
    You've managed to sneak in to the prototype suit manufacturing lab. The Elves are making decent progress, but are still struggling with the suit's size reduction capabilities.

    While the very latest in 1518 alchemical technology might have solved their problem eventually, you can do better. You scan the chemical composition of the suit's material and discover that it is formed by extremely long polymers (one of which is available as your puzzle input).

    The polymer is formed by smaller units which, when triggered, react with each other such that two adjacent units of the same type and opposite polarity are destroyed. Units' types are represented by letters; units' polarity is represented by capitalization. For instance, r and R are units with the same type but opposite polarity, whereas r and s are entirely different types and do not react.

    For example:

    In aA, a and A react, leaving nothing behind.
    In abBA, bB destroys itself, leaving aA. As above, this then destroys itself, leaving nothing.
    In abAB, no two adjacent units are of the same type, and so nothing happens.
    In aabAAB, even though aa and AA are of the same type, their polarities match, and so nothing happens.
    Now, consider a larger example, dabAcCaCBAcCcaDA:

    dabAcCaCBAcCcaDA  The first 'cC' is removed.
    dabAaCBAcCcaDA    This creates 'Aa', which is removed.
    dabCBAcCcaDA      Either 'cC' or 'Cc' are removed (the result is the same).
    dabCBAcaDA        No further actions can be taken.
    After all possible reactions, the resulting polymer contains 10 units.

    How many units remain after fully reacting the polymer you scanned? (Note: in this puzzle and others, the input is large; if you copy/paste your input, make sure you get the whole thing.)
  
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/27/2018
*/

#ifndef _2018_ADVENTOFCODE_05_
#define _2018_ADVENTOFCODE_05_

#include <iostream>
#include <cstdio>
#include <string>
#include <deque>
#include <cmath>
#include <cctype>
#include <climits>
#include <unordered_map>
#include <vector>

namespace aoc2018_05 {

  int reactionDiffValue;

  bool reacts(char a, char b) {
    return abs(a - b) == reactionDiffValue;
  }

  bool ignoreChar(char c, char ignoredChar) {
    int diff = abs(c - ignoredChar);
    return diff == 0 || diff == reactionDiffValue;
  }

  std::deque<char> getReducedInput(const std::string &input, char ignoredChar = ' ') {
    std::deque<char> reducedStack;
    for (int i = 0; i < input.size(); ++i) {
      if (ignoreChar(input[i], ignoredChar)) {
        continue;
      }
      if (reducedStack.size() > 0 && reacts(reducedStack.back(), input[i])) {
        reducedStack.pop_back();
      } else {
        reducedStack.push_back(input[i]);
      }
    }
    return reducedStack;
  }

  void printStack(const std::deque<char> &reducedStack) {
    using namespace std;
    for (auto it = reducedStack.begin(); it != reducedStack.end(); ++it) {
      cout << *it;
    }
    cout << endl;
  }

  void solve1(const std::string &input) {
    printStack(getReducedInput(input));
  }

  void solve2(const std::string &input) {
    using namespace std;
    unordered_map<char,int> reducedMap;

    for (int i = 0; i < input.size(); ++i) {
      reducedMap.insert(make_pair<char,int>(tolower(input[i]), 0));
    }

    int shortestLength = INT_MAX;
    deque<char> smallestReduced;
    char smallestReducedIgnoredChar;
    for (auto it = reducedMap.begin(); it != reducedMap.end(); ++it) {
      deque<char> reducedStack = getReducedInput(input, it->first);
      it->second = reducedStack.size();
      if (it->second < shortestLength) {
        smallestReduced = reducedStack;
        smallestReducedIgnoredChar = it->first;
        shortestLength = it->second;
      }
      cout << "W/o char: " << it->first << " => " << reducedStack.size() << endl; //": ";
      // printStack(reducedStack);

    }
    cout << endl << "Smallest reduced (" << smallestReduced.size() << ") w/o letter: ";
    cout << smallestReducedIgnoredChar << endl;
    printStack(smallestReduced);
  }
  
  void solve(int part = 1) {
    using namespace std;

    reactionDiffValue = 'a' - 'A';
    string input;
    cin >> input;
    if (part == 1) solve1(input); else solve2(input);
  }
};

#endif /* _2018_ADVENTOFCODE_05_ */
