/*
 Description:   Now that you can think clearly, you move deeper into the
                labyrinth of hallways and office furniture that makes up this
                part of Easter Bunny HQ. This must be a graphic design
                department; the walls are covered in specifications for
                triangles. Or are they?
 
                The design document gives the side lengths of each triangle it
                describes, but... 5 10 25? Some of these aren't triangles. You
                can't help but mark the impossible ones.
 
                In a valid triangle, the sum of any two sides must be larger
                than the remaining side. For example, the "triangle" given above
                is impossible, because 5 + 10 is not larger than 25.
                 
                In your puzzle input, how many of the listed triangles are
                possible?
               
  Programmer:   Michael Duarte.

  Date:         12/03/2016
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


// You could also take an existing vector as a parameter.
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

bool sortRepeatedChars(const pair<char, int> &p1, const pair<char, int> &p2){
  if (p2.second == p1.second)
    return p1.first < p2.first;
  return p2.second < p1.second;
}

void printTop5Chars(const vector<pair<char, int>> &vec){
  auto it = vec.begin();
  if( it != vec.end()){
    cout << it->first;
    for (++it; it <vec.begin()+5; ++it){
      cout << ", " << it->first; 
    }
    cout << endl;
  }
}

void fillRepetitions(unordered_map<char, int> &reps, const vector<string> &vec){
  for (int i = 0; i < vec.size()-1; ++i){
    for (char c : vec[i]){
      auto found = reps.find(c);
      if (found == reps.end()){
        reps[c] = 1;
      } else {
        reps[c] = found->second + 1;
      }
    }
  } 
}

int getSectorIdIfValid(int id, const vector<pair<char,int>> &repeated,
    const string &checksum){

  for (int i = 1; i < 6; ++i){
    if (checksum[i] != repeated[i-1].first){
      // cout << num << "  - " << checksum << " is not Legit. " << endl;
      return 0;
    }
  }
  return id;

}

void shiftChar(char &c, int n){
  if ( ('z'- n) >= c ){
    c += n;
  } else {
    c = '`' + (n - ('z' - c) );
  }
}

void printShifted(const vector<string> &vec, int cicles){
  int n = cicles % 26;
  for (int i = 0; i < vec.size()-1; ++i){
    for (char c : vec[i]){
      // cout << "\t" << c << " + " << n << " is turning to: "; 
      shiftChar(c, n);
      cout << c;
    }
    cout << ' ';
  }
  cout << " = " << cicles << endl;
}

int main() {
  string line;
  while(!cin.eof()) {
    cin >> line;
    unordered_map<char, int> repetitions;
    vector<string> vec = split(line, '-');
    fillRepetitions(repetitions, vec);

    // get the sector ID and the 
    int num;
    string checksum;
    stringstream ss(vec.back());
    ss >> num >> checksum;
    // cout << num << "  - " << checksum << endl;

    // Sort characters by descending repetition amount or alphabetically.
    vector<pair<char, int>> repeatedChars(repetitions.begin(), repetitions.end());
    sort(repeatedChars.begin(), repeatedChars.end(), sortRepeatedChars);

    if (getSectorIdIfValid(num, repeatedChars, checksum))
      printShifted(vec, num);
  }
  return EXIT_SUCCESS;
}