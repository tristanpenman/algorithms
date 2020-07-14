#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <sstream>

#include "Point.h"

namespace Util {

// Print an array to stdout
template<typename T>
void printArray(T* a, unsigned int left, unsigned int right) {
  for (; left <= right; ++left) {
    std::cout << a[left];
    if (left < right) {
      std::cout << ", ";
    }
  }

  std::cout << std::endl;
}

// Allow an array to be written to an output stream
template<typename T>
std::string streamArray(T* a, unsigned int left, unsigned int right) {
  std::stringstream s;
  for (; left <= right; ++left) {
    s << a[left];
    if (left < right) {
      s << ", ";
    }
  }

  return s.str();
}

}

// Allow a TPPoint object to be written to an output stream
inline std::ostream& operator<<(std::ostream &o, const Point &p) {
  o << "[" << p.x << ", " << p.y << "]";
  return o;
}

#endif
