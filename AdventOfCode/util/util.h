
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