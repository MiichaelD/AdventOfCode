/*
  Link:         http://adventofcode.com/2023/day/08
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2023
*/

#ifndef _2023_ADVENTOFCODE_08_H_
#define _2023_ADVENTOFCODE_08_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <cstring>  // strcmp()
#include <ctype.h>  // char related functions
#include <climits>  // INT_MAX and such numeric limits
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/util.h"

namespace aoc2023_08 {
using namespace std;

constexpr char kInitialSufix = 'A';
const string kInitialNodeId = "AAA";
constexpr char kFinalSufix = 'Z';
const string kFinalNodeId = "ZZZ";

struct Node {
  string id_;
  vector<string> node_ids;
  void print() const {
    cout << "Node: " << id_ << " has L: " << node_ids.front() << ", R: ";
    cout << node_ids.back();
    if (id_[2] == 'A') {
      cout << ". This is a STARTING node";
    }
    cout << endl;
  }
};

struct PuzzleInput {
  string instructions;
  unordered_map<string, Node> nodes_by_id;
  vector<const string*> starting_nodes_ids;

  static PuzzleInput getInput(int part = 1) {
    PuzzleInput input;
    getline(cin, input.instructions);
    cout << "Instructions: " << input.instructions << endl;
    string aux;
    getline(cin, aux);  // Empty line break
    while(!cin.eof()) {
      cin >> aux;
      Node& node = input.nodes_by_id[aux];
      node.id_ = aux;
      if (aux == kInitialNodeId || (part == 2 && aux[2] == kInitialSufix)) {
        input.starting_nodes_ids.push_back(&node.id_);
      }
      cin >> aux;  // =
      cin >> aux;
      node.node_ids.push_back(aux.substr(1, 3));
      cin >> aux;
      node.node_ids.push_back(aux.substr(0, 3));
      node.print();
    }
    return input;
  }
};

class Solver {
  int part = 1;
  size_t steps = 0;
  int index = -1;

  PuzzleInput& input;
  const string* node_id;

public:
  Solver(PuzzleInput& input, const string* start_id, int part = 1)
    : input{input}, node_id{start_id}, part{part} {
      cout << "Starting solver for " << *node_id << endl;
    }

  bool Step(char instruction){
    const Node& current_node = input.nodes_by_id[*node_id];
    node_id = (instruction == 'L') ? &current_node.node_ids.front()
                                   : &current_node.node_ids.back();
    bool ok = (*node_id == kFinalNodeId ||
               (part == 2 && (*node_id)[2] == kFinalSufix));
    cout << "\t -> " << *node_id << (ok ? " * " : "") << endl;
    return ok;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::getInput(part);
  int index = -1;
  uint64_t steps = 0;
  
  vector<Solver> solvers;
  vector<uint64_t> solutions;
  for (const string* starting_node_id : input.starting_nodes_ids) {
    solvers.emplace_back(input, starting_node_id, part);
    solutions.emplace_back(0);
  }

  while(true) {
    ++steps;
    if (++index >= input.instructions.size()) { index = 0;}
    char instruction = input.instructions[index];
    // cout << "\tInstruction: " << instruction << endl;
    size_t finished = 0;
    for (int s = 0; s < solvers.size(); ++s) {
      if (solvers[s].Step(instruction)) {
        ++finished;
        if (!solutions[s]) {
          solutions[s] = steps;
        }
      }
    }
    if (finished == solvers.size()){
      break; // We're done linearly.
    }
    int s = 0;
    for (; s< solutions.size(); ++s) {
      if (!solutions[s]) {
        break;
      }
    }
    if (s == solutions.size()) {
      cout << "Found a pattern on each solver at step: " << steps << endl;
      cout << "Now find the LCM / MCD of the following patterns here:\t";
      cout << "https://calculatorsoup.com/calculators/math/lcm.php" << endl;
      
      util::printVector(solutions, true);
      break;
    }
  }
  cout << "Solution " << steps << " steps." << endl;
}

};  // aoc2023_08

#endif /* _2023_ADVENTOFCODE_08_H_ */
