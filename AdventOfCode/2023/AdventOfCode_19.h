/*
  Link:         http://adventofcode.com/2023/day/19
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/19/2023
*/

#ifndef _2023_ADVENTOFCODE_19_H_
#define _2023_ADVENTOFCODE_19_H_

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

namespace aoc2023_19 {
using namespace std;



class Rating {
 public:
  int x,  m,  a,  s;
  Rating(const string& line) {
    int index = 3;
    x = util::getNumberRef(line, index);
    index += 3;
    m = util::getNumberRef(line, index);
    index += 3;
    a = util::getNumberRef(line, index);
    index += 3;
    s = util::getNumberRef(line, index);
    Print();
  }

  void Print() const {
    cout << "Rating -  x: " << x << ", m: " << m << ", a: " << a << ", s: " << s << endl;
  }
};

class Predicate {
 public:
  char metric = ' ';
  char comparator = ' ';
  int value = -1;
  string next_node;

  Predicate(const string& token, int startInd, int endInd) {
    int i = startInd;
    for (i = startInd; i < endInd; ++i) {
      if (token[i] == ':') {
        metric = token[startInd];
        comparator = token[startInd + 1];
        value = util::getNumber(token, startInd + 2);
        break;
      }
    }
    if (i++ < endInd) {
      next_node = token.substr(i, endInd - i);
    } else {
      next_node = token.substr(startInd, endInd - startInd);
    }
  }

  void Print() const {
    cout << "Predicate - ";
    if (comparator != ' ') {
      cout << "Metric: " << metric << ", Comparator: " << comparator;
      cout << ", Value: " << value <<  ", ";
    }
    cout <<"Next Node: " << next_node << endl;
  }
};

class WorkFlow {
 public:
  string name;
  vector<Predicate> predicates;
  WorkFlow(const string& token) {
    int index = 0;
    for ( ; index < token.size(); ++index) {
      if (token[index] == '{') {
        name = token.substr(0, index);
        break;
      }
    }
    cout << "Workflow: " << name << endl;
    for (int j = ++index; j < token.size(); ++j) {
      if (token[j] == ',' || token[j] == '}') {
        cout << "\t";
        predicates.emplace_back(token, index, j).Print();
        index = ++j;
      }
    }
  }

  void Print() const {
    cout << "Workload: " << name << " has: ";
    for (const auto& predicate : predicates) {
      cout << "\t"; predicate.Print();
    }
    cout << endl;
  }
};

struct PuzzleInput {
  vector<WorkFlow> workflows;
  vector<Rating> ratings;

  static PuzzleInput GetInput(int part) {
    PuzzleInput input;
    string line;
    while(!cin.eof()) {  // Workloads
      getline(cin, line);
      if (line.empty()) break;
      input.workflows.emplace_back(line);
    }
    while(!cin.eof()) {  // Ratings
      getline(cin, line);
      if (line.empty()) break;
      input.ratings.emplace_back(line);
    }

    return input;
  }

};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput(part);
}

};  // aoc2023_19

#endif /* _2023_ADVENTOFCODE_19_H_ */
