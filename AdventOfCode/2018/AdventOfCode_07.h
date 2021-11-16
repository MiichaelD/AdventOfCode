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
  bool is_processing;
  int min_duration;
  explicit Node(char c) : val{c}, available{false}, processed{false} {
    min_duration = (c - 'A') + 1 + 60 ;  // Test
  }

  inline void addDependant(Node* node) {
    dependants.insert({node->val, node});
  }
  inline void addDependency(Node* node) {
    dependencies.insert({node->val, node});
  }

  inline int getDuration() {
    return min_duration;
  }

  void startProcessing() {
    if (!isAvailable()) {
      cout << "Node " << val << " is not available - can't start process" << endl; 
      return;
    }
    is_processing = true;
  }

  // returns the duration of the processing.
  void process() {
    if (!isAvailable()) {
      cout << "Node " << val << " is not available - can't  processed" << endl; 
      return;
    }
    for (auto it : dependants) {
      it.second->dependencies.erase(val);
    }
    processed = true;
    is_processing = false;
    cout << "Node " << val << ": Finished processing" << endl << endl;
  }

  bool isAvailable(bool print = false) {
    if (print) cout << "\tNode " << val << " available:  ";
    if (available) {
      if (print) cout << "YES" << endl;
      return true;
    }
    
    for (const auto entry : dependencies) {
      if (print) cout << endl << "\t\t checking dependency: " <<  entry.second->val;
      if (!entry.second->available || entry.second->is_processing || !entry.second->processed) {
        if (print) cout << " Not available" << endl;
        return false;
      }
      cout << endl;
    }
    if (print) cout << "YES" << endl;
    return (available = true);
  }
};
bool node_sorter(const Node *lhs, const Node *rhs) {
    return lhs->val < rhs->val;
}

void print(const vector<Node*> &nodes) {
  cout << '[';
  for (const Node* node : nodes) {
    cout << node->val << ", ";
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
    if (it.second->isAvailable(true) && !it.second->is_processing && !it.second->processed) {
      availableNodes.push_back(it.first);
      // return it.second;
    }
  }
  if (availableNodes.empty()) {
    cerr << "No nodes available, returning nullptr" << endl;
    return nullptr;
  }
  sort(availableNodes.begin(), availableNodes.end());
  // print(availableNodes);
  return nodes.at(availableNodes.front());
}

vector<Node*> getAvailableNodes(const unordered_map<char, Node*> &nodes) {
  vector<Node*> availableNodes;
  for (const auto it : nodes) {
    // cout << "\t checking " << it.first << endl;
    if (it.second->isAvailable(true) && !it.second->is_processing && !it.second->processed) {
      availableNodes.push_back(it.second);
      // return it.second;
    }
  }
  if (availableNodes.empty()) {
    cerr << "No nodes available, returning nullptr" << endl;
    return {};
  }
  sort(availableNodes.begin(), availableNodes.end(), &node_sorter);
  print(availableNodes);
  return availableNodes;
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

struct Worker {
  bool is_available = true;
  int pending_time = 0;
  Node* current_node = nullptr;

  void process(Node* node) {
    if (!is_available) {
      cerr << "Busy worker can't process new node. This shouldn't happen" << endl;
      return;
    }
    current_node = node;
    if (node == nullptr) {
      cerr << "Worker can't process null node. This shouldn't happen" << endl;
      return;
    }
    pending_time = node->getDuration();
    node->startProcessing();
    is_available = false;
  }

  // returns whether the worker is available
  bool tick(int time = 1) {
    if (is_available) {
      return true;
    }
    if (pending_time) {
      pending_time = max(0, pending_time - time);
    }
    if (pending_time == 0) {  // Finished processing
      is_available = true;
      current_node->process();  // mark the node as processed.
    }
    return is_available;
  }
};

struct WorkerPool {
  vector<Worker> workers_;
  int total_workers_;
  int available_workers_;
  int completed_nodes_;

  WorkerPool(int total_workers) :  total_workers_{total_workers},
  available_workers_{total_workers}, completed_nodes_{0} {
    workers_ = vector<Worker>(total_workers);
  }

  int getAvailableWorkerIndex() {
    for (int w = 0; w < total_workers_; ++w) {
      if (workers_.at(w).is_available) {
        return w;
      }
    }
    return -1;
  }

  int tick(stringstream &solution) {
    int tick_count = 1;
    int min_duration = INT_MAX;
    for (int w = 0; w < total_workers_; ++w) {
      if (workers_.at(w).is_available) {
        continue;
      }
      min_duration = min(min_duration, workers_.at(w).pending_time);
    }
    if (min_duration != INT_MAX) {
      tick_count = min_duration;
    }
    cout << "Time processing: " << tick_count << endl;
    for (int w = 0; w < total_workers_; ++w) {
      if (!workers_.at(w).is_available) {
        if (workers_.at(w).tick(tick_count)) {
          ++available_workers_;
          ++completed_nodes_;
          solution << workers_.at(w).current_node-> val;
        }
      }
    }
    return tick_count;
  }

  string process(unordered_map<char, Node*> &nodes) {
    stringstream solution;
    int processing_time = 0;
    int it = 0;
    while (completed_nodes_ < nodes.size() && ++it < 50) {
      while (available_workers_) {  // Make them busy
        vector<Node*> currentNodes = getAvailableNodes(nodes);
        if (currentNodes.empty()) {
          break;
        }
        int consumedNodes = 0;
        for (Node *currentNode : currentNodes) {
          if (currentNode->isAvailable()) {  // There is work to be done
            int availableWorkerIndex = getAvailableWorkerIndex();
            if (availableWorkerIndex != -1) {
              cout << "\tAssigning Node " << currentNode->val << " to  worker: " << availableWorkerIndex << endl << endl;
              Worker *currentWorker = &workers_.at(availableWorkerIndex);  // The available worker
              currentWorker->process(currentNode);  // Do it! 
              --available_workers_;
              ++consumedNodes;
            } else {
              cerr << "\t-More jobs available but no Workers available." << endl;
              break;
            }
          } else {
            // chill - but check email, you might be needed soon.
            cout << "\t " << available_workers_ << " chilling..." << endl;
            break;
          }
        }
        if (consumedNodes == currentNodes.size()){
          break; // No need to check for more available nodes - we consumed all.
        }
      }
      processing_time += tick(solution);
    }
    solution << ". Total processing time: " << processing_time << ". Iterations: " << it;
    return solution.str();
  }
};


void solve(int part = 1) {
  unordered_map<char, Node*> nodes;
  string input;
  while(!cin.eof()) {
    getline(cin, input);
    processInput(input, nodes);
  }
  
  if (part == 1) {
    cout << getFinishOrder(nodes) << endl << endl;
  } else {
    WorkerPool workPool(5);  // Demo
    // WorkerPool workPool(5);
    cout << "Part 2:" <<  workPool.process(nodes) << endl;
  }
}

};  // aoc2018_07

#endif /* _2018_ADVENTOFCODE_08_H_ */
