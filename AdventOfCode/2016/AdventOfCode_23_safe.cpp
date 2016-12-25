/*
  Description:  This is one of the top floors of the nicest tower in EBHQ. The
                Easter Bunny's private office is here, complete with a safe
                hidden behind a painting, and who wouldn't hide a star in a safe
                behind a painting?

                The safe has a digital screen and keypad for code entry. A
                sticky note attached to the safe has a password hint on it:
                "eggs". The painting is of a large rabbit coloring some eggs.
                You see 7.

                When you go to type the code, though, nothing appears on the
                display; instead, the keypad comes apart in your hands,
                apparently having been smashed. Behind it is some kind of socket
                - one that matches a connector in your prototype computer! You
                pull apart the smashed keypad and extract the logic circuit,
                plug it into your computer, and plug your computer into the
                safe.

                Now, you just need to figure out what output the keypad would
                have sent to the safe. You extract the assembunny code from the
                logic chip (your puzzle input).
                The code looks like it uses almost the same architecture and
                instruction set that the monorail computer used! You should be
                able to use the same assembunny interpreter for this as you did
                there, but with one new instruction:

                tgl x toggles the instruction x away (pointing at instructions
                like jnz does: positive means forward; negative means backward):

                - For one-argument instructions, inc becomes dec, and all other
                one-argument instructions become inc.
                - For two-argument instructions, jnz becomes cpy, and all other
                two-instructions become jnz.
                - The arguments of a toggled instruction are not affected.
                - If an attempt is made to toggle an instruction outside the
                program, nothing happens.
                - If toggling produces an invalid instruction (like cpy 1 2) and
                an attempt is later made to execute that instruction, skip it
                instead.
                - If tgl toggles itself (for example, if a is 0, tgl a would
                target itself and become inc a), the resulting instruction is
                not executed until the next time it is reached.

                For example, given this program:

                cpy 2 a
                tgl a
                tgl a
                tgl a
                cpy 1 a
                dec a
                dec a

                - cpy 2 a initializes register a to 2.
                - The first tgl a toggles an instruction a (2) away from it,
                which changes the third tgl a into inc a.
                - The second tgl a also modifies an instruction 2 away from it,
                which changes the cpy 1 a into jnz 1 a.
                - The fourth line, which is now inc a, increments a to 3.
                - Finally, the fifth line, which is now jnz 1 a, jumps a (3)
                instructions ahead, skipping the dec a instructions.
                
                In this example, the final value in register a is 3.

                The rest of the electronics seem to place the keypad entry (the
                number of eggs, 7) in register a, run the code, and then send
                the value left in register a to the safe.

                What value should be sent to the safe?

                --- Part Two ---
                The safe doesn't open, but it does make several angry noises to
                express its frustration.

                You're quite sure your logic is working correctly, so the only
                other thing is... you check the painting again. As it turns out,
                colored eggs are still eggs. Now you count 12.

                As you run the program with this new input, the prototype
                computer begins to overheat. You wonder what's taking so long,
                and whether the lack of any instruction more powerful than "add
                one" has anything to do with it. Don't bunnies usually multiply?

                Anyway, what value should actually be sent to the safe?

  Notes:        For part 1 a = 7
                d = a ...
                d * c ...
                7 * 6 = 42
                42 * 5 = 210
                ...
                ...
                7! = 5040

                This is obligatory:
                94 * 90 = 9306

                So answer was: 
                9306 + 5040 = 14346

                For part 2 a = 12:
                12! = 479,001,600
                
                so answer was:
                9306 + 479001600 = 479010906

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

unordered_map<char, int> registers;

void set(char reg, int value) {
  registers[reg] = value;
  cout << "setting " << reg << " = " << value << endl;
}

void initRegisters() {
  for (char c = 'a'; c < 'e'; ++c){
    set(c, 0);
  }
  // uncomment next line for part 1
  set('a', 7);
  // uncomment next line for part 2
  // set('a', 12);

}

void printRegisters() {
  for (char c = 'a'; c < 'e'; ++c){
    cout << c << " = " << registers[c] << endl;
  }
}

void increment(char reg, bool positive = true) {
  if (positive)
    set(reg, registers[reg]+1);
  else
    set(reg, registers[reg]-1);
}

void toggle(string &&value, int index, vector<string> &instructions) {
  // printRegisters();
  cout << "Toggling " << index << " + " << value << " = ";
  index += value[0] == '-' || isdigit(value[0]) ? atoi(value.c_str()) : registers[value[0]];
  cout << index ;
  if (index >= instructions.size() || index < 0) {
    cout << ". Ignoring :("<< endl;
    return;
  }
  string &instructionToToggle = instructions[index];

  cout << " from: [" << instructionToToggle << "] to: [";
  switch (instructionToToggle[0]) {
    case 'c':
    instructionToToggle[0] = 'j';
    instructionToToggle[1] = 'n';
    instructionToToggle[2] = 'z';
    break;

    case 'd':
    case 't':
    instructionToToggle[0] = 'i';
    instructionToToggle[1] = 'n';
    instructionToToggle[2] = 'c';
    break;

    case 'i':
    instructionToToggle[0] = 'd';
    instructionToToggle[1] = 'e';
    instructionToToggle[2] = 'c';
    break;

    case 'j':
    instructionToToggle[0] = 'c';
    instructionToToggle[1] = 'p';
    instructionToToggle[2] = 'y';
    break;
  }
  cout << instructionToToggle << "]" << endl;
}

void copy(const string &from, const string &to) {
  cout << "Setting " << to << " = " << from << " which is: ";
  int value = from[0] == '-'||isdigit(from[0]) ? atoi(from.c_str()) : registers[from[0]];
  if (isdigit(to[0])){
    cout << endl;
    return; // ignore command.
  }
  set(to[0], value);

}

int jump(const string &check, const string &value) {
  cout << "check (" << check << ") is ";
  int checked = isdigit(check[0]) ? atoi(check.c_str()) : registers[check[0]];
    int ivalue = value[0] == '-' || isdigit(value[0]) ? atoi(value.c_str()) : registers[value[0]];
  cout<< checked << ". Jumping: ";
  if (checked != 0 && ivalue) {
    cout << ivalue << endl; ;
    return ivalue;
  } 
  cout << "default (1) " << endl;
  return 1;
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

void printInstructions(const vector<string> &instructions) {
  cout << "There are " << instructions.size() << ": " << endl;
  for (const string &ins : instructions) {
    cout << ins << endl;
  }
  cout << endl;
}

regex rcp ("cpy ([-\\w\\d]+) ([-\\w\\d]+)");
regex rinc("inc (\\w)");
regex rdec("dec (\\w)");
regex rtgl("tgl (\\w)");
regex rjnz("jnz ([-\\w\\d]+) ([-\\w\\d]+)");

int processInstruction(vector<string> &instructions, int index) {
  std::smatch stringmatch;
  const string &instruction = instructions[index];
  cout << index << ") " << instruction << ".-\t";
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

    case 't':
    regex_match(instruction, stringmatch, rtgl);
    toggle(stringmatch[1].str(), index, instructions);
    break;

    case 'j':
    regex_match(instruction, stringmatch, rjnz);
    return jump(stringmatch[1].str(), stringmatch[2].str());

    default:
    break;
  }
  return 1;
}

void processInstructions(vector<string> instructions) {
  uint32_t processed = 0;
  for (int i = 0 ; i >= 0 && i < instructions.size();) {
    ++processed;;
    i += processInstruction(instructions, i);
  }
  cout << "Total instructions processed: " << processed << endl;
}

int main() {
  initRegisters();
  auto instructions = getInstructions();
  // printInstructions(instructions);
  processInstructions(instructions);
  cout << endl << "Result: " << registers['a'] << endl;
  printRegisters();
  return EXIT_SUCCESS;
}