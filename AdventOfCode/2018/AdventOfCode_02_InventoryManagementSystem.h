/*
  Link:         http://adventofcode.com/2018/day/2
 
  Description:  --- Day 2: Inventory Management System ---
    You stop falling through time, catch your breath, and check the screen on the device. "Destination reached. Current Year: 1518. Current Location: North Pole Utility Closet 83N10." You made it! Now, to find those anomalies.

    Outside the utility closet, you hear footsteps and a voice. "...I'm not sure either. But now that so many people have chimneys, maybe he could sneak in that way?" Another voice responds, "Actually, we've been working on a new kind of suit that would let him fit through tight spaces like that. But, I heard that a few days ago, they lost the prototype fabric, the design plans, everything! Nobody on the team can even seem to remember important details of the project!"

    "Wouldn't they have had enough fabric to fill several boxes in the warehouse? They'd be stored together, so the box IDs should be similar. Too bad it would take forever to search the warehouse for two similar box IDs..." They walk too far away to hear any more.

    Late at night, you sneak to the warehouse - who knows what kinds of paradoxes you could cause if you were discovered - and use your fancy wrist device to quickly scan every box and produce a list of the likely candidates (your puzzle input).

    To make sure you didn't miss any, you scan the likely candidate boxes again, counting the number that have an ID containing exactly two of any letter and then separately counting those with exactly three of any letter. You can multiply those two counts together to get a rudimentary checksum and compare it to what your device predicts.

    For example, if you see the following box IDs:

    abcdef contains no letters that appear exactly two or three times.
    bababc contains two a and three b, so it counts for both.
    abbcde contains two b, but no letter appears exactly three times.
    abcccd contains three c, but no letter appears exactly two times.
    aabcdd contains two a and two d, but it only counts once.
    abcdee contains two e.
    ababab contains three a and three b, but it only counts once.
    Of these box IDs, four of them contain a letter which appears exactly twice, and three of them contain a letter which appears exactly three times. Multiplying these together produces a checksum of 4 * 3 = 12.

    What is the checksum for your list of box IDs?

    Your puzzle answer was 7134.

    The first half of this puzzle is complete! It provides one gold star: *

    --- Part Two ---
    Confident that your list of box IDs is complete, you're ready to find the boxes full of prototype fabric.

    The boxes will have IDs which differ by exactly one character at the same position in both strings. For example, given the following box IDs:

    abcde
    fghij
    klmno
    pqrst
    fguij
    axcye
    wvxyz
    The IDs abcde and axcye are close, but they differ by two characters (the second and fourth). However, the IDs fghij and fguij differ by exactly one character, the third (h and u). Those must be the correct boxes.

    What letters are common between the two correct box IDs? (In the example above, this is found by removing the differing character from either ID, producing fgij.)

  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/16/2018
*/

#ifndef _2018_ADVENTOFCODE_02_INVENTORY_MANAGEMENT_SYSTEM_H_
#define _2018_ADVENTOFCODE_02_INVENTORY_MANAGEMENT_SYSTEM_H_

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace aoc2018_02 {

  void solve1() {    
    using namespace std;
    int dos = 0;
    int tres = 0;
    int k = 0;
    while(!cin.eof()) {
        int arr[26];
        for (int i = 0 ; i < 26; ++i) {
            arr[i] = 0;
        }
        string input;
        cin >> input;
        for (char c : input) {
            ++arr[c - 'a'];
        }

        bool seen2 = false;
        bool seen3 = false;
        for (int i = 0 ; i < 26; ++i) {
            if (arr[i] == 2 && !seen2) {
                seen2 = true;
                ++dos;
            }
            if (arr[i] == 3 && !seen3) {
                seen3 = true;
                ++tres;
            }
        }
        cout << "\t " << k++ << ": dos " << dos << ", tres " << tres << endl; 
    }
    cout << (dos * tres) << endl;
  }

  void solve2() {    
    using namespace std;
    vector<string> ids;
    while(!cin.eof()) {
        string input;
        cin >> input;
        ids.emplace_back(move(input));
    }
    cout << "Palabras: " << ids.size() << ". Ejemplo: " << ids[0] << ". Longitud: ";
    cout << ids[0].length() << endl << endl;

    for (int i = 0; i < ids.size(); ++i) {
        for (int j = i+1; j < ids.size(); ++j) {
            bool oneDiferent = false;
            int diffChar = -1;
            for (int k = 0; k < ids[i].length(); ++k) {
                if (ids[i][k] != ids[j][k]) {
                    if (!oneDiferent) {
                        oneDiferent = true;
                        diffChar = k;
                        continue;
                    }
                    oneDiferent = false;
                    break;
                }
            }
            if (oneDiferent) {
                for (int k = 0; k < ids[i].length(); ++k) {
                    if (ids[i][k] == ids[j][k]) {
                        cout << ids[i][k];
                    }
                }
                cout << endl;
                return;
            }
        }
    }
    cout << "<NOT_FOUND>" << endl;
  }

  void solve(int part = 1) {
    using namespace std;
    if (part == 1) {
        solve1();
    } else {
        solve2();
    }
  }

};

#endif /* _2018_ADVENTOFCODE_02_INVENTORY_MANAGEMENT_SYSTEM_H_ */
