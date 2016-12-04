/*
 Description:   Finally, you come across an information kiosk with a list of
                rooms. Of course, the list is encrypted and full of decoy data,
                but the instructions to decode the list are barely hidden
                nearby. Better remove the decoy data first.

                Each room consists of an encrypted name (lowercase letters
                separated by dashes) followed by a dash, a sector ID, and a
                checksum in square brackets.

                A room is real (not a decoy) if the checksum is the five most
                common letters in the encrypted name, in order, with ties broken
                by alphabetization. For example:

                - aaaaa-bbb-z-y-x-123[abxyz] is a real room because the most
                common letters are a (5), b (3), and then a tie between x, y,
                and z, which are listed alphabetically.
                - a-b-c-d-e-f-g-h-987[abcde] is a real room because although the
                letters are all tied (1 of each), the first five are listed
                alphabetically.
                - not-a-real-room-404[oarel] is a real room.
                - totally-real-room-200[decoy] is not.
                Of the real rooms from the list above, the sum of their sector
                IDs is 1514.

                What is the sum of the sector IDs of the real rooms?
               
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

int main() {
  string line;
  int counter = 0;
  while(!cin.eof()) {
    cin >> line;
    unordered_map<char, int> repetitions;
    vector<string> vec = split(line, '-');
    fillRepetitions(repetitions, vec);

    // Sort characters by descending repetition amount or alphabetically.
    vector<pair<char, int>> repeatedChars(repetitions.begin(), repetitions.end());
    sort(repeatedChars.begin(), repeatedChars.end(), sortRepeatedChars);
    // printTop5Chars(repeatedChars);

    // get the sector ID and the 
    int num;
    string checksum;
    stringstream ss(vec.back());
    ss >> num >> checksum;
    // cout << num << "  - " << checksum << endl;

    counter += getSectorIdIfValid(num, repeatedChars, checksum);
  }
  cout << counter << endl;

  return EXIT_SUCCESS;
}