/*
	Description: 	"bathroom codes will no longer be written down. Instead, please
								memorize and follow the procedure below to access the bathrooms."

								The document goes on to explain that each button to be pressed
								can be found by starting on the previous button and moving to
								adjacent buttons on the keypad: U moves up, D moves down, L
								moves left, and R moves right. Each line of instructions
								corresponds to one button, starting at the previous button (or,
								for the first line, the "5" button); press whatever button you
								are on at the end of each line. If a move doesn't lead to a
								button, ignore it.

								You can't hold it much longer, so you decide to figure out the
								code as you walk to the bathroom. You picture a keypad like this:

								1 2 3
								4 5 6
								7 8 9
								Suppose your instructions are:
								ULL
								RRDDD
								LURDL
								UUUUD
								- You start at "5" and move up (to "2"), left (to "1"), and left
								(you can't, and stay on "1"), so the first button is 1.
								- Starting from the previous button ("1"), you move right twice
								(to "3") and then down three times (stopping at "9" after two
								moves and ignoring the third), ending up with 9.
								- Continuing from "9", you move left, up, right, down, and left,
								ending with 8.
								- Finally, you move up four times (stopping at "2"), then down
								once, ending with 5.
								
								So, in this example, the bathroom code is 1985.

								You finally arrive at the bathroom (it's a several minute walk
								from the lobby so visitors can behold the many fancy conference
								rooms and water coolers on this floor) and go to punch in the
								code. Much to your bladder's dismay, the keypad is not at all
								like you imagined it. Instead, you are confronted with the
								result of hundreds of man-hours of bathroom-keypad-design
								meetings:
								    1
								  2 3 4
								5 6 7 8 9
								  A B C
								    D

								You still start at "5" and stop when you're at an edge, but
								given the same instructions as above, the outcome is very
								different:

								- You start at "5" and don't move at all (up and left are both
								edges), ending at 5.
								- Continuing from "5", you move right twice and down three times
								(through "6", "7", "B", "D", "D"), ending at D.
								- Then, from "D", you move five more times (through "D", "B",
								"C", "C", "B"), ending at B.
								- Finally, after five more moves, you end at 3.
								So, given the actual keypad layout, the code would be 5DB3.

								Using the same instructions in your puzzle input, what is the
								correct bathroom code?

	Programmer:		Michael Duarte.

	Date:					12/01/2016
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;

vector<vector<char>> keypad(5, vector<char>(5));

void fill() {
	keypad[0][0] = '_';
	keypad[0][1] = '_';
	keypad[0][2] = '5';
	keypad[0][3] = '_';
	keypad[0][4] = '_';

	keypad[1][0] = '_';
	keypad[1][1] = '2';
	keypad[1][2] = '6';
	keypad[1][3] = 'A';
	keypad[1][4] = '_';

	keypad[2][0] = '1';
	keypad[2][1] = '3';
	keypad[2][2] = '7';
	keypad[2][3] = 'B';
	keypad[2][4] = 'D';

	keypad[3][0] = '_';
	keypad[3][1] = '4';
	keypad[3][2] = '8';
	keypad[3][3] = 'C';
	keypad[3][4] = '_';

	keypad[4][0] = '_';
	keypad[4][1] = '_';
	keypad[4][2] = '9';
	keypad[4][3] = '_';
	keypad[4][4] = '_';
}


bool canUp(int x, int y){
	if (y < 1 || keypad[x][y-1] == '_')
		return false;
	return true;
}

bool canDown(int x, int y){
	if (y > 3 || keypad[x][y+1] == '_')
		return false;
	return true;
}

bool canLeft(int x, int y){
	if (x < 1 || keypad[x-1][y] == '_')
		return false;
	return true;
}

bool canRight(int x, int y){
	if (x > 3 || keypad[x+1][y] == '_')
		return false;
	return true;
}


int main() {
	char c;
	string input;
	fill();
	int x = 0, y = 2;
	cout << "Starting at:\t" << keypad[x][y] << endl; 
	while(cin){
		cin >> input;
		for (int i = 0 ; i < input.length(); ++i) {
			c = input[i];
			switch(c) {
				case 'U':
					if (canUp(x,y)) --y;
					break;

				case 'R':
					if (canRight(x,y)) ++x;
					break;

				case 'D':
					if (canDown(x,y)) ++y;
					break;

				case 'L':
					if (canLeft(x,y)) --x;
					break;

				default:
					break;
			};
			// cout << "\t" << c << ": " << keypad[x][y] << endl; 
		}
		cout << keypad[x][y] << endl;
	}
	return EXIT_SUCCESS;
}