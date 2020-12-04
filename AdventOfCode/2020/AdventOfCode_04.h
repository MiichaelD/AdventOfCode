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
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
#include "../util/util.h"

namespace aoc2020_04 {
  using namespace std;

  string optionalId = "cid:";

  unordered_set<string> ids = 
      {"byr:","iyr:","eyr:","hgt:","hcl:","ecl:","pid:","cid:"};

  vector<string> getLines() {
    string line;
    vector<string> passports;
    stringstream ss;
    while(!cin.eof()) {
      getline(cin, line);
      ss << line;
      if (line.empty()) {
        passports.push_back(ss.str());
        // cout << "Passport: " << ss.str() << endl;
        ss.str("");
        ss.clear();
      } else {
        ss << ' ';
      }
    }
    passports.push_back(ss.str());
    // cout << "Passport: " << ss.str() << endl;
    return passports;
  }

  bool hasValidPassportIds(const string &pass) {
    // cout << "checking passport ids: " << pass << endl;
    unordered_set<string> passIds = ids;
    stringstream ss;
    bool skipping = false;
    string id;
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
          // cout << "\tfound: " << id << ". Ids left: " << passIds.size() << endl;
        }
        ss.str("");
        ss.clear();
        skipping = true;
      }
    }
    if (passIds.find(optionalId) != passIds.end()) {
      passIds.erase(optionalId);
    }
    return passIds.empty();
  }

  unsigned long long solve1() {
    vector<string> lines = getLines();
    size_t valids = 0;
    for (const string &p : lines) {
      if (hasValidPassportIds(p)) {
        ++valids;
      }
    }
    return valids;
  }

  bool isValidIdValue(const string &key, const string &value) {
    // cout << "\t\tChecking: '" << key << "'\t'" << value << "'";
    if (key == "byr:") {
      int val = atoi(value.c_str());
      return value.size() == 4 && val >= 1920 && val <= 2002;
    }
    if (key == "iyr:") {
      int val = atoi(value.c_str());
      return value.size() == 4 && val >= 2010 && val <= 2020;
    }
    if (key == "eyr:") {
      int val = atoi(value.c_str());
      return value.size() == 4 && val >= 2020 && val <= 2030;
    }
    if (key == "hgt:") {
      int val = atoi(value.c_str());
      if (value[value.size() - 2] == 'i') {
        return  val >= 59 && val <= 76;
      }
     return  val >= 150 && val <= 193;
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
      static unordered_set<string> validEyeColors =
         {"amb","blu","brn","gry","grn","hzl","oth"};
      return validEyeColors.find(value) != validEyeColors.end();
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
    unordered_set<string> passIds = ids;
    stringstream ss;
    bool skipping = false;
    string id;
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
          // cout << "\tfound: " << id << ". Ids left: " << passIds.size() << endl;
        } else {
          return false;
        }
        ss.str("");
        ss.clear();
        skipping = true;
      }
    }
    if (skipping) {
      skipping = false;
      if (isValidIdValue(id, ss.str())) ++validIdCount;
      ss.str("");
      ss.clear();
    }
    if (passIds.find(optionalId) != passIds.end()) {
      passIds.erase(optionalId);
    }
    return passIds.empty() && validIdCount >= 7;
  }

  unsigned long long solve2() {
    vector<string> lines = getLines();
    size_t valids = 0;
    for (const string &p : lines) {
      if (isValidPassport(p)) {
        ++valids;
      }
    }
    return valids;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2020_ADVENTOFCODE_04_H_ */





/*
  pair<bool,bool> isValidPassport(const string &pass) {
    cout << "checking passport validity: " << pass << endl;
    unordered_set<string> passIds = ids;
    stringstream ss;
    bool skipping = false;
    string id;
    size_t validIdCount = 0;
    for (int i = 0; i < pass.size(); ++i) {
      if (skipping) {
        if (pass[i] == ' ') {
          skipping = false;
          if (!isValidIdValue(id, ss.str())) {
            return false;
          } else { 
            ++validIdCount;
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
          cout << "\tfound: " << id << ". Ids left: " << passIds.size() << endl;
        } else {
          return false;
        }
        ss.str("");
        ss.clear();
        skipping = true;
      }
    }
    if (skipping) {
      skipping = false;
      if (isValidIdValue(id, ss.str())) ++validIdCount;
      ss.str("");
      ss.clear();
    }
    return make_pair(
        passIds.empty() || (passIds.size() == 1 && passIds.find("cin:") != passIds.end()),
        validIdCount >= 7);
  }

*/