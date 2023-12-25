/*
  Link:         http://adventofcode.com/2023/day/19
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/19/2023
*/

#ifndef _2023_ADVENTOFCODE_19_H_
#define _2023_ADVENTOFCODE_19_H_

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

namespace aoc2023_19 {
using namespace std;

const std::string kInitialWorkflow("in");
constexpr char kAccepted = 'A';
constexpr char kRejected = 'R';
constexpr int kMaxPredicateValue = 4000;

class Rating {
 public:
  int x,  m,  a,  s;
  Rating(const string& line) {
    int index = 3;
    x = util::getNumberRef(line, index);
    index += 3;
    m = util::getNumberRef(line, index);
    index += 3;
    a = util::getNumberRef(line, index);
    index += 3;
    s = util::getNumberRef(line, index);
  }

  void Print() const {
    cout << "Rating -  x: " << x << ", m: " << m << ", a: " << a << ", s: " << s << endl;
  }

  int GetValue(char c) const {
    switch(c) {
      case 'x': return x;
      case 'm': return m;
      case 'a': return a;
      case 's': return s;
    }
    return -1;  // This shouldn't happen.
  }

  int TotalValue() const {
    return x + m + a + s;
  }
};

class Predicate {
 public:
  char metric = ' ';
  char comparator = ' ';
  int value = -1;
  string next_node;

  Predicate(const string& token, int startInd, int endInd) {
    int i = startInd;
    for (i = startInd; i < endInd; ++i) {
      if (token[i] == ':') {
        metric = token[startInd];
        comparator = token[startInd + 1];
        value = util::getNumber(token, startInd + 2);
        break;
      }
    }
    if (i++ < endInd) {
      next_node = token.substr(i, endInd - i);
    } else {
      next_node = token.substr(startInd, endInd - startInd);
    }
  }

  void Print() const {
    cout << "Predicate - ";
    if (comparator != ' ') {
      cout << "Metric: " << metric << ", Comparator: " << comparator;
      cout << ", Value: " << value <<  ", ";
    }
    cout <<"Next Node: " << next_node << endl;
  }
};

class WorkFlow {
 public:
  string name;
  vector<Predicate> predicates;
  
  WorkFlow(const string& token) {
    int index = 0;
    for ( ; index < token.size(); ++index) {
      if (token[index] == '{') {
        name = token.substr(0, index);
        break;
      }
    }
    for (int j = ++index; j < token.size(); ++j) {
      if (token[j] == ',' || token[j] == '}') {
        predicates.emplace_back(token, index, j);
        index = ++j;
      }
    }
  }

  void Print() const {
    cout << "Workload: " << name <<  " at address: " << this  << " has:" << endl;
    for (const auto& predicate : predicates) {
      cout << "\t"; predicate.Print();
    }
    cout << endl;
  }

  std::string Process(const Rating& rating) const {
    // cout << "\tWorkflow `" << name << "` has " << predicates.size() << " predicates" << endl;
    for (const Predicate& p : predicates) {
      // cout << "\t"; p.Print();
      switch(p.comparator) {
        case '<':
          // cout << "\t\tComparing " << rating.GetValue(p.metric) << " < " << p.value << endl;
          if (rating.GetValue(p.metric) < p.value) {
            return p.next_node;
          }
          break;
        case '>': 
          // cout << "\t\tComparing " << rating.GetValue(p.metric) << " > " << p.value << endl;
          if (rating.GetValue(p.metric) > p.value) {
            return p.next_node;
          }
          break;
        case ' ':
        default:
          // cout << "\t\tReturning " << p.next_node << endl;
          return p.next_node;
      }
    }
    cerr << "\tReturning ERROR" << endl;
    return "ERROR";
  }
};

struct PuzzleInput {
  vector<WorkFlow> workflows;
  unordered_map<string, const WorkFlow*> id_to_workflow;
  vector<Rating> ratings;

  static PuzzleInput GetInput(int part) {
    PuzzleInput input;
    string line;
    while(!cin.eof()) {  // Workloads
      getline(cin, line);
      if (line.empty()) break;
      auto& workflow = input.workflows.emplace_back(line);
      // This doesn't work because when assigning we create a copy with different addresses.
      input.id_to_workflow[workflow.name] = &workflow;  
    }
    while(!cin.eof()) {  // Ratings
      getline(cin, line);
      if (line.empty()) break;
      input.ratings.emplace_back(line);
    }
    return input;
  }

  void Print() const {
    cout << "Workflows:" <<endl;
    for (const auto& entry : workflows) {
      entry.Print();
    }
    cout << endl << "Workflows map:" <<endl;
    for (const auto& entry : id_to_workflow) {
      util::printPair(entry); cout << " - Name: " << entry.second->name << endl;
    }
    cout << endl <<"Ratings:" <<endl;
    for (const auto& entry : ratings) {
      entry.Print();
    }
  }

  size_t ProcessRatings() const {
    unordered_map<string, const WorkFlow*> local_id_to_workflow;
    for (const auto& entry : workflows) {
      local_id_to_workflow[entry.name] = &entry;
    }
    size_t solution = 0;

    for (const Rating& r : ratings) {
      r.Print();
      string workflow_id = kInitialWorkflow; 
      while (true) {
        const auto it = local_id_to_workflow.find(workflow_id);
        if (it == local_id_to_workflow.end()) {   // This shouldn't happen.
          cerr << "ERROR: Workflow `" << workflow_id << "` not found." << endl;
          return 0;
        }
        workflow_id = it->second->Process(r);
        if (workflow_id[0] == kAccepted) {
          cout << "\tAccepted" << endl;
          solution += r.TotalValue();
          break;
        } else if (workflow_id[0] == kRejected) {
          cout << "\tRejected" << endl;
          break;
        }
      }
    }
    return solution;
  }

  uint64_t FindAllAcceptedCombinations() const {
    uint64_t solution = 1;
    for (const auto& workflow: workflows) {
      workflow.Print();
      for (const auto& predicate : workflow.predicates) {
        if (predicate.next_node.at(0) == kAccepted) {
          switch(predicate.comparator) {
            case '<':
              solution *= predicate.value;
              break;
            case '>':
              solution *= (kMaxPredicateValue - predicate.value);
              break;
            case ' ':
            default :
              solution *= kMaxPredicateValue;
              break;
          }
        }
      }
    }
    return solution;
  }
};

void solve(int part = 1) {
  PuzzleInput input = PuzzleInput::GetInput(part);
  input.Print();
  if (part == 1) {
    auto result = input.ProcessRatings();
    cout << "Result: " << result << endl;
  } else {
    auto result = input.FindAllAcceptedCombinations();
    cout << "Result: " << result << endl;
  }
}

};  // aoc2023_19

#endif /* _2023_ADVENTOFCODE_19_H_ */
