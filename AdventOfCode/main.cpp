
#include "2020/AdventOfCode_01.h"
#include "2020/AdventOfCode_02.h"
#include "2020/AdventOfCode_03.h"
#include "2020/AdventOfCode_04.h"
#include "2020/AdventOfCode_05.h"
#include "2020/AdventOfCode_06.h"
#include "2020/AdventOfCode_07.h"
#include "2020/AdventOfCode_08.h"
#include "2020/AdventOfCode_09.h"
#include "2020/AdventOfCode_10.h"
#include "2020/AdventOfCode_11.h"
#include "2020/AdventOfCode_12.h"
#include "2020/AdventOfCode_13.h"
#include "2020/AdventOfCode_14.h"
// #include "2020/AdventOfCode_15.h"
// #include "2020/AdventOfCode_16.h"
// #include "2020/AdventOfCode_17.h"
// #include "2020/AdventOfCode_18.h"
// #include "2020/AdventOfCode_19.h"
// #include "2020/AdventOfCode_20.h"
// #include "2020/AdventOfCode_21.h"
// #include "2020/AdventOfCode_22.h"
// #include "2020/AdventOfCode_23.h"
// #include "2020/AdventOfCode_24.h"
// #include "2020/AdventOfCode_25.h"

// Compiling:    g++ -std=c++11 main.cpp -o main; ./main [<part>|<generate <day>>]
// Getting a day input:
// curl --cookie "session=<session_str>" https://adventofcode.com/2020/day/<day>/input
static constexpr char kGenerate[] = "generate";
static constexpr char kTemplateFilename[] = "2020/AdventOfCode.txt";

int main(int argc, char *argv[]) {
  using namespace std;
  int part = 1;
  if (argc > 1) {
    const char *p1 = argv[1];
    if (isdigit(p1[0])) {
      part = atoi(p1);
    } else if (strcmp(p1,kGenerate) == 0) {
      if (argc != 3) {
        cout << "Please specify day to generate (with leading 0)" << endl;
        return EXIT_FAILURE;
      }
      return util::generateFileFromTemplate(kTemplateFilename, argv[2]);
    }
  }
  cout << "Solving part: " << part << endl;
  aoc2020_14::solve(part);
  return EXIT_SUCCESS;
}