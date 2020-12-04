/*
  Link:         http://adventofcode.com/2020/day/4

  Description:
  Compiling:    g++ -std=c++11 main.cpp -o main

  Programmer:   Michael Duarte.

  Date:         12/04/2020
*/

#ifndef _2020_ADVENTOFCODE_04_H_
#define _2020_ADVENTOFCODE_04_H_

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../util/util.h"

namespace aoc2020_04 {
using namespace std;

static constexpr char kOptionalId[] = "cid:";

static const unordered_set<string> kIds = {
    "byr:", "iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:", "cid:"};

vector<string> getLines() {
  string line;
  vector<string> lines;
  stringstream ss;
  while (!cin.eof()) {
    getline(cin, line);
    ss << line;
    if (line.empty()) {
      lines.push_back(ss.str());
      // cout << "Passport: " << ss.str() << endl;
      ss.str("");
      ss.clear();
    } else {
      ss << ' ';
    }
  }
  lines.push_back(ss.str());
  // cout << "Passport: " << ss.str() << endl;
  return lines;
}

bool hasValidPassportIds(const string &pass) {
  // cout << "checking passport IDs: " << pass << endl;
  unordered_set<string> passIds = kIds;
  stringstream ss;
  string id;
  bool skipping = false;
  for (int i = 0; i < pass.size(); ++i) {
    if (skipping) {
      if (pass[i] == ' ') {
        skipping = false;
      }
      continue;
    }
    ss << pass[i];
    if (pass[i] == ':') {
      id = ss.str();
      if (passIds.find(id) != passIds.end()) {
        passIds.erase(id);
        // cout << "\tfound: " << id << ". Ids left: " << passIds.size() <<
        // endl;
      }
      ss.str("");
      ss.clear();
      skipping = true;
    }
  }
  if (passIds.find(kOptionalId) != passIds.end()) {
    passIds.erase(kOptionalId);
  }
  return passIds.empty();
}

inline bool isValueInRange(const string &value, int from, int to, int len = 4) {
  int val = atoi(value.c_str());
  return (len == -1 || value.size() == len) && val >= from && val <= to;
}

bool isValidIdValue(const string &key, const string &value) {
  // cout << "\t\tChecking: '" << key << "'\t'" << value << "'";
  if (key == "byr:") {
    return isValueInRange(value, 1920, 2002);
  }
  if (key == "iyr:") {
    return isValueInRange(value, 2010, 2020);
  }
  if (key == "eyr:") {
    return isValueInRange(value, 2020, 2030);
  }
  if (key == "hgt:") {
    if (value[value.size() - 2] == 'i') {
      return isValueInRange(value, 59, 76, -1);
    }
    return isValueInRange(value, 150, 193, -1);
  }
  if (key == "hcl:") {
    if (value[0] != '#' || value.size() != 7) {
      return false;
    }
    for (int i = 1; i < value.size(); ++i) {
      if (!isxdigit(value[i])) {
        return false;
      }
    }
    return true;
  }
  if (key == "ecl:") {
    static unordered_set<string> kValidEyeColors = {"amb", "blu", "brn", "gry",
                                                    "grn", "hzl", "oth"};
    return kValidEyeColors.find(value) != kValidEyeColors.end();
  }
  if (key == "pid:") {
    if (value.size() != 9) {
      return false;
    }
    for (int i = 0; i < value.size(); ++i) {
      if (!isdigit(value[i])) {
        return false;
      }
    }
    return true;
  }
  return key == "cid:";
}

bool isValidPassport(const string &pass) {
  // cout << "checking passport validity: " << pass << endl;
  unordered_set<string> passIds = kIds;
  stringstream ss;
  string id;
  bool skipping = false;
  size_t validIdCount = 0;
  for (int i = 0; i < pass.size(); ++i) {
    if (skipping) {
      if (pass[i] == ' ') {
        skipping = false;
        if (isValidIdValue(id, ss.str())) {
          // cout << "\tValid" << endl;
          ++validIdCount;
        } else {
          // cout << "\tINVALID - MOVING ON..." << endl;
          return false;
        }
        ss.str("");
        ss.clear();
      } else {
        ss << pass[i];
      }
      continue;
    }
    ss << pass[i];
    if (pass[i] == ':') {
      id = ss.str();
      if (passIds.find(id) != passIds.end()) {
        passIds.erase(id);
        // cout << "\tfound: " << id << ". Ids left: " << passIds.size() <<
        // endl;
      }
      ss.str("");
      ss.clear();
      skipping = true;
    }
  }
  if (isValidIdValue(id, ss.str())) {
    ++validIdCount;
  }
  if (passIds.find(kOptionalId) != passIds.end()) {
    passIds.erase(kOptionalId);
  }
  return passIds.empty() && validIdCount >= 7;
}

void solve(int part = 1) {
  vector<string> lines = getLines();
  size_t valids = 0;
  if (part == 1) {
    for (const string &p : lines) {
      if (hasValidPassportIds(p)) {
        ++valids;
      }
    }
  } else {
    for (const string &p : lines) {
      if (isValidPassport(p)) {
        ++valids;
      }
    }
  }
  cout << valids << endl;
}

};  // namespace aoc2020_04

#endif /* _2020_ADVENTOFCODE_04_H_ */
