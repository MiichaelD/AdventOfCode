/*
  Link:         http://adventofcode.com/2019/day/6
 

  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/06/2019
*/

#ifndef _2019_ADVENTOFCODE_06_H_
#define _2019_ADVENTOFCODE_06_H_

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

namespace aoc2019_06 {  
  using namespace std;

  const char DELIMITER = ')';

  struct Planet {
    string name;
    string parentName;
    unordered_set<string> orbitingPlanets;
    Planet(const string &n) : name(n) {};
  };

  pair<string, string> processInput(const string &input) {
    stringstream ss;
    string id1, id2;
    for (int i = 0 ; i < input.length(); ++i) {
      if (input[i] == DELIMITER) {
        id1 = ss.str();
        ss.str(string());
      } else {
        ss << input[i];
      }
    }
    id2 = ss.str();
    return make_pair(id1,id2);
  }

  void addPlanetIfNotInMap(unordered_map<string, Planet> &planets, const string &p) {
    if (planets.find(p) == planets.end()) {
      planets.emplace(p, Planet(p));
    }
  }

  void printPlanet(const Planet &planet) {
    cout << "Planet " << planet.name;
    if (planet.orbitingPlanets.empty()) {
      cout << endl;
    } else {
      cout << " has following orbiting planets: " << endl;
    }

    for (auto &p : planet.orbitingPlanets) {
      cout << "\t" << p << endl;
    }
  }

  unordered_map<string, Planet> fillPlanets() {
    string input;
    unordered_map<string, Planet> planets;
    while (!cin.eof()) {
      cin >> input;
      auto orbitingPlanets = processInput(input);
      addPlanetIfNotInMap(planets, orbitingPlanets.first);
      addPlanetIfNotInMap(planets, orbitingPlanets.second);
      planets.at(orbitingPlanets.first).orbitingPlanets.insert(orbitingPlanets.second);
      planets.at(orbitingPlanets.second).parentName = orbitingPlanets.first;
    }
    for (auto p : planets) {  
      printPlanet(p.second);
    }
    return planets;
  }
  
  int totalOrbits(const unordered_map<string, Planet> &planets) {
    int totalOrbits = 0;
    for (const auto &p : planets) {  
      const Planet *aux = &p.second;
      while (!aux->parentName.empty()) {
        aux = &planets.at(aux->parentName);
        ++totalOrbits;
      }
    }
    return totalOrbits;
  }
  
  int minOrbitTransfers(
    const unordered_map<string, Planet> &planets, const string &p1, const string &p2) {
    int totalOrbits = 0;
    unordered_map<string,int> path;
    const Planet *aux = &planets.at(p1);
    while (!aux->parentName.empty()) {
      path.emplace(aux->parentName, ++totalOrbits);
      aux = &planets.at(aux->parentName);
    }
    cout << "From " << p1 << " to root: " << totalOrbits << endl; 

    totalOrbits = 0;
    aux = &planets.at(p2);
    while (!aux->parentName.empty()) {
      ++totalOrbits;
      if (path.find(aux->parentName) != path.end()) {
        return path.at(aux->parentName) + totalOrbits - 2;
      }
      aux = &planets.at(aux->parentName);
    }
    return totalOrbits;
  }

  void solve1() {   
    auto planets = fillPlanets();
    cout << totalOrbits(planets) << endl;
  }

  void solve2() {  
    auto planets = fillPlanets();
    cout << minOrbitTransfers(planets, "YOU", "SAN") << endl;
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

#endif /* _2019_ADVENTOFCODE_06_H_ */
