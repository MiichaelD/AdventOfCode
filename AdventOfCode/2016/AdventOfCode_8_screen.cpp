/*
 Description:   You come across a door implementing what you can only assume is
                an implementation of two-factor authentication after a long game
                of requirements telephone.

                To get past the door, you first swipe a keycard (no problem;
                there was one on a nearby desk). Then, it displays a code on a
                little screen, and you type that code on a keypad. Then,
                presumably, the door unlocks.

                Unfortunately, the screen has been smashed. After a few minutes,
                you've taken everything apart and figured out how it works. Now
                you just have to work out what the screen would have displayed.

                The magnetic strip on the card you swiped encodes a series of
                instructions for the screen; these instructions are your puzzle
                input. The screen is 50 pixels wide and 6 pixels tall, all of
                which start off, and is capable of three somewhat peculiar
                operations:

                - rect AxB turns on all of the pixels in a rectangle at the
                top-left of the screen which is A wide and B tall.
                - rotate row y=A by B shifts all of the pixels in row A (0 is
                the top row) right by B pixels. Pixels that would fall off the
                right end appear at the left end of the row.
                - rotate column x=A by B shifts all of the pixels in column A (0
                is the left column) down by B pixels. Pixels that would fall off
                the bottom appear at the top of the column.

  Example:      Here is a simple sequence on a smaller screen:

                rect 3x2 creates a small rectangle in the top-left corner:
                ###....
                ###....
                .......

                rotate column x=1 by 1 rotates the second column down by one
                pixel:
                #.#....
                ###....
                .#.....

                rotate row y=0 by 4 rotates the top row right by four pixels:
                ....#.#
                ###....
                .#.....

                rotate column x=1 by 1 again rotates the second column down by
                one pixel, causing the bottom pixel to wrap back to the top:
                .#..#.#
                #.#....
                .#.....

                As you can see, this display technology is extremely powerful,
                and will soon dominate the tiny-code-displaying-screen market.
                That's what the advertisement on the back of the display tries
                to convince you, anyway.

                There seems to be an intermediate check of the voltage used by
                the display: after you swipe your card, if the screen did work,
                how many pixels should be lit?

                --- Part Two ---

                You notice that the screen is only capable of displaying capital
                letters; in the font it uses, each letter is 5 pixels wide and 6
                tall.

                After you swipe your card, what code is the screen trying to
                display?
               
  Programmer:   Michael Duarte.

  Date:         12/08/2016
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


const char OFF_CHAR = '.';
const char ON_CHAR = '#';
const int MAX_ROWS = 6;
const int MAX_COLS = 50;
int pixelsOn = 0;

class Pixel {
private:
  char prevChar = OFF_CHAR, curChar = OFF_CHAR;

public:
  Pixel() {}
  Pixel(char c):prevChar(c),curChar(c){}

  void update() {
    prevChar = curChar;
  }

  void setChar(char c) {
    update();
    curChar = c;
  }

  void setOn(bool on = true) {
    setChar(on ? ON_CHAR : OFF_CHAR);
  }

  void flip() {
    setChar(!isOn());
  }

  char getChar() const {
    return prevChar;
  }

  bool isOn() const {
    return prevChar == ON_CHAR;
  }

};

std::ostream& operator<<(std::ostream& stream, const Pixel& pixel) {
  stream << pixel.getChar();
  return stream;
}

typedef vector<vector<Pixel>> Matrix;
Matrix matrix(MAX_ROWS, vector<Pixel>(MAX_COLS));


void printMatrix() {
  for (int r = 0; r < matrix.size(); ++r) {
    for (int c = 0; c < matrix.front().size(); ++c){
      cout << matrix[r][c];
    }
    cout << endl;
  }
  cout << "Total pixels on: " << pixelsOn << endl;
}

void createRect(int columns, int rows) {
  int newOn = columns * rows;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < columns; ++c) {
      if (matrix[r][c].isOn()) {
        --newOn;
      } else {
        matrix[r][c].setOn();
        matrix[r][c].update();
      }
    }
  }
  pixelsOn += newOn;
  printMatrix();
}

int columnIndex(int index){
  if (index >= MAX_COLS) {
    return index - MAX_COLS;
  }

  if (index < 0)
    return MAX_COLS - index;

  return index;
}

int rowIndex(int index){
  if (index >= MAX_ROWS) {
    return index - MAX_ROWS;
  }

  if (index < 0)
    return MAX_ROWS - index;

  return index;
}

void rotate(bool isColumn, int index, int pixels) {
  if (isColumn) {
    pixels %= MAX_ROWS;
    for (int r = 0; r < MAX_ROWS; ++r) {
      matrix[rowIndex(r+pixels)][index].setOn(matrix[r][index].isOn());
    }
    for (int r = 0; r < MAX_ROWS; ++r) {
      matrix[r][index].update();
    }
  } else {
    pixels %= MAX_COLS;
    for (int c = 0; c < MAX_COLS; ++c) {
      matrix[index][columnIndex(c+pixels)].setOn(matrix[index][c].isOn());
    }
    for (int c = 0; c < MAX_COLS; ++c) {
      matrix[index][c].update();
    }
  }
  printMatrix();
}

int main() {
  string instruction, param1, param2;
  while(!cin.eof()){
    cin >> instruction;
    if (instruction[1] == 'e') {
      cin >> param1;
      stringstream rec(param1);
      char c;
      int col, row;
      rec >> col >> c >> row;

      createRect(col, row);
    } else {
      cin >> param1 >> param2;
      char c = 'c';
      bool isColumn = param1[0] == c;
      int index, pixels;
      stringstream stream(param2);
      stream >> c >> c >> index;
      cin >> param1 >> pixels;

      rotate(isColumn, index, pixels);
    }
  }
  printMatrix();
  return EXIT_SUCCESS;
}