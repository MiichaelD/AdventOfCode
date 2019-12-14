/*
  Link:         http://adventofcode.com/2019/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/XX/2019
*/

#ifndef _2019_ADVENTOFCODE_12_H_
#define _2019_ADVENTOFCODE_12_H_

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

namespace aoc2019_12 {  
  using namespace std;

  class Moon {
    int x, y, z;
    Moon() : x(0), y(0), z(0) { }
    Moon(int x1, int y1, int z1) : x(x1), y(y1), z(z1) { }
  };

  Moon createMoonFromInput(const string &input) {
    Moon moon;
    int aux = 0;
    bool parsingNumber = false;
    for (int i = 0 ; i < input.size(); ++i) {
      if (isDigit(input[i])) {
        aux *= 10;
        aux += input[i] - '0';
      }
    } 
    return moon;
  }


  vector<Moon> createMoons() {
    string input;
    vector<Moon> moons;
    while (!cin.eof()) {
      cin >> input;
      moons.push_back(createMoonFromInput(input));
    }
    return moons;
  }

  void solve1() {   
    vector<Moon> moons = createMoons();
    
  }

  void solve2() {    
    
  }

  void solve(int part = 1) {
    using namespace std;
    if (part == 1) {
      solve1();
    } else {
      solve2();
    }
  }
};

#endif /* _2019_ADVENTOFCODE_12_H_ */