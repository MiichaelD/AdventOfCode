
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
template<class T>
inline void printPair(const pair<T,T> &p, bool lineBreak=false) {
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
int getNumber(const string &line, int index=0) {
  return getNumberRef(line, index);
}

// Gets number and advances the index reference.
int getNumberAdvancing(const string &line, int &index) {
  int accum = 0;
  for (; index < line.size(); ++index) {
    if (isdigit(line[index])) {
      accum *= 10;
      accum += line[index] - '0';
    } else {
      break;
    }
  }
  return accum;
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