/*
  Link:         http://adventofcode.com/2020/day/20
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/20/2020
*/

#ifndef _2020_ADVENTOFCODE_20_H_
#define _2020_ADVENTOFCODE_20_H_

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

namespace aoc2020_20 {
using namespace std;

class Tile;

typedef vector<string> Matrix;
typedef vector<Tile> Tiles;
const int kSides = 4;

string flipString(const string &str) {
  stringstream ss;
  for (int i = str.size() - 1; i >= 0; --i) {
    ss << str[i];
  }
  return move(ss.str());
}

class Tile {
private:
  Matrix matrix;
  int id;
  vector<string> edges;
  vector<int> edgeMapping;
  
  const vector<string>& initEdges() {
    if (edges.empty()) {
      for (int j = 0; j < 4; ++j) {
        edgeMapping.push_back(j);
        switch(j) {
          case 0:
            edges.push_back(matrix[0]); break;
          case 1: 
            {
              stringstream ss;
              for (int i = 0; i < matrix.size(); ++i) {
                ss << matrix[i].back();
              }
              edges.emplace_back(move(ss.str()));
            }
            break;
          case 2:
            {
              stringstream ss;
              for (int i = matrix.size() -1; i >= 0; --i) {
                ss << matrix.back()[i];
              }
              edges.emplace_back(move(ss.str()));
            }
            break;
          case 3:
            {
              stringstream ss;
              for (int i = matrix.size() -1; i >= 0; --i) {
                ss << matrix[i].front();
              }
              edges.emplace_back(move(ss.str()));              
            }
            break;
        }
      }
    }
    return edges;
  }

public:
  inline int getId() const { return id; }
  inline Matrix& getMatrix() { return matrix; }
  inline const Matrix& getMatrix() const { return matrix; }

  inline int safeIndex(int index) {
    if (index < 0) {
      return kSides + index;
    }
    if (index >= kSides) {
      return index % kSides;
    }
    return index;
  }

  void rotate(bool left=true) {
    for (int i = 0; i < kSides; ++i) {
      edgeMapping[i] = safeIndex(left ? edgeMapping[i] + 1 : edgeMapping[i] - 1);
    }
  }

  vector<const string*> getEdges(bool print=false) const {
    vector<const string*> rotatedEdges(kSides, nullptr);
    for (int i = 0; i < kSides; ++i) {
      if (print) cout << (i + 1) << ": " << edges[edgeMapping[i]] << endl;
      rotatedEdges[i] = &edges[edgeMapping[i]];
    }
    return rotatedEdges;
  }

  unordered_set<string> getEdgesSet(bool flipped=false) const {
    unordered_set<string> rotatedEdges;
    for (int i = 0; i < kSides; ++i) {
      rotatedEdges.insert(
        flipped ? flipString(edges[edgeMapping[i]]) : edges[edgeMapping[i]]);
    }
    return rotatedEdges;
  }

  static Tile fromInput() {
    Tile t;
    string line;
    getline(cin, line);
    t.id = atoi(line.c_str() + 5);
    while (!cin.eof()) {
      getline(cin, line);
      if (line.empty()) {
        break;
      }
      t.matrix.push_back(line);
    }
    t.initEdges();
    return t;
  }

  void print() {
    cout << "Tile " << id << ':' << endl;
    for (int i = 0 ; i < matrix.size(); ++i) {
      for (int j = 0; j < matrix[i].size(); ++j) {
        cout << matrix[i][j];
      }
      cout << endl;
    }
    cout << endl;
  }
};

Tiles getInput() {
  Tiles tiles;
  while(!cin.eof()) {
    tiles.push_back(Tile::fromInput());
  }
  return tiles;
}

inline int getTilesPerRow(const Tiles &tiles) {
  return (int) sqrt(tiles.size());
}

void printTiles(const Tiles &tiles) {
  int tilesPerRow = getTilesPerRow(tiles);
  for (int r = 0; r < tilesPerRow; ++r) {
    for (int i = 0; i < tiles.back().getMatrix().size(); ++i) {
      if (i == 0) {
        for (int j = 0; j < tilesPerRow; ++j) {
          cout << "Tile: " << tiles[r * tilesPerRow + j].getId() << "\t";
        }
        cout << endl;
      }
      for (int j = 0; j < tilesPerRow; ++j) {
        cout << tiles[r * tilesPerRow + j].getMatrix()[i] << "\t";
      }
      cout << endl;
    }
    cout << endl;
  }
}

uint64_t getTilesMultiplication(const Tiles &tiles, const vector<int> &indexes) {
  uint64_t result = 1;
  int tilesPerRow = getTilesPerRow(tiles);
  for (int i = 0 ; i < indexes.size(); ++i) {
    cout << (i == 0 ? "" : " * ") << tiles[indexes[i]].getId();
    result *= tiles[indexes[i]].getId();
  }
  cout << " = " << result << endl;
  return result;
}

uint64_t getCornerTilesMultiplication(const Tiles &tiles) {
  int tilesPerRow = getTilesPerRow(tiles);
  return getTilesMultiplication(tiles, {0, tilesPerRow -1, (tilesPerRow - 1) * tilesPerRow, tilesPerRow * tilesPerRow - 1});
}

unordered_map<int,int> getMatchesForTile(Tiles &tiles, Tile &tile) {
  unordered_set<string> edges = tile.getEdgesSet();
  unordered_set<string> flipped = tile.getEdgesSet(true);
  edges.insert(flipped.begin(), flipped.end());

  unordered_map<int,int> matches;
  for (int j = 0; j < tiles.size(); ++j) {
    Tile &t2 = tiles[j];
    if (t2.getId() == tile.getId()) continue;
    bool foundMatchingSide = false;
    auto t2Edges = t2.getEdges();
    for (int l = 0; l < kSides ; ++l) {
      foundMatchingSide = (edges.find(*t2Edges[l]) != edges.end());
      if (foundMatchingSide) {
        matches[j] = l;
        cout << "\tMatching Tile : " << tiles[j].getId() << " @ side " << l << endl;
        break;
      }
    }
  }
  return matches;
}

vector<int> getCornerTilesIndexes(Tiles &tiles) {
  vector<int> cornerTileIndexes;
  for (int i = 0; i < tiles.size(); ++i) {
    Tile &t1 = tiles[i];
    unordered_map<int,int> matches;
    cout << "Tile : " << t1.getId() << endl;
    matches = getMatchesForTile(tiles, t1);
    cout << "\tFound " << matches.size() << " Matches." << endl;
    if (matches.size() == 2) {
      cornerTileIndexes.push_back(i);
    }
  }
  return cornerTileIndexes;
}

void solve1() {
  Tiles tiles = getInput();
  printTiles(tiles);
  auto cornerTileIndexes = getCornerTilesIndexes(tiles);

  // cout << endl << "Edges: " << endl;
  // tiles.front().getEdges();
  // tiles.front().rotate();
  // cout << endl << "Edges: " << endl;
  // tiles.front().getEdges();

  auto result = getTilesMultiplication(tiles, cornerTileIndexes);
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

};  // aoc2020_20

#endif /* _2020_ADVENTOFCODE_20_H_ */
