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

  const size_t MAX_STEPS = SIZE_MAX;

  class Moon {
    int64_t vx = 0, vy = 0, vz = 0;
    public:
    int64_t x = 0, y = 0, z = 0;
    Moon(int64_t x1, int64_t y1, int64_t z1) : x(x1), y(y1), z(z1) { }

    void print() const {
      cout << getState() << endl;
    }

    void applyGravity(int64_t vx1, int64_t vy1, int64_t vz1) {
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
      ss << "Pos=[x=" << x << ", y=" << y << ", z=" << z << "]\t";
      ss << "Vel=[x=" << vx << ", y=" << vy << ", z=" << vz << "]";
      return ss.str();
    }

    int64_t getPotencialEnergy() const {
      return abs(x) + abs(y) + abs(z);
    }

    int64_t getKineticEnergy() const {
      return abs(vx) + abs(vy) + abs(vz);
    }

    int64_t getTotalEnergy() const {
      return getPotencialEnergy() * getKineticEnergy();
    }
  };

  inline int64_t getVelocity(int64_t v1, int64_t v2) {
    if (v1 == v2) {
      return 0ll;
    }
    return (v1 < v2) ? 1ll : -1ll ;
  }

  vector<Moon> createMoons() {
    string input;
    vector<Moon> moons;
    int64_t x, y, z;
    while (!cin.eof()) {
      getline(cin, input);
      sscanf(input.c_str(), "<x=%lld, y=%lld, z=%lld>", &x, &y, &z);
      moons.emplace_back(x, y, z);
    }
    return moons;
  }

  string getSystemState(const vector<Moon> &moons) {
    stringstream systemStateSs;
    for (auto &moon : moons) {
      systemStateSs << moon.getState() << endl;
    }
    return systemStateSs.str();
  }

  bool doStep(vector<Moon> &moons, unordered_set<string> &systemStates) {
    for (int i = 0; i < moons.size(); ++i) {
      int64_t vx = 0, vy = 0, vz = 0;
      for (int j = 0; j < moons.size(); ++j) {
        if (i == j) continue;
        vx += getVelocity(moons[i].x, moons[j].x);
        vy += getVelocity(moons[i].y, moons[j].y);
        vz += getVelocity(moons[i].z, moons[j].z);
      }
      moons[i].applyGravity(vx, vy, vz);
    }
    for (auto &moon : moons) {
      moon.applyVelocity();
      // moon.print();
    }
    string systemState = getSystemState(moons);
    if (systemStates.find(systemState) != systemStates.end()) {
      cout << endl << "State repeated: " << endl << systemState << endl;
      return true;
    }
    systemStates.insert(systemState);
    return false;
  }

  int64_t getTotalEnergy(const vector<Moon> &moons) {
    int64_t res = 0;
    for (auto &moon : moons) {
      res += moon.getTotalEnergy();
    }
    return res;
  }

  void solve(int part = 1) {
    vector<Moon> moons = createMoons();
    unordered_set<string> systemStates {getSystemState(moons)};
    for (const Moon &moon : moons) {
      moon.print();
    }
    cout << endl;
    for (size_t step = 1; step <= MAX_STEPS; ++step) {
      // cout << "Step " << step << ": " << endl;
      if (doStep(moons, systemStates)) {
        cout << "Repeated state @ step: " << step << endl;
        break;
      }
    }
    cout << "Total Energy: " << getTotalEnergy(moons) << endl;
  }
};

#endif /* _2019_ADVENTOFCODE_12_H_ */