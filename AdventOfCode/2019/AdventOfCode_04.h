/*
  Link:         http://adventofcode.com/2019/day/4
 

  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/04/2019
*/

#ifndef _2019_ADVENTOFCODE_04_H_
#define _2019_ADVENTOFCODE_04_H_

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

namespace aoc2019_04 {  
  using namespace std;

  const char DELIMITER = '-';
  
  int n1, n2;
  string in1, in2;

  void init(const string &input) {
    int aux;
    stringstream ss;
    for (int i = 0 ; i < input.length(); ++i) {
      if (input[i] == DELIMITER) {
        in1 = ss.str();
        ss.str(string());
        n1 = aux;
        aux = 0;
      } else {
        aux *= 10;
        aux += input[i] - '0';
        ss << input[i];
      }
    }
    in2 = ss.str();
    n2 = aux;
    cout << n1 << " (" <<in1<< "), "<<n2<<" ("<<in2<<") "<< endl;
  }

  vector<string> getPasswordCandidates() {
    vector<string> result;
    stringstream ss;

    for (int i = max(n1, 100000); i <= n2; ++i) {
      ss << i;
      string aux = ss.str();
      ss.str(string());
      if (aux.length() != 6) {
        continue;
      }

      bool valid = false;
      for (int j = 0; j < aux.length() - 1; ++j) {
        if (aux[j] > aux[j + 1]) {
          valid = false;
          break;
        } else if (aux[j] == aux[j + 1]) {
          valid = true;
        }
      }
      if (valid) {
        // cout << "Adding: " << aux << endl;
        result.push_back(aux);
      }
    }

    return result;
  }

  void solve1() {   
    // string input = "178416-676461";
    string input = "387638-919123";
    // cin >> input;
    init(input);
    
    vector<string> passwords = getPasswordCandidates();
    cout << passwords.size() << endl;
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

#endif /* _2019_ADVENTOFCODE_04_H_ */
