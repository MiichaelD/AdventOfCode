/*
  Link:         http://adventofcode.com/2020/day/3
 
  Description: 
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/03/2020
*/

#ifndef _2021_ADVENTOFCODE_03_H_
#define _2021_ADVENTOFCODE_03_H_

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "../util/util.h"


namespace aoc2021_03 {
  using namespace std;

  inline int getPopularBit(int reps, int common) {
    cout << "\t\tReps: " << reps << " - Common: "<< common << endl;
    if (reps == common) {
      return -1; // no popular bit
    }
    return (reps > common ? 1 : 0);
  }

  int getRepetition(const vector<string> &entries,  vector<int> &indexOfInterest, int bitOfInterest) {
    int repetition = 0;
    for (int index : indexOfInterest) {
      repetition += entries[index][bitOfInterest] == '1' ? 1 : 0;
    }
    return repetition;
  }


  pair<string,string> getOxygenAndCO2Rating(
    const vector<string> &entries, vector<int> &repetitions) {
    int common = entries.size() / 2;
    const int totalRatings = 2;
    vector<int> ratingIndex(totalRatings);
    for (int rating = 0; rating < totalRatings; ++rating) {
      int repetition = repetitions.front();
      vector<int> indexOfInterest(entries.size());
      for (int i = 1; i < entries.size(); ++i) {
        indexOfInterest[i] = i;
      }
      int bitOfInterest = 0;
      while(indexOfInterest.size() > 1) {
        common = indexOfInterest.size();
        int popularBit = getPopularBit(repetition * 2, common);
        if (rating == 1 && popularBit != -1) {
          popularBit = popularBit == 1 ? 0 : 1;  // flip it
        }
        cout << (bitOfInterest + 1) << " popular bit: " << popularBit;
        cout << "\t Entries remaining: " << indexOfInterest.size() << endl;
        vector<int> indexOfInterestTmp;
        for (int index : indexOfInterest) {
          cout << "Checking index: " << index << ": " << entries[index] << endl;
          int entryBit = entries[index][bitOfInterest] - '0';
          if ((entryBit == popularBit)
              || (popularBit == -1  && ((rating == 0 && entryBit == 1) || (rating == 1 && entryBit == 0)))) {
            cout << "\tInterest: " << entries[index] << endl << endl;
            indexOfInterestTmp.push_back(index);
          }
        }
        cout << endl;
        repetition = getRepetition(entries, indexOfInterestTmp, ++bitOfInterest);
        indexOfInterest = indexOfInterestTmp;
      }
      cout << "Rating " << rating << " -> " << entries[indexOfInterest.front()] << endl << endl;
      ratingIndex[rating] = indexOfInterest.front();
    } 
    return {entries[ratingIndex.front()], entries[ratingIndex.back()]};
  }

  pair<string,string> getGammaAndEpsilon(const vector<int> &reps, int total) {
    stringstream gamma, epsilon;
    int common = total / 2;
    for (const int elementReps : reps) {
      int popularBit = getPopularBit(elementReps, common);
      gamma << popularBit;
      epsilon << (popularBit == 1 ? '0' : '1');
    }
    return {gamma.str(), epsilon.str()};
  }

  size_t solve1() {
    string input;
    vector<int> repetitions;
    int totalEntries = 0;
    while(!cin.eof()) {
      cin >> input;
      if (input.empty()) {
        break;
      }
      ++totalEntries;
      if (repetitions.size() < input.size()) {
        repetitions.resize(input.size());
      }
      // for (int i = input.size() - 1; i >= 0; --i) {
      for (int i = 0; i < input.size(); ++i) {
        repetitions[i] += input[i] == '1' ? 1 : 0;
      }
    }
    util::printVector(repetitions);
    auto gammaAndEpsilon = getGammaAndEpsilon(repetitions, totalEntries);
    util::printPair(gammaAndEpsilon, true);
    uint64_t gamma = util::binaryToDecimal(gammaAndEpsilon.first);
    uint64_t epsilon = util::binaryToDecimal(gammaAndEpsilon.second);
    cout << "Gamma: " << gamma << ". Epsilon: " << epsilon << endl;
    return gamma * epsilon;
  }

  uint64_t solve2() {
    string input;
    vector<string> entries;
    vector<int> repetitions;
    // Process input.
    while(!cin.eof()) {
      cin >> input;
      if (input.empty()) {
        break;
      }
      entries.push_back(input);
      if (repetitions.size() < input.size()) {
        repetitions.resize(input.size());
      }
      // for (int i = input.size() - 1; i >= 0; --i) {
      for (int i = 0; i < input.size(); ++i) {
        repetitions[i] += input[i] == '1' ? 1 : 0;
      }
    }
    util::printVector(repetitions);
    auto ratings = getOxygenAndCO2Rating(entries, repetitions);
    util::printPair(ratings, true);
    uint64_t oxygen = util::binaryToDecimal(ratings.first);
    uint64_t co2 = util::binaryToDecimal(ratings.second);
    cout << "Oxygen Rating: " << oxygen << ". CO2 Rating: " << co2 << endl;
    return oxygen * co2;
  }

  void solve(int part = 1) {
    using namespace std;
    cout << (part == 1 ? solve1() : solve2()) << endl;
  }

};

#endif /* _2021_ADVENTOFCODE_03_H_ */
