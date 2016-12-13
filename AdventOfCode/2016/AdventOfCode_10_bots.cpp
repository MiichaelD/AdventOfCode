/*
  Description:  You come upon a factory in which many robots are zooming around
  handing small microchips to each other.

                Upon closer examination, you notice that each bot only proceeds
                when it has two microchips, and once it does, it gives each one
                to a different bot or puts it in a marked "output" bin.
                Sometimes, bots take microchips from "input" bins, too.

                Inspecting one of the microchips, it seems like they each
                contain a single number; the bots must use some logic to decide
                what to do with each chip. You access the local control computer
                and download the bots' instructions (your puzzle input).

                Some of the instructions specify that a specific-valued
                microchip should be given to a specific bot; the rest of the
                instructions indicate what a given bot should do with its
                lower-value or higher-value chip.

  Example:      Consider the following instructions:
                - value 5 goes to bot 2
                - bot 2 gives low to bot 1 and high to bot 0
                - value 3 goes to bot 1
                - bot 1 gives low to output 1 and high to bot 0
                - bot 0 gives low to output 2 and high to output 0
                - value 2 goes to bot 2
                
                -Initially, bot 1 starts with a value-3 chip, and bot 2 starts
                with a value-2 chip and a value-5 chip.
                - Because bot 2 has two microchips, it gives its lower one (2)
                to bot 1 and its higher one (5) to bot 0.
                - Then, bot 1 has two microchips; it puts the value-2 chip in
                output 1 and gives the value-3 chip to bot 0.
                - Finally, bot 0 has two microchips; it puts the 3 in output 2
                and the 5 in output 0.
                
                In the end, output bin 0 contains a value-5 microchip, output
                bin 1 contains a value-2 microchip, and output bin 2 contains a
                value-3 microchip. In this configuration, bot number 2 is
                responsible for comparing value-5 microchips with value-2
                microchips.

                Based on your instructions, what is the number of the bot that
                is responsible for comparing value-61 microchips with value-17
                microchips?

                --- Part Two ---

                What do you get if you multiply together the values of one chip
                in each of outputs 0, 1, and 2?

  Note:         Pendings queue can be within Bots class and have some logic.
                Pimp this code.

  Programmer:   Michael Duarte.

  Date:         12/12/2016
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
#include <regex>
#include <cmath>
#include <deque>

using namespace std;

typedef std::pair<std::pair<char,int>, std::pair<char,int>> Destinations;


struct Bot {
  int id;
  Destinations to;
  vector<int> chips;
  Bot(int i, char low, int lowId, char high, int highId):
    id(i), to({low,lowId}, {high,highId}) { }
};

unordered_map<int, deque<int>> pendings;
unordered_map<int,Bot> bots;
unordered_map<int,vector<int>> outputs;

void assingToOutput(int chip, int output){
  auto it = outputs.find(output);
  if (it == outputs.end()) {
    outputs[output] = {chip};
  } else {
    outputs[output].push_back(chip);
  }
}

void printOutputs() {
  for (auto &pa :outputs){
    cout << "Output " << pa.first << ":\t";
    for (int i : pa.second) {
      cout << " " << i;
    }
    cout << endl;
  }

  int res = outputs[0].back() * outputs[1].back() * outputs[2].back();
  cout << "Multiplication of output 0, 1, and 2: " << res << endl;
}


void addPending(int chip, int botId){
  auto it =  pendings.find(botId);
  if (it == pendings.end())
    pendings[botId] = {chip};
  else 
    pendings[botId].push_back(chip);
}


void assignChipToBot(int chip, int botId) {
  auto it = bots.find(botId);
  if (it == bots.end()) {
    addPending(chip, botId);
  } else {
    Bot &bot = it->second;
    vector<int> &chips = bot.chips;
    chips.push_back(chip);
    cout << "bot " << botId << " has new chip: " << chip << endl; 
    if (chips.size() == 2){
      sort(chips.begin(), chips.end());
      int low = chips[0];
      int high = chips[1];
      cout << "\t\tBot "<<botId<<" compared: " << low << " < " << high << endl;
      chips.clear();

      const Destinations &dest= bot.to;
      if (dest.first.first == 'o') {
        assingToOutput(low, dest.first.second);
      } else {
        assignChipToBot(low, dest.first.second);
      }

      if (dest.second.first == 'o') {
        assingToOutput(high, dest.second.second);
      } else {
        assignChipToBot(high, dest.second.second);
      }
    }
  }
}

void runPendingsForBot( int botId) {
  auto it = pendings.find(botId);
  if (it == pendings.end()) return;

  auto &que = pendings[botId];
  while (que.size()) {
    assignChipToBot(que.front(), botId);
    que.pop_front();
  }
  pendings.erase(it);
}


void setRuleToBot(int botId, const string &low, int lowId,
                  const string &high, int highId) {
  Bot bot{botId, low[0], lowId, high[0], highId};
  bots.insert( {botId, bot} );
  runPendingsForBot(botId);
}


regex r("bot (\\d+) gives low to (\\w+) (\\d+) and high to ([a-zA-Z]+) (\\d+)");
regex r1("value (\\d+) goes to bot (\\d+)");

void processInstruction(const string &in){
  std::smatch stringmatch; // std::match_results<string::const_iterator> sm;
  if (in[0] == 'b'){
    regex_match (in, stringmatch, r);
    int from = atoi(stringmatch[1].str().c_str());
    string low = stringmatch[2];
    int lowd = atoi(stringmatch[3].str().c_str());
    string high = stringmatch[4];
    int highd = atoi(stringmatch[5].str().c_str());
    cout << "\tBot " << from << " gives to " << low << " " << lowd << " and ";
    cout << high << " " << highd << endl;
    setRuleToBot(from, low, lowd, high, highd);
  } else {
    regex_match (in, stringmatch, r1);
    int value = atoi(stringmatch[1].str().c_str());
    int bot = atoi(stringmatch[2].str().c_str());
    cout << "\tChip " << value << " to " << bot << endl;
    assignChipToBot(value, bot);
  }
}

int main() {
  string input;
  while(!cin.eof()){
    getline (cin,input);
    processInstruction(input);
  }
  printOutputs();
  return EXIT_SUCCESS;
}