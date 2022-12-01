/*
  Link:         http://adventofcode.com/2021/day/16
  Compiling:    g++ -std=c++20 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/17/2020
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

inline void printSpaces(int depth) {
  for (int i = 0; i < depth; ++i) {
    cout << "\t";
  }
}

struct Packet {
  int version = 0;
  int type = 0;
  int totalLengthInBits = 0;
  int subPacketsCount = 0;
  vector<Packet> packets;
  vector<int> values;
  stringstream literal;
  uint64_t cachedValue = 0;

  uint64_t getTotalVersion() {
    uint64_t result = version;
    for (Packet &packet : packets) {
      result += packet.getTotalVersion();
    }
    return result;
  }

  uint64_t getValue() {
    if (cachedValue) {
      return cachedValue;
    }
    uint64_t result = 0;
    switch(type) {
      case 0:   // Sum value
        for (Packet &packet : packets) {
          result += packet.getValue();
        }
        cachedValue = result;
        return result;
      case 1:  // Product value
        if (packets.size()) {
          result = 1;
          for (Packet &packet : packets) {
            result *= packet.getValue();
          }
        }
        cachedValue = result;
        return result;
      case 2:
        if (packets.size()) {  // Minimum value
          result = INT_MAX;
          for (Packet &packet : packets) {
            int val = packet.getValue();
            if (val < result) {
              result = val;
            }
          }
        }
        cachedValue = result;
        return result;
      case 3:
        if (packets.size()) {  // Maximum value
          result = 0;
          for (Packet &packet : packets) {
            uint64_t val = packet.getValue();
            if (val > result) {
              result = val;
            }
          }
        }
        cachedValue = result;
        return result;
      case 4: {  // Literal
        string literalStr = literal.str(); 
        cachedValue = util::binToDec<uint64_t>(literalStr, 0, literalStr.size());
        return cachedValue;
      }
      case 5:  // Greater than value
        return (packets.size() > 1) && packets[0].getValue() > packets[1].getValue() ? 1 : 0;
      case 6:  // Less than value
        return (packets.size() > 1) && packets[0].getValue() < packets[1].getValue() ? 1 : 0;
      case 7:  // Equal value
        return (packets.size() > 1) && packets[0].getValue() == packets[1].getValue() ? 1 : 0;
    }
    return result;
  }

  void print(int depth = 0) {
    string literalStr = literal.str();
    printSpaces(depth); cout << (type == 4 ? "Literal " : "Operator ") << "Packet Contents: " << endl;
    printSpaces(depth); cout << "Version: " << version << ", Type: " << type << endl;
    printSpaces(depth); cout << "Total (Recursive) Version: " << getTotalVersion() << endl;
    if (type == 4) {
      printSpaces(depth);
      cout << "Literal (" << literalStr.size() << "): " << literalStr << " Decimal: " <<  getValue() << ". Values: [";
      for (int c : values) {
        cout << util::decToHex(c);
      }
      cout << "]" << endl;
    } else {
      printSpaces(depth);
      if (totalLengthInBits) {
        cout << "Total length in bits: " << totalLengthInBits << endl;
      } else {
        cout << "Total amount of subpackets: " << subPacketsCount << endl;
      }
      printSpaces(depth);
      cout << "Number of subpackets: " << packets.size() << endl;
      for (Packet &packet : packets) {
        cout << endl;
        packet.print(depth + 1);
      }
    }
  }
};

Packet binToPacket(const string &binary, int &start, int length) {
  Packet result;
  // Packet header
  result.version = util::binToHex(binary, start, 3);
  result.type = util::binToHex(binary, start + 3, 3);
  start += 6;
  
  // Packet contents:
  if (result.type == 4) {  // Literal packet
    while (true) {
      result.values.push_back(util::binToHex(binary, start + 1, 4));
      result.literal << binary.substr(start + 1, 4);
      if (binary[start] == '0') {
        start += 5;
        break;
      }
      start += 5;
    }
  } else { // Operator packet
    if (binary[start] == '0') {
      result.totalLengthInBits = util::binToDec<uint64_t>(binary, start + 1, 15);
      start += 16;
      int prevStart = start;
      while ((start - prevStart) < result.totalLengthInBits) {
        result.packets.push_back(binToPacket(binary, start, length));
      }
    } else {
      result.subPacketsCount = util::binToDec<uint64_t>(binary, start + 1, 11);
      start += 12;
      for (int i = 0; i < result.subPacketsCount; ++i) {
        result.packets.push_back(binToPacket(binary, start, length));
      }
    }
  }
  return result;
}

Packet binToPacket(const string &binary) {
  int start = 0;
  return binToPacket(binary, start, binary.size());
}

void solve(int part = 1) {
  string input;
  cin >> input;
  string result = util::hexToBinary(input);
  cout << "Input (" << input.size() << "): " << input << endl;
  cout << "Binary: (" << result.size() << "): " << result << endl << endl;
  Packet resultPacket = binToPacket(result);
  resultPacket.print();
  cout << endl << endl;
  cout << "Part 1: Total version: " << resultPacket.getTotalVersion() << endl;
  cout << "Part 2: Evaluated expresion: " << resultPacket.getValue() << endl;
}

};  // aoc2021_16

#endif /* _2021_ADVENTOFCODE_16_H_ */
