/*
  Link:         http://adventofcode.com/2020/day/08
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2020
*/

#ifndef _2018_ADVENTOFCODE_07_H_
#define _2018_ADVENTOFCODE_07_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
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

namespace aoc2018_07 {
using namespace std;

struct Node {
  unordered_map<char, Node*> dependants;
  unordered_map<char, Node*> dependencies;
  char val;
  bool available;
  bool processed;
  explicit Node(char c) : val{c}, available{false}, processed{false} {}

  inline void addDependant(Node* node) {
    dependants.insert({node->val, node});
  }
  inline void addDependency(Node* node) {
    dependencies.insert({node->val, node});
  }

  void process() {
    if (!isAvailable()) {
      cout << "Node " << val << " is not available - can't  processed" << endl; 
      return;
    }
    for (auto it : dependants) {
      it.second->dependencies.erase(val);
    }
    processed = true;
    cout << "Node " << val << ": Finished processing" << endl;
  }

  bool isAvailable() {
    cout << "\tNode " << val << " available:  ";
    if (available) {
      cout << "YES" << endl;
      return true;
    }
    
    cout << "checking dependencies: ";
    for (const auto entry : dependencies) {
      cout << endl << "\t\t checking dependency: " <<  entry.second->val;
      if (!entry.second->available || !entry.second->processed) {
        cout << " Not available" << endl;
        return false;
      }
      cout << endl;
    }
    cout << "YES" << endl;
    return (available = true);
  }
};

void print(const vector<char> &nodes) {
  cout << '[';
  for (char node : nodes) {
    cout << node << ", ";
  }
  cout << ']' << endl;
}

Node* getNode(char c,  unordered_map<char, Node*> &nodes) {
  auto it = nodes.find(c);
  if (it == nodes.end()) {
    Node *n = new Node(c);
    nodes[c] = n;
    return n;
  }
  return it->second;
}

void processInput(const string &input, unordered_map<char, Node*> &nodes) {
  // Step F must be finished before step E can begin.
  char c1 = input[5];
  char c2 = input[36];
  cout << "\tAdding nodes: " << c1 << " -> " << c2 << endl;
  Node *n1 = getNode(c1, nodes);
  Node *n2 = getNode(c2, nodes);
  n1->addDependant(n2);
  n2->addDependency(n1);
}

Node* getAvailableNode(const unordered_map<char, Node*> &nodes) {
  vector<char> availableNodes;
  for (const auto it : nodes) {
    // cout << "\t checking " << it.first << endl;
    if (it.second->isAvailable() && !it.second->processed) {
      availableNodes.push_back(it.first);
      // return it.second;
    }
  }
  if (availableNodes.empty()) {
    cerr << "This shouldn't happen: Not nodes available" << endl;
    return nullptr;
  }
  sort(availableNodes.begin(), availableNodes.end());
  print(availableNodes);
  return nodes.at(availableNodes.front());
}

string getFinishOrder(unordered_map<char, Node*> &nodes) {
  stringstream solution;
  solution << "Correct Order: ";
  for (int i = 0 ; i < nodes.size(); ++i) {
    Node* currentNode = getAvailableNode(nodes);
    currentNode->process();
    solution << currentNode->val;
  }
  return solution.str();
}


void solve(int part = 1) {
  unordered_map<char, Node*> nodes;
  string input;
  while(!cin.eof()) {
    getline(cin, input);
    processInput(input, nodes);
  }
  string solution = getFinishOrder(nodes);
  cout << solution << endl;
}

};  // aoc2018_07

#endif /* _2018_ADVENTOFCODE_08_H_ */
