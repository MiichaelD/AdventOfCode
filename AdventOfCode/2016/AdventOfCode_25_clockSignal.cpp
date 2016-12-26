/*
  Description:  You open the door and find yourself on the roof. The city
                sprawls away from you for miles and miles.

                There's not much time now - it's already Christmas, but you're
                nowhere near the North Pole, much too far to deliver these stars
                to the sleigh in time.

                However, maybe the huge antenna up here can offer a solution.
                After all, the sleigh doesn't need the stars, exactly; it needs
                the timing data they provide, and you happen to have a massive
                signal generator right here.

                You connect the stars you have to your prototype computer,
                connect that to the antenna, and begin the transmission.

                Nothing happens.

                You call the service number printed on the side of the antenna
                and quickly explain the situation. "I'm not sure what kind of
                equipment you have connected over there," he says, "but you need
                a clock signal." You try to explain that this is a signal for a
                clock.

                "No, no, a clock signal - timing information so the antenna
                computer knows how to read the data you're sending it. An
                endless, alternating pattern of 0, 1, 0, 1, 0, 1, 0, 1, 0,
                1...." He trails off.

                You ask if the antenna can handle a clock signal at the
                frequency you would need to use for the data from the stars.
                "There's no way it can! The only antenna we've installed capable
                of that is on top of a top-secret Easter Bunny installation, and
                you're definitely not-" You hang up the phone.

                You've extracted the antenna's clock signal generation
                assembunny code (your puzzle input); it looks mostly compatible
                with code you worked on just recently.

                This antenna code, being a signal generator, uses one extra
                instruction:

                -out x transmits x (either an integer or the value of a register)
                as the next value for the clock signal.
                
                The code takes a value (via register a) that describes the
                signal to generate, but you're not sure how it's used. You'll
                have to find the input to produce the right signal through
                experimentation.

                What is the lowest positive integer that can be used to
                initialize register a and cause the code to output a clock
                signal of 0, 1, 0, 1... repeating forever?

                Your puzzle answer was 196.

                --- Part Two ---
                The antenna is ready. Now, all you need is the fifty stars
                required to generate the signal for the sleigh, but you don't
                have enough.

                You look toward the sky in desperation... suddenly noticing that
                a lone star has been installed at the top of the antenna! Only
                49 more to go.

                You don't have enough stars to transmit the signal, though. You
                need 8 more.

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

const int RESET_VALUE = -1000000;

int expectedOutput = 0;
int matchedClocks = 0;

void set(char reg, int value) {
  registers[reg] = value;
  // cout << "setting " << reg << " = " << value << endl;
}

void initRegisters(int aVal = 0) {
  expectedOutput = 0;
  matchedClocks = 0;
  for (char c = 'a'; c < 'e'; ++c){
    set(c, 0);
  }
  // uncomment next line for part 1
  set('a', aVal);
  // uncomment next line for part 2
  // set('a', 12);
  cout << "Trying with A: " <<aVal << endl;
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
  // cout << "Toggling " << index << " + " << value << " = ";
  index += value[0] == '-' || isdigit(value[0]) ? atoi(value.c_str()) : registers[value[0]];
  // cout << index ;
  if (index >= instructions.size() || index < 0) {
    // cout << ". Ignoring :("<< endl;
    return;
  }
  string &instructionToToggle = instructions[index];

  // cout << " from: [" << instructionToToggle << "] to: [";
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
  // cout << instructionToToggle << "]" << endl;
}

void copy(const string &from, const string &to) {
  // cout << "Setting " << to << " = " << from << " which is: ";
  int value = from[0] == '-'||isdigit(from[0]) ? atoi(from.c_str()) : registers[from[0]];
  if (isdigit(to[0])){
    // cout << endl;
    return; // ignore command.
  }
  set(to[0], value);

}

int jump(const string &check, const string &value) {
  // cout << "check (" << check << ") is ";
  int checked = isdigit(check[0]) ? atoi(check.c_str()) : registers[check[0]];
    int ivalue = value[0] == '-' || isdigit(value[0]) ? atoi(value.c_str()) : registers[value[0]];
  // cout<< checked << ". Jumping: ";
  if (checked != 0 && ivalue) {
    // cout << ivalue << endl; ;
    return ivalue;
  } 
  // cout << "default (1) " << endl;
  return 1;
}

bool transmit(const string &val){
  int value = val[0] == '-' || isdigit(val[0]) ? atoi(val.c_str()) : registers[val[0]];
  cout << "\tOUTPUT:\t" << value << endl;
  if (value == expectedOutput) {
    expectedOutput = expectedOutput == 0 ? 1 : 0;
    ++matchedClocks;
    return true;
  }
  return false;
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
regex rout("out ([-\\w\\d]+)");
regex rjnz("jnz ([-\\w\\d]+) ([-\\w\\d]+)");

int processInstruction(vector<string> &instructions, int index) {
  std::smatch stringmatch;
  const string &instruction = instructions[index];
  // cout << index << ") " << instruction << ".-\t";
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

    case 'o':
    regex_match(instruction, stringmatch, rout);
    if (!transmit(stringmatch[1].str())) {
      return RESET_VALUE;
    }
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

bool processInstructions(vector<string> instructions) {
  uint32_t processed = 0;
  for (int i = 0 ; i >= 0 && i < instructions.size();) {
    ++processed;
    int next = processInstruction(instructions, i);
    if (next == RESET_VALUE) {
      cout << "Aborting, curent value is not the expected one" << endl;
      return false;
    }
    i += next;
    if (matchedClocks == 500)
      break;

  }
  cout << "Total instructions processed: " << processed << endl;
  return true;
}

int main() {
  auto instructions = getInstructions();
  // printInstructions(instructions);
  int i = 1;
  initRegisters(i);
  for(; !processInstructions(instructions);) {
    initRegisters(++i);
  }
  cout << endl << "Result: " << i << endl;
  printRegisters();
  return EXIT_SUCCESS;
}