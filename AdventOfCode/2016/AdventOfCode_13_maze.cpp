/*
  Description:  You arrive at the first floor of this new building to discover a
                much less welcoming environment than the shiny atrium of the
                last one. Instead, you are in a maze of twisty little cubicles,
                all alike.

                Every location in this area is addressed by a pair of non-
                negative integers (x,y). Each such coordinate is either a wall
                or an open space. You can't move diagonally. The cube maze
                starts at 0,0 and seems to extend infinitely toward positive x
                and y; negative values are invalid, as they represent a location
                outside the building. You are in a small waiting area at 1,1.

                While it seems chaotic, a nearby morale-boosting poster
                explains, the layout is actually quite logical. You can
                determinewhether a given x,y coordinate will be a wall or an
                open space using a simple system:

                Find x*x + 3*x + 2*x*y + y + y*y.
                Add the office designer's favorite number (your puzzle input).
                Find the binary representation of that sum; count the number of
                bits that are 1.
                If the number of bits that are 1 is even, it's an open space.
                If the number of bits that are 1 is odd, it's a wall.
                For example, if the office designer's favorite number were 10,
                drawing walls as # and open spaces as ., the corner of the
                building containing 0,0 would look like this:

                  0123456789
                0 .#.####.##
                1 ..#..#...#
                2 #....##...
                3 ###.#.###.
                4 .##..#..#.
                5 ..##....#.
                6 #...##.###
                Now, suppose you wanted to reach 7,4. The shortest route you
                could take is marked as O:

                  0123456789
                0 .#.####.##
                1 .O#..#...#
                2 #OOO.##...
                3 ###O#.###.
                4 .##OO#OO#.
                5 ..##OOO.#.
                6 #...##.###
                Thus, reaching 7,4 would take a minimum of 11 steps (starting
                from your current location, 1,1).

                What is the fewest number of steps required for you to reach 
                31,39?

                Your puzzle input is 1350.

  Programmer:   Michael Duarte.

  Date:         12/16/2016
*/

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <vector>
#include <vector>
#include <string>
#include <cctype>
#include <tuple>
#include <regex>
#include <cmath>
#include <deque>

using namespace std;

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return (x.first * 0x1f1f1f1f) ^ x.second;
    // return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

uint64_t favoriteNumber;
unordered_map<pair<uint64_t,uint64_t>, bool, pairhash> wallCache;

void printBitRepresentation(uint64_t input) {
  do {
    cout << ( input & 1 ? '1' : '0');
    input >>= 1;
  } while (input);
}

int num1bits(uint64_t input) {
  int res = 0;
  while (input){
    input &= input - 1;
    ++res;
  }
  return res;
}

bool isWall(uint64_t x, uint64_t y) {
  auto it = wallCache.find(pair<uint64_t,uint64_t>(x,y));
  if (it != wallCache.end()) {
    // if we repeated this position return true to avoid infinite looping.
    return true;
    //return it->second;
  }
  uint64_t res = (x*x + 3*x + 2*x*y + y*y + y + favoriteNumber);
  int bits = num1bits(res);
  bool isOdd = bits % 2;
  // cout << x << ", " << y << " = " << res <<  " = " << bits << ". is Odd? ";
  // cout << isOdd << "\t";
  // printBitRepresentation(res);
  // cout << endl;
  wallCache.insert( {pair<int,int>(x,y), isOdd} );
  return isOdd;
}

int minStepsToDfs(int x, int y, int fromX = 1, int fromY = 1) {
  if ( fromX < 0 || fromY < 0 || fromX > 60 || fromY > 60 || 
      isWall(fromX,fromY)){
    // cout << ". NO! " << endl;
    return 0;
  }
  

  cout << "Checking if " << x << " & " << y << " = " << fromX << " & " << fromY;
  if (x == fromX && y == fromY){
    cout << ". SI!!!" << endl;
    return 1;
  }
  cout << endl;

  int res = minStepsToDfs(x, y, fromX+1, fromY);
  if (res) { return res + 1; }

  res = minStepsToDfs(x, y, fromX, fromY+1);
  if (res) { return res + 1; }
  
  res = minStepsToDfs(x, y, fromX-1, fromY);
  if (res) { return res + 1; }
  
  res = minStepsToDfs(x, y, fromX, fromY-1);
  if (res) { return res + 1; }
  
  return 0;
}

int minStepsToBfs(int x, int y, int fromX = 1, int fromY = 1) {
  deque<tuple<int,int, int>> cola;
  cola.emplace_back(fromX, fromY, 0);
  int iterations = 0;
  while (cola.size()) {
    const auto &front = cola.front();
    int xx = get<0>(front);
    int yy = get<1>(front);
    int it = get<2>(front);
    if (xx >=0 && yy >= 0 && !isWall(xx, yy)) {
      if (x == xx && y == yy)
        return it;

      ++it;
      cola.emplace_back(xx+1, yy,   it);
      cola.emplace_back(xx,   yy+1, it);
      cola.emplace_back(xx-1, yy,   it);
      cola.emplace_back(xx,   yy-1, it);
    }
    cola.pop_front();
  }
  return 0;
}

void printBoard() {
  cout << "  ";
  for (int i = 0; i < 40; ++i)
    cout << i;
  for (int i = 0; i < 40; ++i) {
    cout << endl << i << ' ';
    for (int j = 0; j < 40; ++j) {
      cout << (isWall(j,i) ? '#' : '.');
    }
  }
  cout << endl;
}

int main() {
  // cin >> favoriteNumber;
  // favoriteNumber = 10;
  favoriteNumber = 1350;
  // printBoard();
  // cout << minStepsToBfs(7,4) << endl;
  cout << minStepsToBfs(31,39) << endl;
  return EXIT_SUCCESS;
}