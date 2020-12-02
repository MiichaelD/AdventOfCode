
#include<utility>

namespace util {

using namespace std;

template<class T>
inline void printPair(const pair<T,T> &p, bool endl=false) {
  cout << "[" << p.first << ", " << p.second << "] ";
  if (endl) {
    cout << endl;
  }
}

}