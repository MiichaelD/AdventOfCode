/*
  Link:         http://adventofcode.com/2021/day/12
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/12/2021
*/

#ifndef _2021_ADVENTOFCODE_12_H_
#define _2021_ADVENTOFCODE_12_H_

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

namespace aoc2021_12 {
using namespace std;

typedef long long int lli;

const char *kStart = "start";
const char *kEnd = "end";

class Node {
public:
  Node(const string &name) : name_{name}, isStartEnd{name == kStart || name == kEnd} {
    // if (isStartEnd) cout << "Found a start/end node: " << name << endl;
  }

  inline bool isBigCave() const { // Big caves have uppercase names.
    return name_[0] >= 'A' && name_[0] <= 'Z';
  }

  inline bool canBeRevisited(int part = 1) const { // Whether the cave can be revisited.
    if (part == 1) return isBigCave();
    return isBigCave() || (!isStartEnd && visits < 2);
  }

  void addAdjacent(Node *adjacent) {
    adjacent_.push_back(adjacent);
  }

  void print() const {
    cout << "Node " << name_ << " has " << adjacent_.size() << " adjacent nodes." << endl;
  }
  
  inline const string& getName() {return name_;}

  inline int markVisited(bool visited) {return visited? ++visits : --visits;}

  inline int getVisitedCount() const {return visits;}

  inline bool isSmallRevisit() const {
    return !isBigCave() && !isStartEnd && visits;
  }

  vector<Node*>& getAdjacents() {
    return adjacent_;
  }

private:
  vector<Node*> adjacent_;
  int visits = 0;
  string name_;
  bool isStartEnd = false;
};

class Graph {
public:
  Graph() {}

  inline bool contains(const string &nodeName) const {
    return allNodes.find(nodeName) != allNodes.end();
  }

  inline Node* get(const string &nodeName) const {
    auto it = allNodes.find(nodeName);
    return (it != allNodes.end()) ? it->second : nullptr;
  }

  inline Node* getOrAdd(const string &nodeName) {
    Node *result = get(nodeName);
    if (result == nullptr) {
      Node *newNode = new Node(nodeName);
      allNodes[nodeName] = newNode;
      // cout << "\tAdded new node: " << nodeName << endl;
      return newNode;
    }
    // cout << "\tFound node: " << nodeName << endl;
    return result;
  }

  void addNodes(const string &nodeA, const string &nodeB) {
    cout << "Nodes: `" << nodeA << "` & `" << nodeB << '`' << endl;
    Node *a = getOrAdd(nodeA);
    Node *b = getOrAdd(nodeB);
    a->addAdjacent(b);
    b->addAdjacent(a);
    if (startNode == nullptr) {
      if (nodeA == "start") {
        startNode = a;
      } else if (nodeB == "start") {
        startNode = b;
      }
    }
    if (endNode == nullptr) {
      if (nodeA == "end") {
        endNode = a;
      } else if (nodeB == "end") {
        endNode = b;
      }
    }
  }

  void print() const {
    cout << "Graph contains: " << allNodes.size() << " Nodes." << endl;
    for (const auto &entry : allNodes) {
      cout << "\t"; entry.second->print();
    }
    cout << endl;
  }

private:
  Node* startNode = nullptr;
  Node* endNode = nullptr;
  unordered_map<string,Node*> allNodes;
};

Graph processInput() {
  Graph graph;
  stringstream ss;
  string line;
  size_t pos;
  while(!cin.eof()) {
    cin >> line;
    if ((pos = line.find('-')) != string::npos) {
      string nodeA = line.substr(0, pos++);
      string nodeB = line.substr(pos, line.size());
      graph.addNodes(nodeA, nodeB);
    }
  }
  return graph;
}

void computePaths(
  vector<vector<string>> &result, vector<string> &currentPath, unordered_set<string> &visited,
  Node *current, Node *goal, int part = 1) {
  if (current == goal) {
    currentPath.push_back(current->getName());
    result.push_back(currentPath);
    currentPath.pop_back();
    return;
  }
  current->markVisited(true);
  currentPath.push_back(current->getName());
  visited.insert(current->getName());
  for (Node *adjacent : current->getAdjacents()) {
    if (!visited.contains(adjacent->getName()) || adjacent->canBeRevisited(part)) {
      // Small cave revisits can only be done 1 per path, switch to part 1 once consumed.
      computePaths(result, currentPath, visited, adjacent, goal,
                   part != 1 && adjacent->isSmallRevisit() ? 1 : part);
    }
  }
  currentPath.pop_back();
  if (!current->markVisited(false)) {
    visited.erase(current->getName());
  }
}

vector<vector<string>> getAllPaths(Graph &graph, int part = 1) {
  vector<vector<string>> result;
  Node *start = graph.get(kStart);
  unordered_set<string> visited;
  vector<string> path;
  visited.insert(kStart);
  path.push_back(kStart);
  start->markVisited(true);
  for (Node *adjacent : start->getAdjacents()) {
    computePaths(result, path, visited, adjacent, graph.get(kEnd), part);
  }
  if (start->markVisited(false)) {
    visited.erase(kStart);
  }
  path.pop_back();
  
  return result;
}

void solve(int part = 1) {
  Graph graph = processInput();
  graph.print();
  vector<vector<string>> allPaths = getAllPaths(graph, part);
  for (const vector<string> &path : allPaths) {
    util::printVector(path);
    cout << endl;
  }
  cout << "Part " << part << ": There are " << allPaths.size() << " total paths."<< endl; 
}

};  // aoc2021_12

#endif /* _2021_ADVENTOFCODE_12_H_ */
