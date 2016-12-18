/*
  Description:  You finally reach the top floor of this building: a garden with
                a slanted glass ceiling. Looks like there are no more stars to
                be had.

                While sitting on a nearby bench amidst some tiger lilies, you
                manage to decrypt some of the files you extracted from the
                servers downstairs.

                According to these documents, Easter Bunny HQ isn't just this
                building - it's a collection of buildings in the nearby area.
                They're all connected by a local monorail, and there's another
                building not far from here! Unfortunately, being night, the
                monorail is currently not operating.

                You remotely connect to the monorail control systems and
                discover that the boot sequence expects a password. The 
                password-checking logic (your puzzle input) is easy to extract,
                but the code it uses is strange: it's assembunny code designed
                for the new computer you just assembled. You'll have to execute
                the code and get the password.

                The assembunny code you've extracted operates on four registers
                (a, b, c, and d) that start at 0 and can hold any integer.
                However, it seems to make use of only a few instructions:

                - cpy x y copies x (either an integer or the value of a register)
                into register y.
                - inc x increases the value of register x by one.
                - dec x decreases the value of register x by one.
                - jnz x y jumps to an instruction y away (positive means
                forward; negative means backward), but only if x is not zero.
                The jnz instruction moves relative to itself: an offset of -1
                would continue at the previous instruction, while an offset of 2
                would skip over the next instruction.

                For example:

                cpy 41 a
                inc a
                inc a
                dec a
                jnz a 2
                dec a

                The above code would set register a to 41, increase its value by
                2, decrease its value by 1, and then skip the last dec a
                (because a is not zero, so the jnz a 2 skips it), leaving
                register a at 42. When you move past the last instruction, the
                program halts.

                After executing the assembunny code in your puzzle input, what
                value is left in register a?

                --- Part Two ---
                As you head down the fire escape to the monorail, you notice it
                didn't start; register c needs to be initialized to the position
                of the ignition key.
                
                If you instead initialize register c to be 1, what value is now
                left in register a?

  Programmer:   Michael Duarte.

  Date:         12/17/2016
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

unordered_map<char, int> registers;

void set(char reg, int value) {
  registers[reg] = value;
  // cout << "setting " << reg << " = " << value << endl;
}

void increment(char reg, bool positive = true) {
  if (positive)
    set(reg, registers[reg]+1);
  else
    set(reg, registers[reg]-1);
}

void copy(const string &from, const string &to) {
  // cout << "Setting " << to << " = " << from << " which is: ";
  int value = isdigit(from[0]) ? atoi(from.c_str()) : registers[from[0]];
  set(to[0], value);

}

int jump(const string &check, const string &value) {
  // cout << "check (" << check << ") is ";
  int checked = isdigit(check[0]) ? atoi(check.c_str()) : registers[check[0]];
  // cout<< checked << ". Jumping: ";
  if (checked != 0 ) {
    // cout << value << endl; 
    return atoi(value.c_str());
  } 
  // cout << "default (1) " << endl;
  return 1;
}

void initRegisters() {
  for (char c = 'a'; c < 'e'; ++c){
    set(c, 0);
  }
  // uncomment next line for part 2
  // set('c',1);
}

vector<string> getInstructions() {
  vector<string> output;
  string input;
  while (!cin.eof()) {
    getline(cin, input);
    output.push_back(input);
  }
  return output;
}

void printInstructions(const vector<string> instructions) {
  cout << "There are " << instructions.size() << ": " << endl;
  for (const string &ins : instructions) {
    cout << ins << endl;
  }
  cout << endl;
}

regex rcp ("cpy ([a-zA-Z0-9]+) (\\w)");
regex rinc("inc (\\w)");
regex rdec("dec (\\w)");
regex rjnz("jnz ([a-zA-Z0-9]+) ([-\\d]+)");



int processInstruction(const string &instruction) {
  // cout << "processing: " << instruction << ".-\t";
  std::smatch stringmatch;
  switch (instruction[0]) {
    case 'c':
    regex_match (instruction, stringmatch, rcp);
    copy(stringmatch[1].str(), stringmatch[2].str());
    break;

    case 'd':
    regex_match(instruction, stringmatch, rdec);
    increment(stringmatch[1].str()[0], false);
    break;

    case 'i':
    regex_match(instruction, stringmatch, rinc);
    increment(stringmatch[1].str()[0]);
    break;

    case 'j':
    regex_match(instruction, stringmatch, rjnz);
    return jump(stringmatch[1].str(), stringmatch[2].str());

    default:
    break;
  }
  return 1;
}

void processInstructions(const vector<string> instructions) {
  int processed = 0;
  for (int i = 0 ; i < instructions.size();) {
    ++processed;
    // cout << "i " << i << ")\t";
    i += processInstruction(instructions[i]);
  }
  cout << "Total instructions processed: " << processed << endl;
}

int main() {
  initRegisters();
  auto instructions = getInstructions();
  // printInstructions(instructions);
  processInstructions(instructions);
  cout << endl << "Result: " << registers['a'] << endl;
  return EXIT_SUCCESS;
}