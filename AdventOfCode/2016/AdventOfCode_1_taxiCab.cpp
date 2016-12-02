/*
  Description:  The Document indicates that you should start at the given
                coordinates (where you just landed) and face North. Then, follow
                the provided sequence: either turn left (L) or right (R) 90
                degrees, then walk forward the given number of blocks, ending at
                a new intersection.

                There's no time to follow such ridiculous instructions on foot,
                though, so you take a moment and work out the destination. Given
                that you can only walk on the street grid of the city, how far
                is the shortest path to the destination?

  Example:      Following R2, L3 leaves you 2 blocks East and 3 blocks North, or
                5 blocks away.
                R2, R2, R2 leaves you 2 blocks due South of your starting
                position, which is 2 blocks away.
                R5, L5, R5, R3 leaves you 12 blocks away.
                
                How many blocks away is Easter Bunny HQ?

  Programmer:   Michael Duarte.

  Date:         12/01/2016
*/

#include <iostream>
#include <locale>
#include <cmath>
using namespace std;

// skip commas, spaces and line breaks from cin.
struct comma_space_enter : std::ctype<char> {
  comma_space_enter() : std::ctype<char>(get_table()) {}
  static mask const* get_table(){
    static mask rc[table_size];
    rc[' '] = std::ctype_base::space;
    rc[','] = std::ctype_base::space;
    rc['\n'] = std::ctype_base::space;
    return &rc[0];
  }
};

bool isHorizontal = true;
int hor = 0, ver = 0;
char curOrientation = 'N';

void updateOrientation(char dir){
  // this can be improved as a ciclic list of ints 0 - 3
  // R adds 1, L removes 1.
  switch (curOrientation){
    case 'N':
      if (dir == 'R') 
        curOrientation = 'E';
      else
        curOrientation = 'W';
      break;

    case 'E':
      if (dir == 'R')
        curOrientation = 'S';
      else
        curOrientation = 'N';
      break;

    case 'S':
      if (dir == 'R')
        curOrientation = 'W';
      else
        curOrientation = 'E';
      break;

    case 'W':
      if (dir == 'R')
        curOrientation = 'N';
      else
        curOrientation = 'S';
      break;
  }
}

void addSteps(int steps) {
  switch (curOrientation) {
    case 'N':
      ver += steps;
      break;

    case 'E':
      hor += steps;
      break;

    case 'S':
      ver -= steps;
      break;

    case 'W':
      hor -= steps;
      break;
  } 
}


void process(char dir, int steps) {
  cout << "found: " << dir << ' ' << steps << endl; 
  updateOrientation(dir);
  addSteps(steps);
}

int getDistance() {
  return abs(hor) + abs(ver);
}

// figure out why this is getting the last instruction
void capture1() {
  char direction, lastDirection;
  int steps, lastSteps;
  int n = 0;
  while (std::cin){
    cin >> direction >> steps;
    if (n++){
      process(lastDirection, lastSteps);
    }
    lastDirection = direction;
    lastSteps = steps;
  } 
}

int main(){
  cin.imbue(locale(cin.getloc(), new comma_space_enter));
  capture1();
  cout << getDistance() << endl;
  return 0;
}