/*
  Description:  The Elves contact you over a highly secure emergency channel.
                Back at the North Pole, the Elves are busy misunderstanding
                White Elephant parties.

                Each Elf brings a present. They all sit in a circle, numbered
                starting with position 1. Then, starting with the first Elf,
                they take turns stealing all the presents from the Elf to their
                left. An Elf with no presents is removed from the circle and
                does not take turns.

                For example, with five Elves (numbered 1 to 5):

                  1
                5   2
                 4 3

                Elf 1 takes Elf 2's present.
                Elf 2 has no presents and is skipped.
                Elf 3 takes Elf 4's present.
                Elf 4 has no presents and is also skipped.
                Elf 5 takes Elf 1's two presents.
                Neither Elf 1 nor Elf 2 have any presents, so both are skipped.
                Elf 3 takes Elf 5's three presents.
                So, with five Elves, the Elf that sits starting in position 3
                gets all the presents.

                With the number of Elves given in your puzzle input, which Elf
                gets all the presents?

                Your puzzle input is 3004953.

                --- Part Two ---

                Realizing the folly of their present-exchange rules, the Elves
                agree to instead steal presents from the Elf directly across the
                circle. If two Elves are across the circle, the one on the left
                (from the perspective of the stealer) is stolen from. The other
                rules remain unchanged: Elves with no presents are removed from
                the circle entirely, and the other elves move in slightly to
                keep the circle evenly spaced.

                For example, with five Elves (again numbered 1 to 5):

                - The Elves sit in a circle; Elf 1 goes first:
                  1
                5   2
                 4 3

                - Elves 3 and 4 are across the circle; Elf 3's present is stolen,
                being the one to the left. Elf 3 leaves the circle, and the rest
                of the Elves move in:
                  1           1
                5   2  -->  5   2
                 4 -          4

                - Elf 2 steals from the Elf directly across the circle, Elf 5:
                  1         1 
                -   2  -->     2
                  4         4 

                - Next is Elf 4 who, choosing between Elves 1 and 2, steals from
                Elf 1:
                 -          2  
                    2  -->
                 4          4

                - Finally, Elf 2 steals from Elf 4:
                 2
                    -->  2  
                 -

                So, with five Elves, the Elf that sits starting in position 2
                gets all the presents.

                With the number of Elves given in your puzzle input, which Elf
                now gets all the presents?

  Programmer:   Michael Duarte.

  Date:         12/19/2016
*/

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iterator>
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
#include <list>

using namespace std;

// test
// const int MAX_ELVES = 5;
const int MAX_ELVES = 3004953;

int elfCount = MAX_ELVES;

struct Elf{
  int id = 1;
  int presents = 1;
  void stealFrom(Elf &e){
    presents += e.presents;
    e.presents = 0;
  }
  bool hasPresents() {
    return presents != 0;
  }
};

ostream &operator<<(ostream &out, Elf &e) {
  out << "Elf " << e.id << " has " << e.presents << " presents.";
  return out;
}

list<Elf> elves(MAX_ELVES);

void setElves() {
  auto it = elves.begin();
  for (int i = 1; i <= MAX_ELVES; ++i){
    (it++)->id = i;
  }
}

void printElves() {
  for (Elf &e : elves) {
    cout << e << endl;
  }
  cout << endl;
}

void incrementIterator(list<Elf>::iterator &it) {
  if (++it == elves.end())
    it = elves.begin();
}

list<Elf>::iterator getNeighbor(list<Elf>::iterator elfIt) {
  incrementIterator(elfIt);
  return elfIt;
}

list<Elf>::iterator getNeighborAcross(list<Elf>::iterator elfIt) {
  int toAdvance = elfCount / 2;
  auto neighbor = elfIt;
  int toEnd = distance(elfIt, elves.end());
  if (toAdvance >= toEnd) {
    // cout << "Distance to end: " << toEnd << " but we want to advance ";
    // cout << toAdvance << endl;
    toAdvance -= toEnd;
    elfIt = elves.begin();
  }
  advance(elfIt, toAdvance);
  return elfIt;
}


int getWinningElf() {
  auto elf = elves.begin();
  while (elfCount > 1) {
    auto neighbor = getNeighborAcross(elf);
    // cout << elf->id << "(" << elf->presents << ") stole from " << neighbor->id;
    // cout << "(" << neighbor->presents << ") and now has: " << elf->presents;
    elf->stealFrom(*neighbor);
    elves.erase(neighbor);
//     cout << ". Remaining elves: " << elves.size() << endl;
    --elfCount;
    incrementIterator(elf);
  }
  return elf->id;
}

void setFastIo(){
  std::ios::sync_with_stdio(false); // http://en.cppreference.com/w/cpp/io/ios_base/sync_with_stdio
  cin.tie(NULL);
}

int main() {
//   setFastIo();
  setElves();
  // printElves();
  int winner = getWinningElf();
  cout << "Winning elf: " << winner << endl;
  return EXIT_SUCCESS;
}
