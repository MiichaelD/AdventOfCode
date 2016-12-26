/*
  Description:  The computer system you're breaking into uses a weird scrambling
                function to store its passwords. It shouldn't be much trouble to
                create your own scrambled password so you can add it to the
                system; you just have to implement the scrambler.

                The scrambling function is a series of operations (the exact
                list is provided in your puzzle input). Starting with the
                password to be scrambled, apply each operation in succession to
                the string. The individual operations behave as follows:

                - swap position X with position Y means that the letters at
                indexes X and Y (counting from 0) should be swapped.
                - swap letter X with letter Y means that the letters X and Y
                should be swapped (regardless of where they appear in the
                string).
                - rotate left/right X steps means that the whole string should
                be rotated; for example, one right rotation would turn abcd into
                dabc.
                - rotate based on position of letter X means that the whole
                string should be rotated to the right based on the index of
                letter X (counting from 0) as determined before this instruction
                does any rotations. Once the index is determined, rotate the
                string to the right one time, plus a number of times equal to
                that index, plus one additional time if the index was at least
                4.
                - reverse positions X through Y means that the span of letters
                at indexes X through Y (including the letters at X and Y) should
                be reversed in order.
                - move position X to position Y means that the letter which is
                at index X should be removed from the string, then inserted such
                that it ends up at index Y.

  Example:      Suppose you start with abcde and perform the following
                operations:

                - swap position 4 with position 0 swaps the first and last
                letters, producing the input for the next step, ebcda.
                - swap letter d with letter b swaps the positions of d and b:
                edcba.
                - reverse positions 0 through 4 causes the entire string to be
                reversed, producing abcde.
                - rotate left 1 step shifts all letters left one position,
                causing the first letter to wrap to the end of the string:
                bcdea.
                - move position 1 to position 4 removes the letter at position
                1 (c), then inserts it at position 4 (the end of the string):
                bdeac.
                - move position 3 to position 0 removes the letter at position
                3 (a), then inserts it at position 0 (the front of the string):
                abdec.
                - rotate based on position of letter b finds the index of letter
                b (1), then rotates the string right once plus a number of times
                equal to that index (2): ecabd.
                - rotate based on position of letter d finds the index of letter
                d (4), then rotates the string right once, plus a number of
                times equal to that index, plus an additional time because the
                index was at least 4, for a total of 6 right rotations: decab.
                After these steps, the resulting scrambled password is decab.

                Now, you just need to generate a new scrambled password and you
                can access the system. Given the list of scrambling operations
                in your puzzle input, what is the result of scrambling abcdefgh?

  Programmer:   Michael Duarte.

  Date:         12/25/2016
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

// Means that the letters at indexes X and Y (counting from 0) should be swapped
void swapPos(string &input, int x, int y) {
  char c = input[x];
  input[x] = input[y];
  input[y] = c;
}

// Means that the letters X and Y should be swapped (regardless of where they
// appear in the string).
void swapLet(string &input, char x, char y) {
  int ix = -1, iy = -1;
  for (int i = 0; (ix == -1 || iy == -1) && i < input.length(); ++i) {
    if (input[i] == x) ix = i;
    if (input[i] == y) iy = i;
  }
  swapPos(input, ix, iy);
}

// Means that the whole string should be rotated; for example, one right
// rotation would turn abcd into dabc.
void rotateSteps(string &input, int steps, bool right = true) {
  steps %= input.length();
  if (right) {
    steps = input.length() - steps;
  } 
  rotate(input.begin(), input.begin() + steps, input.end());
}

// Means that the whole string should be rotated to the right based on the index
// of letter X (counting from 0) as determined before this instruction does any
// rotations. Once the index is determined, rotate the string to the right one
// time, plus a number of times equal to that index, plus one additional time if
// the index was at least 4.
void rotateBased(string &input, char letter) {
  int ix = -1;
  for (int i = 0; ix == -1 && i < input.length(); ++i) {
    if (input[i] == letter) ix = i;
  }
  if (ix > 3) ++ix; // additional time if index is at least 4;
  rotateSteps(input, ix+1);
}

// Means that the span of letters at indexes X through Y (including the letters
// at X and Y) should be reversed in order.
void reversePositions(string &input, int x, int y) {
  reverse(input.begin()+x, input.begin()+y+1);
}

// Means that the letter which is at index X should be removed from the string,
// then inserted such that it ends up at index Y.
void movePositions(string &input, int x, int y) {
  char c = input[x];
  if (x < y) {
    for (int i = x; i < y; ++i) {
      input[i] = input[i+1];
    }
  } else {
    for (int i = x; i > y; --i) {
      input[i] = input[i-1];
    }
  }
  input[y] = c;
}

//swap position X with position Y
regex rswappos("swap position (\\d+) with position (\\d+)");
//swap letter X with letter Y
regex rswaplet("swap letter (\\w) with letter (\\w)");
//rotate left/right X steps
regex rrotstep("rotate (\\w+) (\\d+) step[s]*");
//rotate based on position of letter X
regex rrotbase("rotate based on position of letter (\\w)");
//reverse positions X through Y
regex rrevepos("reverse positions (\\d+) through (\\d+)");
//move position X to position Y
regex rmovepos("move position (\\d+) to position (\\d+)");



void processR(string &input, const string &instruction) {
  std::smatch match;
  int x, y;
  switch(instruction[7]) {
    case 'l':
      regex_match (instruction, match, rrotstep);
      x = atoi(match[2].str().c_str());
      rotateSteps(input, x, false);
    break;

    case 'r':
      regex_match (instruction, match, rrotstep);
      x = atoi(match[2].str().c_str());
      rotateSteps(input, x, true);
    break;

    case 'b':
      regex_match (instruction, match, rrotbase);
      x = match[1].str()[0];
      rotateBased(input, x);
    break;

    case ' ':
      regex_match (instruction, match, rrevepos);
      x = atoi(match[1].str().c_str());
      y = atoi(match[2].str().c_str());
      reversePositions(input, x, y);
    break;
  }
}

void processLine(string &input, const string &instruction) {
  std::smatch match;
  switch(instruction[0]) {
    case 's':
      if (instruction[5] == 'p') {
        regex_match (instruction, match, rswappos);
        int x = atoi(match[1].str().c_str());
        int y = atoi(match[2].str().c_str());
        swapPos(input, x, y);
      } else {
        regex_match (instruction, match, rswaplet);
        char x = match[1].str()[0], y = match[2].str()[0];
        swapLet(input, x, y);
      }
    break;

    case 'r':
      processR(input, instruction);
    break;

    case 'm':
      regex_match (instruction, match, rmovepos);
      int x = atoi(match[1].str().c_str());
      int y = atoi(match[2].str().c_str());
      movePositions(input, x, y);
    break;
  }
}

void doStuff() {
  // string input = "abcde";
  // string input = "abcdefgh";
  string input = "fbgdceah";
  string instruction;
  cout << input << " being processed: " << endl;
  do{
    getline(cin, instruction);
    processLine(input, instruction);
    cout << "\t" << input << " | " << instruction << endl;
  }while(!cin.eof());
  cout << "Solution: " << input << endl;
}

void test() {
  string input = "0123456789abcdefghijklmnopqrstuvwxyz";
  cout << input << " -> ";
  // rotateSteps(input, 1, true);
  // cout << input << " -> ";
  // rotateSteps(input, 1, false);
  // cout << input << " -> ";
  // rotateSteps(input, 5, true);
  // cout << input << " -> ";
  // rotateSteps(input, 5, false);

  // reversePositions(input, 5, 10);
  cout << input << endl;
}

int main() {
  doStuff();
  return EXIT_SUCCESS;
}