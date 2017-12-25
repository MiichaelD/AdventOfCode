/*

  Link:         http://adventofcode.com/2017/day/4
 
  Description:  --- Day 4: High-Entropy Passphrases ---
      A new system policy has been put in place that requires all accounts to use a passphrase
      instead of simply a password. A passphrase consists of a series of words (lowercase letters)
      separated by spaces.

      To ensure security, a valid passphrase must contain no duplicate words.

      For example:

      aa bb cc dd ee is valid.
      aa bb cc dd aa is not valid - the word aa appears more than once.
      aa bb cc dd aaa is valid - aa and aaa count as different words.
      The system's full passphrase list is available as your puzzle input. How many passphrases are
      valid?
      Your puzzle answer was 455.

      --- Part Two ---
      For added security, yet another system policy has been put in place. Now, a valid passphrase
      must contain no two words that are anagrams of each other - that is, a passphrase is invalid
      if any word's letters can be rearranged to form any other word in the passphrase.

      For example:

      abcde fghij is a valid passphrase.
      abcde xyz ecdab is not valid - the letters from the third word can be rearranged to form the
          first word
      a ab abc abd abf abj is a valid passphrase, because all letters need to be used when forming
          another word.
      iiii oiii ooii oooi oooo is valid.
      oiii ioii iioi iiio is not valid - any of these words can be rearranged to form any other word.

      Under this new system policy, how many passphrases are valid?
      Your puzzle answer was 186.

  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/23/2017
*/

#ifndef _2017_ADVENTOFCODE_04_HIGH_ENTROPY_PASSPHRASE_H_
#define _2017_ADVENTOFCODE_04_HIGH_ENTROPY_PASSPHRASE_H_

#include <climits>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

namespace aoc2017_04{

  bool isValidPassphrase(const std::string &line, bool checkAnagrams) {
    using namespace std;

    stringstream ss(line);
    unordered_set<string> seenWords;
    string word;
    while (!ss.eof()) {
      ss >> word;
      if (checkAnagrams) {
        std::sort(word.begin(), word.end());
      }
      if (seenWords.find(word) != seenWords.end()){
        return false;
      }
      seenWords.emplace(std::move(word));
    }
    cout << "Valid line: " << line << endl;
    return true;
  }

  int getValidPassphrasesCount(int part) {
    using namespace std;

    string line;
    int count = 0;
    while(getline(cin, line, '\n')) {
      if (isValidPassphrase(line, part == 2)){
        ++count;
      }
    }
    return count;
  }

  void solve(int part = 1) {
    int output = getValidPassphrasesCount(part);
    std::cout << "Output:\t" <<  output << std::endl;
  }
};

#endif /* _2017_ADVENTOFCODE_04_HIGH_ENTROPY_PASSPHRASE_H_ */
