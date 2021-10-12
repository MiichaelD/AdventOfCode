/*
  Link:         http://adventofcode.com/2018/day/4
  Programmer:   Michael Duarte.
  Date:         10/11/2021
*/

#ifndef _2018_ADVENTOFCODE_04_
#define _2018_ADVENTOFCODE_04_

#include <algorithm> // std::sort
#include <iostream>
#include <cstdio>
#include <string>
#include <deque>
#include <cmath>
#include <cctype>
#include <climits>
#include <unordered_map>
#include <vector>

namespace aoc2018_04 {

  using namespace std;

  void print(const vector<string> &container) {
    for (const auto &e : container) {
      cout << e << endl;
    }
    cout << endl;
  }

  int getNumber(const string& line, char delimiter, int startIndex = 0) {
    int number = 0;
    for (; startIndex < line.size() && line[startIndex] != delimiter ; ++startIndex) {
      number *= 10;
      number += line[startIndex] - '0';
    }
    return number;
    
  }

  inline int getId(const string& line, int startIndex = 26) {
    return getNumber(line, ' ', 26);
  }

  inline int getMinute(const string& line, int startIndex = 26) {
    return getNumber(line, ']', 15);
  }

  struct GuardLog {
    int id;
    size_t timeAsleep = 0;
    vector<int> sleepCounter;

    GuardLog(int id) : id{id} {
      sleepCounter = vector<int>(60);
    }

    void addToLog(vector<int> &sleepHours) {
      for (int i = 0 ; i < sleepHours.size(); ++i) {
        this->sleepCounter[i] += sleepHours[i];
        timeAsleep += sleepHours[i];
        sleepHours[i] = 0;
      }
    }

    pair<int,int> minuteMostSlept() const {
      int min = 0;
      int minIndex = 0;
      for (int i = 0; i < sleepCounter.size(); ++i) {
        if (sleepCounter[i] > min) {
          min = sleepCounter[i];
          minIndex = i;
        }
      }
      return {minIndex, min} ;
    }

  };

  void solve(int part = 1) {
    string input;
    vector<string> logs;
    while (!cin.eof()) {
      getline(cin, input);
      if (!input.empty()) {
        logs.push_back(input);
      }
    }
    sort(logs.begin(), logs.end());
    print(logs);
    

    vector<int> sleepCounter(60);
    unordered_map<int, GuardLog> guardToSleepCounter;
    int mostSleepGuard = -1;
    int mostSleepMinutes = 0;
    int currentGuard = -1;
    int sleepStart = -1;
    int sleepEnd = -1;
    
    for (const auto& e : logs) {
      char action = e[19];
      // cout << "'" << action << "'" << endl;
      switch(action) {
        case 'G':
          if (currentGuard != -1) {
            auto guardIt = guardToSleepCounter.find(currentGuard);
            if (guardIt ==  guardToSleepCounter.end()) {
              guardToSleepCounter.emplace(currentGuard, GuardLog(currentGuard));
              guardIt = guardToSleepCounter.find(currentGuard);
            }
            guardIt->second.addToLog(sleepCounter);
            cout << "Guard: " << currentGuard << " Slept " << guardIt->second.timeAsleep << " minutes." << endl;
            if (guardIt->second.timeAsleep > mostSleepMinutes) {
              mostSleepMinutes = guardIt->second.timeAsleep;
              mostSleepGuard = currentGuard;
            }
          }
          currentGuard = getId(e);
          cout << "Guard: " << currentGuard << " Started shift." << endl;
          break;
        case 'f':
          sleepStart = getMinute(e);
          cout << "Guard: " << currentGuard << " Fell asleep at minute: " << sleepStart << "." << endl;
          break;
        case 'w':
          sleepEnd = getMinute(e);
          for (; sleepStart < sleepEnd; ++sleepStart) {
            ++sleepCounter[sleepStart];
          }
          sleepStart = -1;
          cout << "Guard: " << currentGuard << " Woke up at minute: " << sleepEnd << "." << endl;
          break;
        default:
          cout << e << " - This shouldn't happen - Stopping" << endl;
          return;
      }
    }

    if (currentGuard != -1) {
      auto guardIt = guardToSleepCounter.find(currentGuard);
      if (guardIt ==  guardToSleepCounter.end()) {
        guardToSleepCounter.emplace(currentGuard, GuardLog(currentGuard));
        guardIt = guardToSleepCounter.find(currentGuard);
      }
      guardIt->second.addToLog(sleepCounter);
      cout << "Guard: " << currentGuard << " Slept " << guardIt->second.timeAsleep << " minutes." << endl;
      if (guardIt->second.timeAsleep > mostSleepMinutes) {
        mostSleepMinutes = guardIt->second.timeAsleep;
        mostSleepGuard = currentGuard;
      }
    }

    if (part == 1) {
      auto guardIt = guardToSleepCounter.find(mostSleepGuard);
      int64_t mostSleptMin = guardIt->second.minuteMostSlept().first;
      cout << "The most sleeping guard: " << mostSleepGuard << ". Most slept minute: " << mostSleptMin << endl;
      cout << "Solution: " << (mostSleptMin * mostSleepGuard) << endl;
    } else {
      pair<int,int> winner(0,0);
      int winnerId = 0;
      for (const auto& p : guardToSleepCounter) {
        auto p1 = p.second.minuteMostSlept();
        if (p1.second > winner.second) {
          winner = p1;
          winnerId = p.first;
        }
      }
      auto guardIt = guardToSleepCounter.find(mostSleepGuard);
      int64_t mostSleptMin = guardIt->second.minuteMostSlept().first;
      cout << "The most reliable sleeping guard: " << winnerId << ". Most slept minute: " << winner.first << " for: " << winner.second << endl;
      cout << "Solution: " << (winner.first * winnerId) << endl;

    }
  }
};

#endif /* _2018_ADVENTOFCODE_04_ */
