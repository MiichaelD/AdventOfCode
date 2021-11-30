/*
  Link:         http://adventofcode.com/2020/day/16
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/16/2020
*/

#ifndef _2021_ADVENTOFCODE_16_H_
#define _2021_ADVENTOFCODE_16_H_

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

namespace aoc2021_16 {
using namespace std;

struct Ticket {
  vector<size_t> values;
  bool discarded = false;
};

struct Range {
  string name;
  vector<pair<int,int>> rules;
  vector<size_t> possibleFieldIndexes;
  int fieldIndex = -1;

  bool isInRange(int val) const {
    for (const auto &p : rules) {
      if (p.first <= val && val <= p.second) {
        return true;
      }
    }
    return false;
  }

  void print() const {
    cout << "\tRange '" << name << "':" << endl;
    for (const auto &r : rules) {
      cout << "\t\t"; util::printPair(r, true);
    }
    if (fieldIndex != -1)
      cout << "\t\tField Index Match: " << fieldIndex << endl;
    cout << "\t\tPossible ticket indexes:";
    for (size_t index : possibleFieldIndexes) {
      cout << " " << index;
    }
    cout << endl << endl;
  }

  int getTicketValue(const Ticket &ticket) const {
    return fieldIndex == -1 ? -1 : ticket.values[fieldIndex];
  }
};

struct Data{
  vector<Range> ranges;
  vector<Ticket> tickets;

  void print() const {
    cout << "Ranges: " << ranges.size() << endl;
    for (const auto &r : ranges) {
      r.print();
    }
    cout << "Tickets: " << tickets.size() << endl;
    // for (const auto &ticket : tickets) {
    //   cout << "\t";
    //   for (size_t t : ticket) {
    //     cout << ", " << t;
    //   }
    //   cout << endl;
    // }
  }
};

pair<int,int> getRangeRule(const string &line, size_t &index) {
  pair<int,int> result{-1, -1};
  int accum = 0;
  for (; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == '-') {
      result.first = accum;
      accum = 0;
    } else if (line[index] == ' ') {
      result.second = accum;
      break;
    }
  }
  if (result.second == -1) {
    result.second = accum;
  }
  return result;
}

Ticket getTicketData() {
  Ticket input;
  string line;
  cin >> line;
  uint64_t accum = 0;
  size_t bInd = 0;
  for (size_t index = 0; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else if (line[index] == ',') {
      input.values.push_back(accum);
      accum = 0;
    }
  }
  input.values.push_back(accum);
  accum = 0;
  return input;
}

Data getInput() {
  Data result;
  string str, line;
  vector<string> lines;
  stringstream ss;
  do {
    getline(cin,line);
    if (line.empty()) break;
    result.ranges.push_back({});
    size_t index = 0;
    // Getting Range name
    for (; index < line.size(); ++index) {
      if (line[index] == ':') {
        result.ranges.back().name = line.substr(0, index);
        break;
      }
    }
    index += 2;
    result.ranges.back().rules.push_back(getRangeRule(line, index));
    index += 4;
    result.ranges.back().rules.push_back(getRangeRule(line, index));
  } while(!line.empty());

  getline(cin,line);
  result.tickets.emplace_back(getTicketData());
  getline(cin,line);
  getline(cin,line);
  getline(cin,line);
  while(!cin.eof()) {
    result.tickets.emplace_back(getTicketData());
  }
  return result;
}

uint64_t discardInvalidTickets(Data &input) {
  uint64_t result = 0;
  for (auto &ticket : input.tickets) {
    for (auto &t : ticket.values) {
      bool isValid = false;
      for (const auto &r: input.ranges) {
        if (r.isInRange(t)) {
          isValid = true;
          break;
        }
      }
      if (!isValid) {
        // cout << "Ticket value " << t << " not in any valid range" << endl;
        ticket.discarded = true;
        result += t;
      }
    }
  }
  cout << "Ticket scanning error rate: " << result << endl;
  return result;
}

void findPossibleMatches(Data &input) {
  for (auto &r: input.ranges) {
    for (int index = 0; index < input.tickets[0].values.size(); ++index) {
      bool valid = true;
      for (auto &ticket : input.tickets) {
        if (ticket.discarded) continue;
        if (!r.isInRange(ticket.values[index])) {
          valid = false;
          break;
        }
      }
      if (valid) {
        r.possibleFieldIndexes.push_back(index);
      }
    }
  }
}

bool matchTicketFields(vector<Range> &ranges, size_t &map, size_t index=0) {
  if (index >= ranges.size()) {
    return true;
  }
  Range &r = ranges[index];
  for (size_t j = 0 ; j < r.possibleFieldIndexes.size(); ++j) {
    size_t trialIndex = r.possibleFieldIndexes[j];
    int shiftedBit = (1l << trialIndex);
    for (int i = 0; i < index; ++i) { cout << "  ";}
    cout << r.name << " [" << index << "] w/ field #" << trialIndex;
    if (map & shiftedBit) {
      cout << ". VISITED" << endl;
      continue; // Already visited
    }
    map |= (shiftedBit);
    r.fieldIndex = trialIndex;
    cout << ". (Remaining:" << (r.possibleFieldIndexes.size() - j - 1) << ')'<< endl;
    if (matchTicketFields(ranges, map, index + 1)) {
      return true; // We found a Range - Field match
    }
    // This didn't work, try next field index
    map &= ~(shiftedBit);
    r.fieldIndex = -1;
  }
  return false;
}

void solve(int part = 1) {
  Data input = getInput();
  discardInvalidTickets(input);
  if (part != 1) {
    const string fieldPrefix = "departure";
    findPossibleMatches(input);
    sort(input.ranges.begin(), input.ranges.end(),
         [](const Range &a, const Range &b){
           return a.possibleFieldIndexes.size() < b.possibleFieldIndexes.size();
         });
    size_t map = 0;
    matchTicketFields(input.ranges, map);
    size_t result = 1;
    for (const auto &r: input.ranges) {
      // r.print();
      int ticketValue = r.getTicketValue(input.tickets.front());
      if (ticketValue > 0) {
        cout << "Ticket's " << r.name << ": " << ticketValue << endl;
        if (!r.name.compare(0, fieldPrefix.size(), fieldPrefix)) {
          result *= ticketValue;
        }
      }
    }
    cout << "Multiplication of ticket's departure field values: " << result << endl; 
  }
}

};  // aoc2021_16

#endif /* _2021_ADVENTOFCODE_16_H_ */
