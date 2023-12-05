
#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

#include <functional>
#include <fstream>
#include <sstream>
#include <utility>
#include <math.h>

namespace util {

using namespace std;


struct pair_hash {
  template <class T1, class T2>
  std::size_t operator () (std::pair<T1,T2> const &p) const {
    std::size_t h1 = std::hash<T1>()(p.first);
    std::size_t h2 = std::hash<T2>()(p.second);
    return h1 ^ h2;
  }
};

struct vector_size_hash {
  template <class T1, class T2>
  bool operator () (const std::vector<T1>& a, const std::vector<T2>& b) {
    return a.size() < b.size();
  }
};

template <class T1, class T2>
bool vector_size_predicate(const std::vector<T1>& a, const std::vector<T2>& b) {
    return a.size() > b.size();
}

// Common utils for daily problems
template<class P1, class P2>
inline void printPair(const pair<P1,P2> &p, bool lineBreak=false) {
  cout << "[" << p.first << ", " << p.second << "] ";
  if (lineBreak) {
    cout << endl;
  }
}

template<class T>
inline void printVector(const vector<T> &container, bool lineBreak = false) {
  cout << "[";
  for (const T& element : container) {
    cout << element << ", ";
    if (lineBreak) cout << endl;
  }
  cout << ']';
  if (lineBreak) cout << endl;
}

inline int64_t binaryToDecimal(const string &str) {
  int64_t result = 0;
  size_t lastIndex = str.size() - 1;
  for (int i = lastIndex; i >= 0; --i) {
    int bit = str[i] - '0';
    result += bit * pow(2, lastIndex - i);
  }
  return result;
}

// Gets the number from the string starting at the given index (or 0) - updates index.
template<class T>
T getNumberRef(const string &line, int &indexRef) {
  T accum = 0;
  for (; indexRef < line.size(); ++indexRef) {
    if (isdigit(line[indexRef])) {
      accum *= 10;
      accum += line[indexRef] - '0';
    } else {
      break;
    }
  }
  return accum;
}

// Gets the number from the string starting at the given index (or 0) - updates index.
int getNumberRef(const string &line, int &indexRef) {
  int accum = 0;
  for (; indexRef < line.size(); ++indexRef) {
    if (isdigit(line[indexRef])) {
      accum *= 10;
      accum += line[indexRef] - '0';
    } else {
      break;
    }
  }
  return accum;
}

// Gets the number from the string starting at the given index (or 0).
inline int getNumber(const string &line, int index=0) {
  return getNumberRef(line, index);
}

// Gets number and advances the index reference -- oops, same as above.
inline int getNumberAdvancing(const string &line, int &index) {
  return getNumberRef(line, index);  // Left for compatibility - CLEAN UP!
}


// BINARY - HEXADECIMAL - DECIMAL

// Reads a string of binary data and converts them to big ints/types
string hexToBinary(char hexadecimal) {
  switch(hexadecimal) {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    default: break;
  }
  return "";
}

// Converts ints [0 - 15] to their Hex value as char.
char decToHex(int c){
  if (c < 10) {
    return c + '0';
  } else {
    switch (c) {
      case 10: return 'A';
      case 11: return 'B';
      case 12: return 'C';
      case 13: return 'D';
      case 14: return 'E';
      case 15: return 'F';
      default: break;
    }
  }
  return '-';
}

string hexToBinary(const string &input) {
  stringstream ss;
  for (char c : input) {
    ss << hexToBinary(c);
  }
  return ss.str();
}

char binToHex(const string &binary, int start = 0, int length = 4) {
  int times = 0;
  char result = 0; 
  for (int i = start + length - 1; i >= start; --i, ++times) {
    if (binary[i] == '1') {
      result += (1 << times);
    }
  }
  return result;
}
template<class T>
T binToDec(const string &binary, int start = 0, int length = 4) {
  int times = 0;
  T shift = 1;
  T result = 0; 
  for (int i = start + length - 1; i >= start; --i, ++times) {
    if (binary[i] == '1') {
      result += (shift << times);  // Be careful when defining the type of bit to be shifted
    }
  }
  return result;
}



// Daily template file generator code
template<typename T>
class FileWrapper {
public:
  FileWrapper(const string &filename) {
    cout << "Opening file: " << filename << endl;
    file.open(filename);
  }

  ~FileWrapper() { if (file.is_open()) { file.close(); } }

  bool processInputFile(
      const function<bool(const string &)> &func, const string &day) {
    if (!file.is_open()) {
      cout << "File is not opened for reading" << endl;
      return false;
    }
    string line;
    while (getline(file, line)) {
      size_t index = 0;
      while(true) {
        index = line.find("XX", index);
        if (index == string::npos) break;
        line.replace(index, 2, day);
        index += 2;
      }
      if (!func(line)) {
        return false;
      }
    }
    return true;
  }

  bool writeLine(const string &input) {
    if (!file.is_open()) {
      cout << "File is not open for writing" << endl;
      return false;
    }
    file << input << endl;
    return true;
  }

private:
  T file;
};

int generateFileFromTemplate(const string &templateFilename, const string &day) {
  stringstream newFilename;
  for (int i = 0; i < templateFilename.size() - 4; ++i) {
    newFilename << templateFilename[i];
  }
  newFilename <<  '_' << day;
  
  FileWrapper<ifstream> templateFile(templateFilename);
  FileWrapper<ofstream> inFile(newFilename.str() + "_input.txt");
  FileWrapper<ofstream> outFile(newFilename.str() + ".h");
  return templateFile.processInputFile(
    ([&outFile](const string &input){return outFile.writeLine(input);}),
    day) ? EXIT_SUCCESS : EXIT_FAILURE;
}
}

#endif // _UTIL_UTIL_H_