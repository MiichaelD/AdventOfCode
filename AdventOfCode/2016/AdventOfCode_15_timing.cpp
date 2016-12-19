/*
  Description:  The halls open into an interior plaza containing a large kinetic
                sculpture. The sculpture is in a sealed enclosure and seems to
                involve a set of identical spherical capsules that are carried
                to the top and allowed to bounce through the maze of spinning
                pieces.

                Part of the sculpture is even interactive! When a button is
                pressed, a capsule is dropped and tries to fall through slots in
                a set of rotating discs to finally go through a little hole at
                the bottom and come out of the sculpture. If any of the slots
                aren't aligned with the capsule as it passes, the capsule
                bounces off the disc and soars away. You feel compelled to get
                one of those capsules.

                The discs pause their motion each second and come in different
                sizes; they seem to each have a fixed number of positions at
                which they stop. You decide to call the position with the slot
                0, and count up for each position it reaches next.

                Furthermore, the discs are spaced out so that after you push the
                button, one second elapses before the first disc is reached, and
                one second elapses as the capsule passes from one disc to the
                one below it. So, if you push the button at time=100, then the
                capsule reaches the top disc at time=101, the second disc at
                time=102, the third disc at time=103, and so on.

                The button will only drop a capsule at an integer time - no
                fractional seconds allowed.

                For example, at time=0, suppose you see the following
                arrangement:

                Disc #1 has 5 positions; at time=0, it is at position 4.
                Disc #2 has 2 positions; at time=0, it is at position 1.
                If you press the button exactly at time=0, the capsule would
                start to fall; it would reach the first disc at time=1. Since
                the first disc was at position 4 at time=0, by time=1 it has
                ticked one position forward. As a five-position disc, the next
                position is 0, and the capsule falls through the slot.

                Then, at time=2, the capsule reaches the second disc. The second
                disc has ticked forward two positions at this point: it started
                at position 1, then continued to position 0, and finally ended
                up at position 1 again. Because there's only a slot at position
                0, the capsule bounces away.

                If, however, you wait until time=5 to push the button, then when
                the capsule reaches each disc, the first disc will have ticked
                forward 5+1 = 6 times (to position 0), and the second disc will
                have ticked forward 5+2 = 7 times (also to position 0). In this
                case, the capsule would fall through the discs and come out of
                the machine.

                However, your situation has more than two discs; you've noted
                their positions in your puzzle input. What is the first time you
                can press the button to get a capsule?

                --- Part Two ---

                After getting the first capsule (it contained a star! what great
                fortune!), the machine detects your success and begins to
                rearrange itself.

                When it's done, the discs are back in their original
                configuration as if it were time=0 again, but a new disc with 11
                positions and starting at position 0 has appeared exactly one
                second below the previously-bottom disc.

                With this new disc, and counting again starting from time=0 with
                the configuration in your puzzle input, what is the first time
                you can press the button to get another capsule?



  Compilation:  g++ -std=c++11 -o main AdventOfCode_15_timing.cpp 

  Programmer:   Michael Duarte.

  Date:         12/18/2016
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

regex regExp("Disc #(\\d+) has (\\d+) positions; at time=(\\d+), it is at position (\\d+).");

int maxPositions = 0;
int maxIndex = 0;

struct Disc {
  int id;
  int positions;
  int startPos;

  Disc(int i, int p, int s):id(i), positions(p), startPos(s) {}

  int getPosAtOffset(int offset){
    return (startPos + offset) % positions;
  }

  bool worksAtOffset(int offset){
    return getPosAtOffset(offset) == 0;
  }
};

ostream& operator<<(ostream &out, Disc &d) {
  out << d.id << " has  " << d.positions << " positions. starts at pos ";
  out << d.startPos;
  return out;
}

vector<Disc> discs;

void printDiscs() {
  for (Disc &disc : discs) {
    cout << disc << endl;
  }
}

void loadDiscs() {
  string input;
  smatch match;
  while(!cin.eof()) {
    getline(cin, input);
    regex_match(input, match, regExp);
    int id = atoi(match[1].str().c_str());
    int positions = atoi(match[2].str().c_str());
    int startTime = atoi(match[3].str().c_str()) - id;
    int startPos  = atoi(match[4].str().c_str());

    if (startTime) {
      startPos -= startTime;
      startPos %= positions;
      if (startPos < 0) {
        startPos = positions - startPos;
      }
    }
    discs.emplace_back( id, positions, startPos );

    if (positions > maxPositions){
      maxPositions = positions;
      maxIndex = id;
    }
  }
}

int getStartTime() {
  int offset = - discs[maxIndex].startPos;
  bool needsWork = true;
  while(needsWork) {
    needsWork = false;
    for (int i = 0 ; i < discs.size(); ++i) {
      if (!discs[i].worksAtOffset(offset)) {
        offset += discs[maxIndex].positions;
        needsWork = true;
        break;
      }
    }
  }
  return offset;
}

int main() {
  loadDiscs();
  printDiscs();
  cout << getStartTime() << endl;
  return EXIT_SUCCESS;
}