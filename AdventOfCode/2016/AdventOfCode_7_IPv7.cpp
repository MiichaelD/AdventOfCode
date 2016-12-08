/*
 Description:   While snooping around the local network of EBHQ, you compile a
                list of IP addresses (they're IPv7, of course; IPv6 is much too
                limited). You'd like to figure out which IPs support TLS
                (transport-layer snooping).

                An IP supports TLS if it has an Autonomous Bridge Bypass
                Annotation, or ABBA. An ABBA is any four-character sequence
                which consists of a pair of two different characters followed by
                the reverse of that pair, such as xyyx or abba. However, the IP
                also must not have an ABBA within any hypernet sequences, which
                are contained by square brackets.

 Example:       - abba[mnop]qrst supports TLS (abba outside square brackets).
                - abcd[bddb]xyyx does not support TLS (bddb is within square
                brackets, even though xyyx is outside square brackets).
                - aaaa[qwer]tyui does not support TLS (aaaa is invalid; the
                interior characters must be different).
                - ioxxoj[asdfgh]zxcvbn supports TLS (oxxo is outside square
                brackets, even though it's within a larger string).


                How many IPs in your puzzle input support TLS?

                --- Part Two ---
                You would also like to know which IPs support SSL (super-secret
                listening).

                An IP supports SSL if it has an Area-Broadcast Accessor, or ABA,
                anywhere in the supernet sequences (outside any square bracketed
                sections), and a corresponding Byte Allocation Block, or BAB,
                anywhere in the hypernet sequences. An ABA is any three-
                character sequence which consists of the same character twice
                with a different character between them, such as xyx or aba. A
                corresponding BAB is the same characters but in reversed
                positions: yxy and bab, respectively.

  Example:      - aba[bab]xyz supports SSL (aba outside square brackets with
                corresponding bab within square brackets).
                - xyx[xyx]xyx does not support SSL (xyx, but no corresponding
                yxy).
                - aaa[kek]eke supports SSL (eke in supernet with corresponding
                kek in hypernet; the aaa sequence is not related, because the
                interior character must be different).
                - zazbz[bzb]cdb supports SSL (zaz has no corresponding aza, but
                zbz has a corresponding bzb, even though zaz and zbz overlap).

                How many IPs in your puzzle input support SSL?
               
  Programmer:   Michael Duarte.

  Date:         12/07/2016
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> splitIntoSections(const string &line){
  vector<string> result;
  result.reserve(3);

  stringstream ss;

  for (int i = 0; i < line.length(); ++i) {
    if (line[i] == '[' || line[i] == ']'){
      result.push_back(ss.str());
      ss.str(string());
      ss.clear();
    } else {
      ss << line[i];
    }
  }
  result.push_back(ss.str());
  ss.str(string());
  ss.clear();
  return result;
}

void printSections(const vector<string> &sections){
  for (const string &section : sections) {
    cout << section << endl;
  }
}

bool hasAbba(const string &section) {
  int length = section.length() - 3;
  for (int i = 0; i < length; ++i) {
    if (section[i] == section[i+3] &&  section[i+1] == section[i+2] &&
        section[i] != section[i+1])
      return true;
  }
  return false;
}

bool doesSupportTls(const string &line) {
  vector<string> sections = splitIntoSections(line);
  // printSections(sections);

  for (int i = 1; i < sections.size(); i += 2)
    if (hasAbba(sections[i]))
      return false;


  for (int i = 0; i < sections.size(); i += 2)
    if (hasAbba(sections[i]))
      return true;

  return false;
}

int getAbaIndex(const string &section, int startIndex = 0) {
  int length = section.length() - 2;
  for (int i = startIndex; i < length; ++i) {
    if (section[i] == section[i+2] && section[i] != section[i+1])
      return i;
  }
  return -1;
}

int getBabIndex(const string &section, char a, char b, int startIndex = 0) {
  int length = section.length() - 2;
  for (int i = startIndex; i < length; ++i) {
    if (section[i] == b && section[i+1] == a && section[i+2] == b)
      return i;
  }
  return -1;
}


bool doesSupportSsl(const string &line) {
  vector<string> sections = splitIntoSections(line);

  for (int i = 0; i < sections.size(); i += 2){
    const string &supernet = sections[i];
    int abaIndex = -1;
    while (true) {
      abaIndex = getAbaIndex(supernet, abaIndex + 1);
      if (abaIndex == -1)
        break;

      // cout << "\tchecking: " << supernet[abaIndex] << " & " << supernet[abaIndex+1];
      // cout << " from: " << supernet << endl;
      for (int j = 1; j < sections.size(); j += 2) {
        const string &hypernet = sections[j];
        int babIndex = -1;
        while(true) {
          babIndex = getBabIndex(hypernet, supernet[abaIndex],
                                 supernet[abaIndex+1], babIndex + 1);
          if (babIndex == -1)
            break;

          // cout << "\t\tfound " << hypernet << endl;
          return true;
        }
      }
    }
  }

  return false;
}

void part1() {
  string line;
  int total = 0;
  while(!cin.eof()) {
    cin >> line;
    if (doesSupportTls(line)){
      cout << "TLS supported: " << line << endl;
      ++total;
    }
  }
  cout << total << endl;
}

void part2() {
  string line;
  int total = 0;
  while(!cin.eof()) {
    cin >> line;
    if (doesSupportSsl(line)){
      cout << "SSL supported: " << line << endl;
      ++total;
    }
  }
  cout << total << endl;
}

int main() {
  part2();
  return EXIT_SUCCESS;
}