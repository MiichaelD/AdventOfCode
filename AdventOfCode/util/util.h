
#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

#include<utility>

namespace util {

using namespace std;

template<class T>
inline void printPair(const pair<T,T> &p, bool lineBreak=false) {
  cout << "[" << p.first << ", " << p.second << "] ";
  if (lineBreak) {
    cout << endl;
  }
}

}

#endif // _UTIL_UTIL_H_