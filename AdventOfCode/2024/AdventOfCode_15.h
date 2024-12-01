/*
  Link:         http://adventofcode.com/2023/day/15
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/15/2023
*/

#ifndef _2023_ADVENTOFCODE_15_H_
#define _2023_ADVENTOFCODE_15_H_

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

namespace aoc2023_15 {
using namespace std;

constexpr int kTotalLenses = 9;
constexpr int kTotalBoxes = 256;

int GetTokenValue(const string& token) {
  int token_value = 0;
  for (char c : token) {
    token_value += c;
    token_value *= 17;
    token_value %= 256;
    // cout << "\tchar: " << c << ", value: " << token_value << endl; 
  }
  return token_value;
}

tuple<string,char,int> TokenToCommand(const string& token) {
  tuple<string,char,int> command;
  for (int i = 0; i < token.size(); ++i) {
    if (token[i] == '-' || token[i] == '=') {
      std::get<0>(command) = token.substr(0, i);
      std::get<1>(command) = token[i];
      if (token[i] == '=') {
        std::get<2>(command) = util::getNumber(token, i + 1);
      }
    }
  }
  // cout << "Command: " << std::get<0>(command) << ", " << std::get<1>(command)
  //      << ", " << std::get<2>(command) << endl;
  return command;
}

struct PuzzleInput {
  string line;
  vector<string> tokens;
  vector<tuple<string,char,int>> commands;
  vector<deque<pair<string,int>>> box_to_label_lens;

  static PuzzleInput GetInput() {
    PuzzleInput input;
    input.box_to_label_lens.reserve(kTotalBoxes + 1);
    for (int i = 0 ; i <= kTotalBoxes; ++i) {
      input.box_to_label_lens.emplace_back();
    }
    getline(cin, input.line);
    stringstream buffer;
    for (char c : input.line) {
      if (c ==',') {
        input.tokens.push_back(buffer.str());
        // cout << "Token added: " << input.tokens.back() << endl;
        buffer.str("");
        input.commands.emplace_back(TokenToCommand(input.tokens.back()));
        continue;
      }
      buffer << c;
    }
    input.tokens.emplace_back(buffer.str());
    input.commands.emplace_back(TokenToCommand(input.tokens.back()));
    // cout << "Token added: " << input.tokens.back() << endl;
    return input;
  }

  int64_t GetValue() const {
    int64_t value = 0;
    for (const string& token : tokens) {
      int token_value = GetTokenValue(token);
      // cout << "Token Value: " << token_value << endl;
      value += token_value;
    }
    return value;
  }

  void Process() {
    for (const auto&  command : commands) {
      // cout << "Command: " << std::get<0>(command) << ", " << std::get<1>(command)
      //     << ", " << std::get<2>(command) << endl;
      const auto& label = std::get<0>(command);
      int label_hash = GetTokenValue(label);
      auto& box_contents = box_to_label_lens[label_hash];
      switch(std::get<1>(command)) {
        case '-': {
          for (auto it = box_contents.begin(); it != box_contents.end(); ++it) {
            if (it->first == label) {
              box_contents.erase(it);
              break;
            }
          }
          break;
        }
        case '=': {
          int lens = std::get<2>(command);
          bool found = false;
          for (auto it = box_contents.begin(); it != box_contents.end(); ++it) {
            if (it->first == label) {
              it->second = lens;
              found = true;
              break;
            }
          }
          if (!found) {
            box_contents.push_back({label, lens});
          }
          break;
        }
      }
      
    }
  }

  int64_t GetFocusingPower() {
    int64_t result = 0;
    for (int b = 0; b < box_to_label_lens.size(); ++b) {
      for (int l = 0; l < box_to_label_lens[b].size(); ++l) {
        int value = (1 + b) * (1 + l) * box_to_label_lens[b][l].second;
        // cout << "Label: " << box_to_label_lens[b][l].first << " has value: " << value << endl;
        result += value;
      }
    }
    return result;
  }
};


void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput();
  if (part == 1) {
    cout << "Result: " << input.GetValue() << endl; 
  } else {
    input.Process();
    auto result = input.GetFocusingPower();
    cout << "Result: " << result << endl; 
  }

}

};  // aoc2023_15

#endif /* _2023_ADVENTOFCODE_15_H_ */
