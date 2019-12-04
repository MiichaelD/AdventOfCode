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

  void init(const string &input) {
    int aux;
    stringstream ss;
    for (int i = 0 ; i < input.length(); ++i) {
      if (input[i] == DELIMITER) {
        n1 = aux;
        aux = 0;
      } else {
        aux *= 10;
        aux += input[i] - '0';
      }
    }
    n2 = aux;
    cout << n1 << " - " << n2 << endl;
  }

  bool validate1(const string &term) {
    bool valid = false;
    for (int j = 0; j < term.length() - 1; ++j) {
      if (term[j] > term[j + 1]) {
        valid = false;
        break;
      } else if (term[j] == term[j + 1]) {
        valid = true;
      }
    }
    return valid;
  }

  bool validate2(const string &term) {
    bool valid = false;
    for (int j = 0; j < term.length() - 1; ++j) {
      if (term[j] > term[j + 1]) {
        valid = false;
        break;
      } else if (term[j] == term[j + 1]) {
        if ((j + 2) < term.length() && term[j] == term[j + 2]) {
          // Group is bigger than 2
          // cout << "\t" << term << " j " << j;
          int x = j;
          for (; j < term.length(); ++j) {
            if (term[x] != term[j]) {
              j -= 2;
              break;
            }
          }
          // cout  << " -> " << j+1 << endl;
        } else {
          valid = true;
        }
      }
    }
    return valid;
  }

  vector<string> getPasswordCandidates(int part = 1) {
    vector<string> result;
    stringstream ss;

    for (int i = max(n1, 100000); i <= n2; ++i) {
      ss << i;
      string aux = ss.str();
      ss.str(string());
      if (aux.length() != 6) {
        continue;
      }

      if (part == 1 ? validate1(aux) : validate2(aux)) {
        // cout << "Adding: " << aux << endl;
        result.push_back(aux);
      }
    }
    return result;
  }


  void solve(int part = 1) {
    using namespace std;

    string input = "178416-676461";
    // string input = "387638-419123";
    // cin >> input;
    init(input);
    
    vector<string> passwords = getPasswordCandidates(part);
    cout << passwords.size() << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_04_H_ */
