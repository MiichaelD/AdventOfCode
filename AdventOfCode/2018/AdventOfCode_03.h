/*
  Link:         http://adventofcode.com/2018/day/3
 
  Description:  --- Day 3: No Matter How You Slice It ---
    The Elves managed to locate the chimney-squeeze prototype fabric for Santa's suit (thanks to someone who helpfully wrote its box IDs on the wall of the warehouse in the middle of the night). Unfortunately, anomalies are still affecting them - nobody can even agree on how to cut the fabric.

    The whole piece of fabric they're working on is a very large square - at least 1000 inches on each side.

    Each Elf has made a claim about which area of fabric would be ideal for Santa's suit. All claims have an ID and consist of a single rectangle with edges parallel to the edges of the fabric. Each claim's rectangle is defined as follows:

    The number of inches between the left edge of the fabric and the left edge of the rectangle.
    The number of inches between the top edge of the fabric and the top edge of the rectangle.
    The width of the rectangle in inches.
    The height of the rectangle in inches.
    A claim like #123 @ 3,2: 5x4 means that claim ID 123 specifies a rectangle 3 inches from the left edge, 2 inches from the top edge, 5 inches wide, and 4 inches tall. Visually, it claims the square inches of fabric represented by # (and ignores the square inches of fabric represented by .) in the diagram below:

    ...........
    ...........
    ...#####...
    ...#####...
    ...#####...
    ...#####...
    ...........
    ...........
    ...........
    The problem is that many of the claims overlap, causing two or more claims to cover part of the same areas. For example, consider the following claims:

    #1 @ 1,3: 4x4
    #2 @ 3,1: 4x4
    #3 @ 5,5: 2x2
    Visually, these claim the following areas:

    ........
    ...2222.
    ...2222.
    .11XX22.
    .11XX22.
    .111133.
    .111133.
    ........
    The four square inches marked with X are claimed by both 1 and 2. (Claim 3, while adjacent to the others, does not overlap either of them.)

    If the Elves all proceed with their own plans, none of them will have enough fabric. How many square inches of fabric are within two or more claims?
  
  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/19/2018
*/

#ifndef _2018_ADVENTOFCODE_03_
#define _2018_ADVENTOFCODE_03_

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace aoc2018_03 {
  
  class Claim {
  public:
    int id;
    int leftMargin;
    int topMargin;
    int width;
    int height;
    
    Claim() = default;

    Claim(int id, int lmargin, int tmargin, int w, int h) :
      id(id),leftMargin(lmargin), topMargin(tmargin), width(w), height(h) { }

    static Claim fromInputLine(const std::string &input) {
      Claim c;
      // std::cout << "\t input: " << input << std::endl; 
      sscanf(input.c_str(), "#%d @ %d,%d: %dx%d\n",
        &c.id, &c.leftMargin, &c.topMargin, &c.width, &c.height);
      return c;
    }

    int getEnd() {
        return leftMargin + width;
    }

    int getBottom() {
        return topMargin + height;
    }

    void print() {
      std::cout << id << ": lm: " << leftMargin << ", tm: " << topMargin << ", w:" << width;
      std::cout << ", h: " << height;
    }
  };

  void solve(int part = 1) {
    using namespace std;
    vector<Claim> claims;
    unordered_set<int> claimIds;
    string inputLine;
    int maxW = 0;
    int maxH = 0;
    while(!cin.eof()) {
      getline(cin, inputLine);
      if (inputLine.empty()) {
        break;
      }
      claims.emplace_back(move(Claim::fromInputLine(inputLine)));
      if (claims.back().getEnd() > maxW) {
        maxW = claims.back().getEnd();
      }
      if (claims.back().getBottom() > maxH) {
        maxH = claims.back().getBottom();
      }
      claimIds.insert(claims.back().id);
      // cout << "Added new Claim: "; claims.back().print(); cout << endl;
    }
    cout << "Claims: "<< claims.size() << ", end: " << maxW << " bottom: " << maxH << endl;

    vector<vector<unordered_set<int>>> fabric(maxW, vector<unordered_set<int>>(maxH));
    int maxSeen = 0;
    for (auto &claim : claims) {
      for (int f = claim.leftMargin; f < claim.getEnd(); ++f) {
        for (int c = claim.topMargin; c < claim.getBottom(); ++c) {
          fabric[f][c].insert(claim.id);
          if (fabric[f][c].size() > maxSeen) {
            maxSeen = fabric[f][c].size();
          }
        }
      }
    }
    cout << "Max Overlaps: " << maxSeen  << endl;
    int sqInOverlap = 0;
    for (int f = 0; f < maxW; ++f) {
      for (int c = 0; c < maxH; ++c) {
        if (fabric[f][c].size() > 1) {
          ++sqInOverlap;
          for (auto it = fabric[f][c].begin(); it != fabric[f][c].end(); ++it) {
            if (claimIds.find(*it) != claimIds.end()) {
              claimIds.erase(*it);
            }
          }
        }
      }
    }
    cout << "Sq Inches overlap: " << sqInOverlap << endl;
    cout << claimIds.size() << " non-Overlapping claim: " << *claimIds.begin() << endl;
  }

};

#endif /* _2018_ADVENTOFCODE_03_ */
