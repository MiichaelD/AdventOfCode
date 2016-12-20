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

// test
// const int MAX_ELVES = 5;
const int MAX_ELVES = 3004953;

vector<int> elves(MAX_ELVES, 1);

int incrementIndex(int &index) {
  if (++index == MAX_ELVES)
    index =  0;
  return index;
}

bool hasPresents(int index) {
  return elves[index] != 0;
}

int findNextElfWithPresents(int &index) {
  while (elves[incrementIndex(index)] == 0);
  return index;
}

int getNextElfWithPresents(int index) {
  return findNextElfWithPresents(index);
}

void steal(int from, int to){
  elves[to] += elves[from];
  elves[from] = 0;
}

int getWinningElf() {
  int index = 0;
  while (true) {
    if (hasPresents(index)) {
      // cout << "Elf " << (index+1) << " (" << elves[index] <<") stole from";
      int neighbor = getNextElfWithPresents(index);
      steal(neighbor, index);
      // cout << " elf " << (neighbor+1) << ". Now: " << elves[index] << endl;
      if (elves[index] == MAX_ELVES)
        return index;
      index = getNextElfWithPresents(neighbor);
    } else {
      findNextElfWithPresents(index);
    }
  }
  return index;
}

int main() {
  int winner = getWinningElf() + 1;
  cout << "Winning elf: " << winner << endl;
  return EXIT_SUCCESS;
}