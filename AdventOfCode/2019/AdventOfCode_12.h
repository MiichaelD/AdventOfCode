/*
  Link:         http://adventofcode.com/2019/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/XX/2019
*/

#ifndef _2019_ADVENTOFCODE_12_H_
#define _2019_ADVENTOFCODE_12_H_

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

namespace aoc2019_12 {  
  using namespace std;

  const int MAX_STEPS = 2780;

  class Moon {
    int vx = 0, vy = 0, vz = 0;
    unordered_set<string> states;
    public:
    int x = 0, y = 0, z = 0;
    Moon(int x1, int y1, int z1) : x(x1), y(y1), z(z1) { 
      saveState();
    }

    void print() const {
      cout << "Pos=[x= " << x << ", y= " << y << ", z= " << z << "]\t";
      cout << "Vel=[x=" << vx << ", y= " << vy << ", z= " << vz << "]" << endl;
    }

    void applyGravity(int vx1, int vy1, int vz1) {
      vx += vx1;
      vy += vy1;
      vz += vz1;
    }

    void applyVelocity() {
      x += vx;
      y += vy;
      z += vz;
    }

    string getState() const {
      stringstream ss;
      ss << "x=" << x << " y=" << y << " z=" << z << " vx=" << vx << " vy=" << vy << " vz=" << vz;
      return ss.str();
    }

    void saveState() {
      // string state = getState();
      // if (states.find(state) != states.end()) {
      //   cout << "\t\033[1;31m\033[1;47m Duplicate found! \033[0m" << state << endl;
      // }
      states.insert(getState());
    }

    bool isStateRepeated() {
      return states.find(getState()) != states.end();
    }

    long long getPotencialEnergy() const {
      return abs(x) + abs(y) + abs(z);
    }

    long long getKineticEnergy() const {
      return abs(vx) + abs(vy) + abs(vz);
    }

    long long getTotalEnergy() const {
      return getPotencialEnergy() * getKineticEnergy();
    }
  };

  inline int getVelocity(int v1, int v2) {
    if (v1 == v2) {
      return 0;
    }
    return (v1 < v2) ? 1 : -1 ;
  }

  vector<Moon> createMoons() {
    string input;
    vector<Moon> moons;
    int x, y, z;
    while (!cin.eof()) {
      getline(cin, input);
      sscanf(input.c_str(), "<x=%d, y=%d, z=%d>", &x, &y, &z);
      moons.emplace_back(x, y, z);
    }
    return moons;
  }

  bool doStep(vector<Moon> &moons, unordered_set<string> &systemStates) {
    for (int i = 0; i < moons.size(); ++i) {
      int vx = 0, vy = 0, vz = 0;
      for (int j = 0; j < moons.size(); ++j) {
        if (i == j) continue;
        vx += getVelocity(moons[i].x, moons[j].x);
        vy += getVelocity(moons[i].y, moons[j].y);
        vz += getVelocity(moons[i].z, moons[j].z);
      }
      moons[i].applyGravity(vx, vy, vz);
    }
    int repeatedStates = 0;
    stringstream systemStateSs;
    for (auto &moon : moons) {
      moon.applyVelocity();
      moon.print();
      systemStateSs << moon.getState() << endl;
      moon.saveState();
    }
    string systemState = systemStateSs.str();
    if (systemStates.find(systemState) != systemStates.end()) {
      cout << "\tState repeated: " << systemState << endl;
      return true;
    }
    systemStates.insert(systemState);
    return false;
  }

  long long getTotalEnergy(const vector<Moon> &moons) {
    long long res = 0;
    for (auto &moon : moons) {
      res += moon.getTotalEnergy();
    }
    return res;
  }

  void solve(int part = 1) {
    vector<Moon> moons = createMoons();
    unordered_set<string> systemStates;
    for (const Moon &moon : moons) {
      moon.print();
    }
    cout << endl;
    for (int step = 1; step <= MAX_STEPS; ++step) {
      cout << "Step " << step << ": " << endl;
      if (doStep(moons, systemStates)) {
        break;
      }
    }
    cout << "Total Energy: " << getTotalEnergy(moons) << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_12_H_ */