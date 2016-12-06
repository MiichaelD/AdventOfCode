/*
 Description:   Something is jamming your communications with Santa.
                Fortunately, your signal is only partially jammed, and protocol
                in situations like this is to switch to a simple repetition code
                to get the message through.

                In this model, the same message is sent repeatedly. You've
                recorded the repeating message signal (your puzzle input), but
                the data seems quite corrupted - almost too badly to recover.
                Almost.

                All you need to do is figure out which character is most
                frequent for each position. For example, suppose you had
                recorded the following messages:

                eedadn
                drvtee
                eandsr
                raavrd
                atevrs
                tsrnev
                sdttsa
                rasrtv
                nssdts
                ntnada
                svetve
                tesnvt
                vntsnd
                vrdear
                dvrsen
                enarar
                The most common character in the first column is e; in the
                second, a; in the third, s, and so on. Combining these
                characters returns the error-corrected message, easter.

                Given the recording in your puzzle input, what is the
                error-corrected version of the message being sent?

                Your puzzle answer was asvcbhvg
               
  Programmer:   Michael Duarte.

  Date:         12/05/2016
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;

typedef vector<unordered_map<char,int>>  vmap;

bool sortch(const pair<char, int> &p1, const pair<char, int> &p2){
  // part 1
  // return p2.second < p1.second;

  //part 2
  return p1.second < p2.second;
}


void increment (char c, unordered_map<char, int> &posMap) {
  auto found = posMap.find(c);
  if (found == posMap.end()){
    posMap[c] = 1;
  } else {
    posMap[c] = found->second + 1;
  }
}

void processLine(const string &line, vmap &positions){
  for (int i = 0;  i  < line.length(); ++i){
    increment(line[i], positions[i]);
  }
}

vector<char> getDecoded(const vmap &positions){
  vector<char> res(positions.size());
  for (int i = 0 ; i < positions.size(); ++i){
    const auto &charmap = positions[i];
    vector<pair<char,int>> chars(charmap.begin(), charmap.end());
    sort(chars.begin(), chars.end(), sortch);

    // for (auto it : chars){
    //   cout << "\t" << it.first << " -> " << it.second <<
    // } cout << endl;

    res[i] = chars[0].first;
  }
  return res;
}

void printResult(const vector<char> &res){
  for (char c : res)
    cout << c;
  cout << endl;
}

int main() {
  string line;
  cin >> line;
  int charAmount = line.length();
  vmap positions(charAmount);

  processLine(line, positions);
  
  while(!cin.eof()) {
    cin >> line;
    processLine(line, positions);
  }

  vector<char> res = getDecoded(positions);
  printResult(res);

  return EXIT_SUCCESS;
}