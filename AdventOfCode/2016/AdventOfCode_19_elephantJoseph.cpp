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
                So, with five Elves, the Elf that sits starting in position 3 gets all the presents.

                With the number of Elves given in your puzzle input, which Elf gets all the presents?

                Your puzzle input is 3004953.

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

int getWinningElf() {
  auto elf = elves.begin();
  while (elfCount > 1) {
    auto neighbor = getNeighbor(elf);
    // cout << elf->id << "(" << elf->presents << ") stole from " << neighbor->id;
    // cout << "(" << neighbor->presents << ") and now has: ";
    elf->stealFrom(*neighbor);
    elves.erase(neighbor);
    // cout << elf->presents << ". Remaining elves: " << elves.size() << endl;
    --elfCount;
    incrementIterator(elf);
  }
  return elf->id;
}

int main() {
  setElves();
  // printElves();
  int winner = getWinningElf();
  cout << "Winning elf: " << winner << endl;
  return EXIT_SUCCESS;
}