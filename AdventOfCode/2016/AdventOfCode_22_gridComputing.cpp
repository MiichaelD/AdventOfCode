/*
 Description:   You gain access to a massive storage cluster arranged in a grid;
                each storage node is only connected to the four nodes directly
                adjacent to it (three if the node is on an edge, two if it's in
                a corner).

                You can directly access data only on node /dev/grid/node-x0-y0,
                but you can perform some limited actions on the other nodes:

                You can get the disk usage of all nodes (via df). The result of
                doing this is in your puzzle input.
                You can instruct a node to move (not copy) all of its data to an
                adjacent node (if the destination node has enough space to
                receive the data). The sending node is left empty after this
                operation.
                Nodes are named by their position: the node named node-x10-y10 
                is adjacent to nodes node-x9-y10, node-x11-y10, node-x10-y9, and
                node-x10-y11.

                Before you begin, you need to understand the arrangement of data
                on these nodes. Even though you can only move data between
                directly connected nodes, you're going to need to rearrange a
                lot of the data to get access to the data you need. Therefore,
                you need to work out how you might be able to shift data around.

                To do this, you'd like to count the number of viable pairs of
                nodes. A viable pair is any two nodes (A,B), regardless of
                whether they are directly connected, such that:

                - Node A is not empty (its Used is not zero).
                - Nodes A and B are not the same node.
                - The data on node A (its Used) would fit on node B (its Avail).
                
                How many viable pairs of nodes are there?

  Programmer:   Michael Duarte.

  Date:         12/23/2016
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

// const int WIDTH{3}, HEIGHT{3};
const int WIDTH{35}, HEIGHT{29};

struct NodeInfo {
  int x, y, size, used, avail, percent;

  void moveFrom(NodeInfo &info) {
    used += info.used;
    avail -= info.used;
    info.avail += info.used;
    info.used = 0;
  }
};

vector<vector<NodeInfo*>> matrix(HEIGHT, vector<NodeInfo*>(WIDTH));

void printInfo(const NodeInfo &n) {
  // printf("/dev/grid/node-x%d-y%d\t%dT\t%dT\t%dT\t%d%%",
  printf("/dev/grid/node-x%d-y%d\t%d\t%d\t%d\t%d%%",
         n.x, n.y, n.size, n.used, n.avail, n.percent);
}

ostream &operator<<(ostream &out,const NodeInfo &n) {
  printInfo(n);
  return out;
}

vector<NodeInfo> infos;

bool sortByLocation(const NodeInfo &n1, const NodeInfo &n2) {
  int x1 = n1.y + (n1.x * 10000);
  int x2 = n2.y + (n2.x * 10000);
  return x1 < x2;
}

bool sortByCapacity(const NodeInfo &n1, const NodeInfo &n2) {
  return n1.size > n2.size;
}

bool sortByFreeSize(const NodeInfo &n1, const NodeInfo &n2) {
  return n1.avail > n2.avail;
}

bool sortByUsedSize(const NodeInfo &n1, const NodeInfo &n2) {
  return n1.used > n2.used;
}

void printInfos() {
  printf("Filesystem\t\tSize\tUsed\tAvail\tUse%%\n");
  for (NodeInfo &in : infos) {
    cout << in << endl;
  }
  cout << endl;
}

void loadInfo() {
  infos.reserve(1015);
  string line;
  getline(cin, line);
  getline(cin, line);
  while (true) {
    infos.emplace_back();
    NodeInfo &last = infos.back();
    if (scanf("/dev/grid/node-x%d-y%d %dT %dT %dT %d%%\n",
          &last.x, &last.y, &last.size, &last.used, &last.avail, &last.percent)
        == EOF) {
      infos.pop_back();
    return;
    }
  }
}

void solvePt1() {
  int moves = 0;
  for (int i = 0 ; i < infos.size(); ++i) {
    for (int j = i+1; j < infos.size(); ++j) {
      if ( infos[i].avail >= infos[j].used ){
        // infos[i].moveFrom(infos[j]);
        ++moves;
      }
    }
  }
  cout << "Total moves: " << moves << endl;
}

void solvePt2_0() {
  int moves = 0;
  for (int i = 0 ; i < infos.size(); ++i) {
    if (infos[i].used == 0){
      cout << "Empty foud at: " << infos[i].x << "," << infos[i].y << endl;
      moves = ((infos[i].x + infos[i].y + WIDTH - 1) + (WIDTH - 2) * 5) - 2;
    }
  }
  cout << "Minimum moves: " << moves << endl;
  // for (int n1{0}; n1 < WIDTH * HEIGHT; ++n1)
  //   if (n[n1].used == 0)
  //     emptyX = (n1 % WIDTH), emptyY = (n1 / WIDTH);
  // os << (part2 ? ((emptyX + emptyY + WIDTH - 1) + (WIDTH - 2) * 5) : pairs) << std::endl;
}

NodeInfo *findEmpty() {
  for (NodeInfo &info : infos) {
    if (info.used == 0)
      return &info;
  }
  return nullptr;
}

NodeInfo *findGoal() {
  return matrix[0][WIDTH-1];
}

void matrixify() {
  for (NodeInfo &info :infos) {
    matrix[info.y][info.x] = &info;
  }
}

void printMatrix() {
  for (int r = 0; r < HEIGHT; ++r) {
    for (int c = 0; c < WIDTH; ++c) {
      if (r == 0){
        if (c == 0) {
          cout << "(.)";
          continue;
        }
        if (c == WIDTH-1){
          cout << " G ";
          continue;
        }
      }
      cout << (matrix[r][c]->used == 0 ? " _ " : " . "); 
    }
    cout << endl;
  }
  cout << endl;
}

void printData(const NodeInfo * const info) {
  cout << info->used << "/" << info->size << "\t";
}

void printDetailedMatrix() {
  for (int r = 0; r < HEIGHT; ++r) {
    for (int c = 0; c < WIDTH; ++c) {
      printData(matrix[r][c]);
    }
    cout << endl;
  }
  cout << endl;
}

bool swap(NodeInfo **from, NodeInfo **to){
  if ((*from)->used <= (*to)->avail ){
    (*to)->moveFrom(**from);
    *to = *from;
    return true;
  }
  cout << "THIS IS IMPOSSIBLE" << endl;
  exit(EXIT_SUCCESS);
  return false;
}

void solvePt2() {
  int moves = 0;
  NodeInfo *aux = nullptr;
  NodeInfo *empty = findEmpty();
  NodeInfo *goal = findGoal();
  cout << "Empty:\t" << *empty << endl << "Goal:\t"<< *goal << endl;

  // move to the left;
  while(empty->x != 0) {
    aux = matrix[empty->y][empty->x-1];
    if (swap(&aux,&empty))
      ++moves;
  }
  cout << "Moves to the left: " << moves << endl;

  // move to the top;
  while(empty->y != 0) {
    aux = matrix[empty->y-1][empty->x];
    if (swap(&aux, &empty))
      ++moves;
  }
  cout << "Moves to the top: " << moves << endl;

  // move to the right;
  while(empty->x < WIDTH - 1) {
    aux = matrix[empty->y][empty->x+1];
    if (swap(&aux,&empty))
      ++moves;
  }
  cout << "Moves to the right: " << moves << endl;

  while(empty->x != 1 || empty->y != 0){
    aux = matrix[empty->y+1][empty->x];
    if (swap(&aux,&empty))
      ++moves;
    for (int i = 1; i < 3; ++i){
      aux = matrix[empty->y][empty->x-1];
      if (swap(&aux,&empty))
      ++moves;
    }
    aux = matrix[empty->y-1][empty->x];
    if (swap(&aux,&empty))
      ++moves;

    aux = matrix[empty->y][empty->x+1];
    if (swap(&aux,&empty))
      ++moves;
  }
  cout << "Moves ciclic: " << moves << endl;

  printDetailedMatrix();
}


int main(){
  loadInfo();
  // sort(infos.begin(), infos.end(), sortByFreeSize);
  // solvePt1();
  sort(infos.begin(), infos.end(), sortByLocation);
  solvePt2_0();
  matrixify();
  printDetailedMatrix();
  // printInfos();
  solvePt2();
  return EXIT_SUCCESS;
}