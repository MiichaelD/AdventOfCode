/*
  Link:         http://adventofcode.com/2020/day/23
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/23/2020
*/

#ifndef _2020_ADVENTOFCODE_23_H_
#define _2020_ADVENTOFCODE_23_H_

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

namespace aoc2020_23 {
using namespace std;

const int kMaxNodes = 10;
const int kRounds = 100;
const int kPickAmount = 3;

struct Node {
  Node *next, *prev;
  int value;
  Node(int i) : value{i}, next{nullptr}, prev{nullptr} { }
  ~Node() {
    next = prev = nullptr;
    // cout << "\tReleased " << value << endl;
  }
};

inline int digitValue(const string &str, int index = 0) {
  return str[index] - '0';
}

vector<Node*> createCircularList(const string &input) {
  vector<Node*> nodes(kMaxNodes, nullptr);
  int value = digitValue(input, 0);
  Node *head = new Node(value), *aux = head;
  nodes[value] = head;
  for (int i = 1; i < input.size(); ++i) {
    value = digitValue(input, i);
    aux->next = new Node(value);
    aux = aux->next;
    nodes[aux->value] = aux;
  }
  aux->next = head;
  return nodes;
}

void releaseRecursively(Node *node) {
  Node *next = nullptr, *oldHead = node;
  while (node && next != oldHead) {
    next = node->next;
    delete node;
    node = next;
  }
}

inline void releaseNodes(vector<Node*> &nodes) {
  for (Node *n : nodes) { if (n) { delete n; } }
  nodes.clear();
}

void print(Node *node, Node *current=nullptr) {
  Node* aux = node;
  do {
    if (current == aux) {
      cout << '(' << aux->value << ")-> ";
    } else {
      cout << aux->value << "-> ";
    }
    aux = aux->next;
  } while(aux && node != aux);
  cout << endl;
}

void printLabelsAfter1(Node *node) {
  while (node->value != 1) {
    node = node->next;
  }
  node = node->next;
  while (node->value != 1){
    cout << node->value;
    node = node->next;
  }
}


Node* getNextPick(Node *current) {
  Node *result = current->next;
  Node *aux = current;
  for (int i = 0; i < kPickAmount; ++i) {
    aux = aux->next;
  }
  current->next = aux->next;
  aux->next = result;
  return result;
}

bool isDestinationInPicked(int dest, Node *picked) {
  for (int i = 0; i < kPickAmount; ++i) {
    if (picked->value == dest)
      return true;
    picked = picked->next;
  }
  return false;
}

int getDestination(Node *current, Node *picked) {
  int destination = current->value;
  while (true) {
    while (isDestinationInPicked(--destination, picked) && destination > 0);
    if (destination > 0) break;
    destination = kMaxNodes;
  }
  return destination;
}

void append(Node *dest, Node *picked) {
  Node *next = dest->next;
  dest->next = picked;
  for (int i = 1; i < kPickAmount; ++i) {
    picked = picked->next;
  }
  picked->next = next;
}

void play(const vector<Node*> &nodes, Node *head) {
  Node *aux = head, *current = head;
  for (int i = 1; i <= kRounds; ++i) {
    cout << "-- Move " << i << "--" << endl;
    cout << "cups:     "; print(current, current);
    Node *picked = getNextPick(current);
    int dest = getDestination(current, picked);
    cout << "pick up:   "; print(picked);
    cout << "dest node: " << dest <<  endl;
    current = current->next;
    append(nodes[dest], picked);
    cout << endl;
  }
}

void solve(int part = 1) {
  string input;
  cin >> input;
  vector<Node*> nodes = createCircularList(input);
  Node* head = nodes[digitValue(input)];
  print(head);
  play(nodes, head);
  printLabelsAfter1(head);
  // releaseRecursively(head);
  releaseNodes(nodes);
}

};  // aoc2020_23

#endif /* _2020_ADVENTOFCODE_23_H_ */
