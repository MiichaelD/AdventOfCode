/*
  Link:         http://adventofcode.com/2021/day/8
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/07/2021
*/

#ifndef _2021_ADVENTOFCODE_08_H_
#define _2021_ADVENTOFCODE_08_H_

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
#include "../util/util.h"
#include "../util/robot_2020.h"

namespace aoc2021_08 {
using namespace std;

typedef pair<vector<string>,vector<string>> Entry;

// 7 = ACF and 1 = CF, (ACF - CF) = A;
char findA(const string &one, const string &seven) {
  for (char c : seven) {
    bool found = false;
    for (char d : one) {
      if (d == c) {
        found = true;
        break;
      }
    }
    if (!found) {
      cout << "\t" << c << " is Segment: a" << endl;
      return c;
    }
  }
  return 0;
}

// 1 = CF, it will be the only missing once we have decoded F.
char findC(const string &one, char f) {
  for (char c : one) {
    if (c != f) {
      cout << "\t" << c << " is Segment: c" << endl;
      return c;
    }
  }
  return 0;
}

// 4 = BCDF, it will be the only one missing once we have decoded BDF.
char findD(const string &four, const unordered_map<char,char> &decoder) {
  for (char c : four) {
    if (decoder.find(c) == decoder.end()) {
      cout << "\t" << c << " is Segment: g" << endl;
      return c;
    }
  }
  return 0;
}

char findG(const unordered_map<char,char> &decoder) {  // The only character not seen.
  int accumulator = 0;
  for (const auto &p : decoder) {
    accumulator |= 1 << (p.first - 'a');
  }
  for (int i = 0; i < 7; ++i) {
    if ((accumulator & (1 << i)) == 0) {
      char result = 'a' + i;
      cout << "\t" << result << " is Segment: d" << endl;
      return result;
    }
  }
  return 0;
}

int decode(const vector<string> message,  const unordered_map<char,char> &decoder) {
  int solution = 0;
  for (int i = 0; i < message.size(); ++i) {
    int digit = 0;
    const string &digitStr = message[i];
    switch(digitStr.size()) {
      case 2: digit = 1; break;
      case 3: digit = 7; break;
      case 4: digit = 4; break;
      case 7: digit = 8; break;
     default:
      // Decode:
      stringstream ss;
      for (char c : digitStr) {
        ss << decoder.at(c);
      }
      string decodedSegments = ss.str();
      sort(decodedSegments.begin(), decodedSegments.end());
      if (decodedSegments == "acdeg") {
        digit = 2;
      } else if (decodedSegments == "acdfg") {
        digit = 3;
      }else if (decodedSegments == "abdfg") {
        digit = 5;
      }else if (decodedSegments == "abdefg") {
        digit = 6;
      }else if (decodedSegments == "abcdfg") {
        digit = 9;
      }
    }

    digit *= pow(10, 3 - i);
    solution += digit;
  }
  return solution;
}

int decode(const Entry &entry) {
  int solution = 0;
  util::printVector(entry.first); cout << " | "; util::printVector(entry.second);
  cout << endl;

  unordered_map<char,char> decoder;
  vector<int> letterReps(7);
  vector<int> digitsIndex(10);

  for (int i = 0; i < entry.first.size(); ++i) {
    for (int j = 0; j < entry.first[i].size(); ++j) {
      ++letterReps[entry.first[i][j] - 'a'];
    }
    switch(entry.first[i].size()) {
      case 2: digitsIndex[1] = i;  break;// one:  cc ff 
      case 4: digitsIndex[4] = i; break; // four: bb cc dd ff
      case 3: digitsIndex[7] = i; break; // seven: aa cc ff
      case 7: digitsIndex[8] = i; break; // eight: aa bb cc dd ee ff gg
    }
  }
  decoder[findA(entry.first[digitsIndex[1]], entry.first[digitsIndex[7]])] = 'a';
  for (char i = 0 ; i < letterReps.size(); ++i) {
    switch(letterReps[i]) {
      case 4: decoder['a' + i] = 'e';
        cout << "\t" << (char)('a' + i) << " is Segment: e" << endl;; 
        break;
      case 6: decoder['a' + i] = 'b';
        cout << "\t" << (char)('a' + i) << " is Segment: b" << endl;
        break;
      case 9: decoder['a' + i] = 'f';
        cout << "\t" << (char)('a' + i) << " is Segment: f" << endl;

        decoder[findC(entry.first[digitsIndex[1]], 'a' + i)] = 'c';
        break;
      default:
        break;
    }
  }
  decoder[findD(entry.first[digitsIndex[4]], decoder)] = 'd';
  decoder[findG(decoder)] = 'g';

  cout << endl << "Decoder: ";
  for (const pair<char,char> &entry : decoder) {
    util::printPair(entry);
  }
  solution = decode(entry.second, decoder);
  cout << endl << "Decoded value: " << solution << endl;
  return solution;
}

void solve(int part = 1) {
  vector<Entry> input;
  string word;
  size_t uniqueSegmentNumbersReps = 0;
  size_t decodedValuesSum = 0;
  while(!cin.eof()) {
    input.push_back({vector<string>(10), vector<string>(4)});
    for (int i = 0 ; i < 10; ++i) {
      cin >> input.back().first[i];
      // sort(input.back().first[i].begin(), input.back().first[i].end());
    }
    cin >> word;
    for (int i = 0 ; i < 4; ++i) {
      cin >> input.back().second[i];
      switch (input.back().second[i].size()) {
        case 2: // one:  cc ff 
        case 4: // four: bb cc dd ff
        case 3: // seven: aa cc ff
        case 7: // eight: aa bb cc dd ee ff gg
          ++uniqueSegmentNumbersReps;
          // cout << "\t" << input.back().second << " Has " << input.back().second.size() << " digits." << endl;
        case 5: //  two:    aa cc dd ee gg
                //  three:  aa cc dd ff gg
                //  five:   aa bb dd ff gg
        case 6: //  six:    aa bb dd ee ff gg
                //  nine:   aa bb cc dd ff gg 
                //  zero:   aa bb cc ee ff gg
        default:
          break;
      }
      // sort(input.back().second[i].begin(), input.back().second[i].end());
    }
    decodedValuesSum += decode(input.back());
  }
  cout << "Part 1: Unique Segment Number Reps: " << uniqueSegmentNumbersReps << endl;
  cout << "Part 2: Decoded values sum:         " << decodedValuesSum << endl;
}

};  // aoc2021_08

#endif /* _2021_ADVENTOFCODE_08_H_ */
