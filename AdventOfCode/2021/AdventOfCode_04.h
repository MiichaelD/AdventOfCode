/*
  Link:         http://adventofcode.com/2020/day/4

  Description:
  Compiling:    g++ -std=c++11 main.cpp -o main

  Programmer:   Michael Duarte.

  Date:         12/04/2020
*/

#ifndef _2021_ADVENTOFCODE_04_H_
#define _2021_ADVENTOFCODE_04_H_

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../util/util.h"

namespace aoc2021_04 {
using namespace std;

static const size_t kBoardSize = 5;

 struct Board {
  vector<vector<int>> matrix;//(5, vector<int>(kBoardSize));
  unordered_set<int> values, seenValues;
  uint32_t sum = 0;
  bool complete = false;
  Board() { }

  bool numberDrawn(int numberDrawn) {
    if (complete) {
      return false; // This board is complete.
    }
    if (values.find(numberDrawn) == values.end()) {
      return false; // Number not on the board.
    }
    seenValues.insert(numberDrawn);
    sum -= numberDrawn;
    return isCompleted();
  }

  void print() {
    cout << "Board w/ " << seenValues.size() << " seen values. Sum: " << sum << endl;
    for (int i = 0; i < kBoardSize; ++i) {
      for (int j = 0; j < kBoardSize; ++j) {
        cout << matrix[i][j] << ", ";
      }
      cout << endl;
    }
    cout << endl;
  }

  bool isCompleted() {
    // This card was processed;
    if (complete) {
      return true;
    }
    // Check rows
    for (int i = 0; i < kBoardSize; ++i) {
      int valuesSeenInRow = 0;
      for (int j = 0; j < kBoardSize; ++j) {
        if (seenValues.find(matrix[i][j]) != seenValues.end()) {
          ++valuesSeenInRow;
        }
      }
      if (valuesSeenInRow == kBoardSize) {
        complete = true;
        return true; // Row is complete.
      }
    }
    // Check columns
    for (int j = 0; j < kBoardSize; ++j) {
      int valuesSeenInCol = 0;
      for (int i = 0; i < kBoardSize; ++i) {
        if (seenValues.find(matrix[i][j]) != seenValues.end()) {
          ++valuesSeenInCol;
        }
      }
      if (valuesSeenInCol == kBoardSize) {
        complete = true;
        return true; // Column is complete.
      }
    }
    return false;
  }

  static Board createBoard() {
    Board board;
    for (int i = 0; i < kBoardSize; ++i) {
      board.matrix.push_back({0,0,0,0,0});
      for (int j = 0; j < kBoardSize; ++j) {
        cin >> board.matrix[i][j];
        board.values.insert(board.matrix[i][j]);
        board.sum += board.matrix[i][j];
      }
    }
    if (board.sum) board.print();
    return board;
  }

};

pair<uint64_t, uint64_t> firstAndLastWinningBoards(
  const string &drawnNumbers, vector<Board> &boards, int part=1) {
  const size_t totalBoards = boards.size();
  pair<uint64_t, uint64_t> finalResult;
  int completedBoards = 0;
  int index = 0;
  while (true) {
    int drawn = util::getNumberRef(drawnNumbers, index);
    cout << "Numero: " << drawn << endl;
    for (Board &board : boards) {
      if (board.numberDrawn(drawn)) {
        // Winning board.
        uint64_t result = board.sum;
        result *= drawn;
        cout << "\tWinner winner chicken dinner: " << board.sum << " * " << drawn;
        cout << " = " << result << endl;
        if (++completedBoards == 1) {
            // First winning board
            finalResult.first = result;
            if (part == 1) {
              return finalResult;
            }
        } else if (completedBoards == totalBoards) {
            // Last winning board
            finalResult.second = result;
            return finalResult;
        }
      }
    }
    ++index; // Skip the current comma.
  }
  return {-1, -1}; // This shouldn't happen
}

void solve(int part = 1) {
  string drawnNumbers;
  getline(cin, drawnNumbers);
  cout << drawnNumbers << endl;
  vector<Board> boards;
  while (true) {
    boards.emplace_back(Board::createBoard());
    if (boards.back().sum == 0) {
      boards.pop_back();
      break;
    }
  }
  cout << "Boards loaded: " << boards.size() << endl;
  cout << "Se va y se corre con ..." << endl;
  util::printPair(firstAndLastWinningBoards(drawnNumbers, boards, part), true);
}

};  // namespace aoc2021_04

#endif /* _2021_ADVENTOFCODE_04_H_ */
