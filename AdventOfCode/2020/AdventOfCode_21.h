/*
  Link:         http://adventofcode.com/2020/day/21
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/21/2020
*/

#ifndef _2020_ADVENTOFCODE_21_H_
#define _2020_ADVENTOFCODE_21_H_

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

namespace aoc2020_21 {
using namespace std;

struct Matches {
  string aName;
  vector<pair<string,int>> iToReps;
  unordered_map<string,int> iToIndex;
};

struct Food {
  vector<string> ingredients, allergens;
  unordered_set<string> ingredientSet, allergenSet;

  void print() const {
    cout << "Food w/ {";
    for (const auto &i : ingredients) {
      cout << " " << i;
    }
    cout << "} contains: {";
    for (const auto &a : allergens) {
      cout << " " << a;
    }
    cout << '}' << endl;
  }

  static Food fromInput() {
    Food result;
    string str;
    while(true) {
      cin >> str;
      if (str[0] == '(') break;
      result.ingredientSet.insert(str);
      result.ingredients.emplace_back(std::move(str));
    }
    while(true) {
      cin >> str;
      if (str.back() == ')') {
        string toAdd = str.substr(0, str.size() - 1);
        result.allergenSet.insert(toAdd);
        result.allergens.emplace_back(move(toAdd));
        break;
      } 
      if (str.back() == ',') {
        string toAdd = str.substr(0, str.size() - 1);
        result.allergenSet.insert(toAdd);
        result.allergens.emplace_back(move(toAdd));
      } else {
        result.allergenSet.insert(str);
        result.allergens.emplace_back(move(str));
      }
    }
    return result;
  }
};

struct Data {
  vector<Food> foods;
  unordered_map<string,int> ingredients, allergens;

  static Data fromInput() {
    Data result;
    while (!cin.eof()) {
      result.foods.push_back(Food::fromInput());
      const auto &f = result.foods.back();
      for (const auto &a: f.allergens) {
        ++result.allergens[a];
      }
      for (const auto &i: f.ingredients) {
        ++result.ingredients[i];
      }
    }
    return result;
  }
};

void fillMatches(
  unordered_map<string,Matches> &matches, vector<Food> &foods) {
  for (const Food &f : foods) {
    // cout << "Processing: "; f.print();
    for (const auto &a : f.allergens) {
      // cout << "\tAllergen: " << a << endl;
      for (const auto &i : f.ingredients) { 
        // cout << "\t\tIngredient: " << i.name << endl;
        auto &allergen = matches[a];
        if (allergen.aName.empty()) {
          allergen.aName = a;
        }
        auto iInAllergen = allergen.iToIndex.find(i);
        if (iInAllergen == allergen.iToIndex.end()) {
          allergen.iToIndex[i] = allergen.iToReps.size();
          allergen.iToReps.emplace_back(i, 1);
        } else {
          ++allergen.iToReps[allergen.iToIndex[i]].second;
        }
      }
    }
  }
}

void printMatches(const unordered_map<string,Matches> &matches) {
  for (const auto &entry : matches) {
    cout <<  entry.first << " possibly matches:" << endl;
    const auto &match = entry.second;
    for (const auto &p : match.iToReps) {
      cout << "\t" << p.first << ": " << p.second << endl;
    }
  }
  cout << endl;
}

void printMatchesVector(const vector<Matches> &matches) {
  for (const auto &match : matches) {
    cout <<  match.aName << " possibly matches:" << endl;
    for (const auto &p : match.iToReps) {
      cout << "\t" << p.first << ": " << p.second << endl;
    }
  }
  cout << endl;
}

vector<Matches> matchesToSortedVector(unordered_map<string,Matches> &matches) {
  vector<Matches> result;
  for (auto &entry : matches) {
    auto &ingredients = entry.second.iToReps;
    sort(ingredients.begin(), ingredients.end(),
         [](const pair<string,int> &p1, const pair<string,int> &p2){
           return p1.second > p2.second;
         });
  }
  for (auto entry : matches) {
    result.push_back(entry.second);
  }
  sort(result.begin(), result.end(),
       [](const Matches &a, const Matches &b){
         return a.iToReps.size() > b.iToReps.size();
       });
  return result;
}

bool matchIngredientToAllergen(
    const vector<Matches> &matches,
    unordered_map<string,string> &inToAll,
    int index = 0) {

  if (inToAll.size() == matches.size()) {
    return true;
  }
  const auto &m1 = matches[index];
  for (int i = 0; i < m1.iToReps.size(); ++i) {
    const auto &ingredient = m1.iToReps[i].first;
    if (inToAll.find(ingredient) != inToAll.end()) {
      continue;
    }
    inToAll[ingredient] = m1.aName;
    if (matchIngredientToAllergen(matches, inToAll, index + 1)) {
      return true;
    }
    inToAll.erase(ingredient);
  }
  return false;
}

void solve1() {
  Data data = Data::fromInput();
  unordered_map<string,Matches> matches;
  fillMatches(matches, data.foods);
  vector<Matches> vMatches = matchesToSortedVector(matches);
  // printMatches(matches);
  printMatchesVector(vMatches);

  unordered_map<string,string> inToAll;
  matchIngredientToAllergen(vMatches, inToAll);

  size_t result = 0;
  for (const auto &inEntry : data.ingredients) {
    if (inToAll.find(inEntry.first) != inToAll.end()) {
      continue;
    }
    result += inEntry.second;
  }
  cout << "Result: " << result << endl;
}

void solve2() {
  string input;
  cin >> input;
}

void solve(int part = 1) {
  if (part == 1) {
    solve1();
  } else {
    solve2();
  }
}

};  // aoc2020_21

#endif /* _2020_ADVENTOFCODE_21_H_ */
