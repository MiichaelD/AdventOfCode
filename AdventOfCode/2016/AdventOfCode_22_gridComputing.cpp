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

                Node A is not empty (its Used is not zero).
                Nodes A and B are not the same node.
                The data on node A (its Used) would fit on node B (its Avail).
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

struct NodeInfo {
  int x, y, size, used, avail, percent;
};

void printInfo(const NodeInfo &n) {
  printf("/dev/grid/node-x%d-y%d\t%dT\t%dT\t%dT\t%d%%",
         n.x, n.y, n.size, n.used, n.avail, n.percent);
}

ostream &operator<<(ostream &out,const NodeInfo &n) {
  printInfo(n);
  return out;
}

vector<NodeInfo> infos;

bool sortByLocation(const NodeInfo &n1, const NodeInfo &n2) {
  int x1 = n1.x + (n1.y * 10000);
  int x2 = n2.x + (n2.y * 10000);
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
  printf("Filesystem              Size\tUsed\tAvail\tUse%%\n");
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

int main(){
  loadInfo();
  sort(infos.begin(), infos.end(), sortByLocation);
  printInfos();
  return EXIT_SUCCESS;
}