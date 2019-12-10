/*
  Link:         http://adventofcode.com/2019/day/8
  Compiling:    g++ -std=c++11 main.cpp -o main
  Programmer:   Michael Duarte.
  Date:         12/08/2019
*/

#ifndef _2019_ADVENTOFCODE_08_H_
#define _2019_ADVENTOFCODE_08_H_

#include <algorithm> // std::sort
#include <iomanip>      // std::setprecision
#include <iostream> // std::cin, std::cout, std::fixed
#include <ctype.h>  // char related functions
#include <cmath>   // pow, floor, sqrt
#include <deque>
#include <limits> // std::numeric_limits<T>::max
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc2019_08 {  
  using namespace std;

  const int WIDTH = 25;
  const int HEIGHT = 6;

  const int BLACK = 0;
  const int WHITE = 1;
  const int TRANSPARENT= 2;

  void printImageData(const vector<vector<vector<int>>> &imageData) {
    for (int l = 0; l < imageData.size(); ++l) {
      cout << "Layer " << l+1 << ":" << endl;
      for (int h = 0; h < imageData[l].size(); ++h) {
        for (int w = 0; w < imageData[l][h].size(); ++w) {
          cout << " " << imageData[l][h][w];
        }
        cout << endl;
      }
      cout << endl;
    }  
  }

  int getOnesTimesTwosInLayerWithFewerZeros(const vector<vector<vector<int>>> &imageData) {
    int result = -1;
    int minZeroes = INT_MAX;
    for (int l = 0; l < imageData.size(); ++l) {
      int numberCounters[10] = {0,0,0,0,0,0,0,0,0,0};
      for (int h = 0; h < imageData[l].size(); ++h) {
        for (int w = 0; w < imageData[l][h].size(); ++w) {
          int num = imageData[l][h][w];
          ++numberCounters[num];
        }
      }
      if (numberCounters[0] < minZeroes) {
        minZeroes = numberCounters[0];
        result = numberCounters[1] * numberCounters[2];
        if (minZeroes == 0) {
          break;  // No need to keep iterating.
        }
      }
    }
    return result;
  }

  vector<vector<vector<int>>> decode(const vector<vector<vector<int>>> &imageData) {   
    vector<vector<vector<int>>> result;
    result.push_back(vector<vector<int>>(HEIGHT, vector<int>(WIDTH)));
    for (int h = 0; h < imageData[0].size(); ++h) {
      for (int w = 0; w < imageData[0][0].size(); ++w) {
        for (int l = 0; l < imageData.size(); ++l) {
          int num = imageData[l][h][w];
          if (num != TRANSPARENT) {
            result.back()[h][w] = num;
            cout << (num == WHITE ?"\033[1;31m\033[1;47m 8\033[0m":"\033[1;30m _\033[0m");
            break;
          }
        }
      }
      cout << endl;
    }
    return result; 
    
  }

  void solve(int part = 1) {
    using namespace std; 
    string input;
    cin >> input;
    vector<vector<vector<int>>> imageData;
    int pixelsPerLayer = WIDTH * HEIGHT;
    int layers = input.length() / (pixelsPerLayer);
    for (int l = 0; l < layers; ++l) {
      cout << "Layer " << l << ": " << endl;
      imageData.emplace_back();
      for (int h = 0; h < HEIGHT; ++h) {
        imageData.back().emplace_back();
        for (int w = 0; w < WIDTH; ++w) {
          int c = input[(l * pixelsPerLayer) + (h * WIDTH) + w] - '0';
          cout << c << ",";
          imageData.back().back().push_back(c);
        }
        cout << endl;
      }
      cout << endl;
    }
    if (part == 1) {
      cout << getOnesTimesTwosInLayerWithFewerZeros(imageData) << endl;
    } else {
      vector<vector<vector<int>>> decoded = decode(imageData);
      // printImageData(decoded);
    }
  }
};

#endif /* _2019_ADVENTOFCODE_08_H_ */