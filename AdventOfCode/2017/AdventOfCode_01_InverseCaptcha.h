/*

  Link:         http://adventofcode.com/2017/day/1
 
  Description:  --- Day 1: Inverse Captcha ---

    The night before Christmas, one of Santa's Elves calls you in a panic. "The printer's
    broken! We can't print the Naughty or Nice List!" By the time you make it to sub-basement 17,
    there are only a few minutes until midnight. "We have a big problem," she says; "there must be
    almost fifty bugs in this system, but nothing else can print The List. Stand in this square,
    quick! There's no time to explain; if you can convince them to pay you in stars, you'll be able
    to--" She pulls a lever and the world goes blurry.

    When your eyes can focus again, everything seems a lot more pixelated than before. She must have
    sent you inside the computer! You check the system clock: 25 milliseconds until midnight. With
    that much time, you should be able to collect all fifty stars by December 25th.

    Collect stars by solving puzzles. Two puzzles will be made available on each day millisecond in
    the advent calendar; the second puzzle is unlocked when you complete the first. Each puzzle
    grants one star. Good luck!

    You're standing in a room with "digitization quarantine" written in LEDs along one wall. The
    only door is locked, but it includes a small interface. "Restricted Area - Strictly No Digitized
    Users Allowed."

    It goes on to explain that you may only leave by solving a captcha to prove you're not a human.
    Apparently, you only get one millisecond to solve the captcha: too fast for a normal human, but
    it feels like hours to you.

    The captcha requires you to review a sequence of digits (your puzzle input) and find the sum of
    all digits that match the next digit in the list. The list is circular, so the digit after the
    last digit is the first digit in the list.

    For example:

    1122 produces a sum of 3 (1 + 2) because the first digit (1) matches the second digit and the
    third digit (2) matches the fourth digit.
    1111 produces 4 because each digit (all 1) matches the next.
    1234 produces 0 because no digit matches the next.
    91212129 produces 9 because the only digit that matches the next one is the last digit, 9.
    What is the solution to your captcha?

    Your puzzle answer was 1203.

    The first half of this puzzle is complete! It provides one gold star: *

    --- Part Two ---

    You notice a progress bar that jumps to 50% completion. Apparently, the door isn't yet
    satisfied, but it did emit a star as encouragement. The instructions change:

    Now, instead of considering the next digit, it wants you to consider the digit halfway around
    the circular list. That is, if your list contains 10 items, only include a digit in your sum if
    the digit 10/2 = 5 steps forward matches it. Fortunately, your list has an even number of
    elements.

    For example:

    1212 produces 6: the list contains 4 items, and all four digits match the digit 2 items ahead.
    1221 produces 0, because every comparison is between a 1 and a 2.
    123425 produces 4, because both 2s match each other, but no other digit has a match.
    123123 produces 12.
    12131415 produces 4.

    What is the solution to your new captcha?
    Your puzzle answer was 1146.

  Compiling:    g++ -std=c++11 main.cpp -o main
               
  Programmer:   Michael Duarte.

  Date:         12/14/2017
*/

#ifndef _2017_ADVENTOFCODE_01_INVERRSE_CAPTCHA_H_
#define _2017_ADVENTOFCODE_01_INVERRSE_CAPTCHA_H_

#include <iostream>
#include <string>


namespace aoc2017_01{

  inline void addToSumIfEqual(const std::string &input, uint32_t &sum, int i, int j) {
    if (input[i] == input[j]) {
      sum += input[i] - '0';
    }
  }
  
  uint32_t getSumCheckingNext(const std::string &input) {
    uint32_t sum = 0;
    if (input.length() == 0) {
      return sum;
    }

    int lastElement = input.length() - 1;
    for (int i = 0; i < lastElement; ++i) {
      addToSumIfEqual(input, sum, i, i + 1);
    }
    addToSumIfEqual(input, sum, lastElement, 0);
    return sum;
  }
  
  uint32_t getSumCheckingHalfwayAround(const std::string &input) {
    uint32_t sum = 0;
    if (input.length() == 0) {
      return sum;
    }

    int elements = input.length();
    int halfWay = elements / 2;
    int lastElement = elements - 1;
    for (int i = 0; i < lastElement; ++i) {
      addToSumIfEqual(input, sum, i, (i + halfWay) % elements);
    }
    addToSumIfEqual(input, sum, lastElement, (lastElement + halfWay) % elements);
    return sum;
  }

  void solve(int part = 1) {
    using namespace std;
    string input;
    cin >> input;
    cout << "Input:\t" << input  << endl;
    uint32_t output = part == 1 
        ? getSumCheckingNext(input)
        : getSumCheckingHalfwayAround(input);
    cout << "Output:\t" << output << endl;
  }
};

#endif /* _2017_ADVENTOFCODE_01_INVERRSE_CAPTCHA_H_ */
