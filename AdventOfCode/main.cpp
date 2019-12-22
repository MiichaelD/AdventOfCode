
#include "2019/AdventOfCode_01.h"
#include "2019/AdventOfCode_02.h"
#include "2019/AdventOfCode_03.h"
#include "2019/AdventOfCode_04.h"
#include "2019/AdventOfCode_05.h"
#include "2019/AdventOfCode_06.h"
#include "2019/AdventOfCode_07.h"
#include "2019/AdventOfCode_08.h"
#include "2019/AdventOfCode_09.h"
#include "2019/AdventOfCode_10.h"
#include "2019/AdventOfCode_11.h"
#include "2019/AdventOfCode_12.h"
#include "2019/AdventOfCode_13.h"
#include "2019/AdventOfCode_14.h"
#include "2019/AdventOfCode_15.h"
#include "2019/AdventOfCode_16.h"
#include "2019/AdventOfCode_17.h"
#include "2019/AdventOfCode_18.h"
#include "2019/AdventOfCode_19.h"
#include "2019/AdventOfCode_20.h"
#include "2019/AdventOfCode_21.h"
#include "2019/AdventOfCode_22.h"

// Compiling:    g++ -std=c++11 main.cpp -o main; ./main [<part>]
int main(int argc, char *argv[]) {
  int part = 2;
  if (argc > 1) {
    part = atoi(argv[argc-1]);
  }
  std::cout << "Solving part: " << part << std::endl;
  aoc2019_22::solve(part);
  return EXIT_SUCCESS;
}