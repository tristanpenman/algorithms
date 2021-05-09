#pragma once

#include <cmath>
#include <utility>

#include "Quicksort.h"
#include "Util.h"

namespace ClosestPair {

template<typename T>
float findDistance(T& i, T& j) {
  int x = i.x - j.x;
  int y = i.y - j.y;

  return sqrt(x*x + y*y);
}

template<typename T>
bool lessThanOrEqualX(T a, T b) {
  return a.x <= b.x;
}

template<typename T>
bool lessThanOrEqualY(T a, T b) {
  return a.y <= b.y;
}

//
// Find the closest two points in an array a[] between
// indices at left and right
//
template<typename T>
std::pair<int, int> findClosestPair(T* a, int left, int right) {
  Quicksort::sort(a, 0, 6, lessThanOrEqualX);

  int n = right - left + 1;

  if (n == 2) {
    // When there are two elements, the work is already done
    return std::pair<int, int>(left, left + 1);
  }

  if (n == 3) {
    // When there are three elements use brute force
    float d1 = findDistance(a[left], a[left + 1]);
    float d2 = findDistance(a[left], a[left + 2]);
    float d3 = findDistance(a[left + 1], a[left + 2]);

    if (d1 <= d2 && d1 <= d3) {
      return std::pair<int, int>(left, left + 1);
    } else if (d2 <= d1 && d2 <= d3) {
      return std::pair<int, int>(left, left + 2);
    }

    return std::pair<int, int>(left + 1, left + 2);
  }

  int middle = left + n / 2;

  // Calculate shortest pair in each subset
  std::pair<int, int> r1 = findClosestPair(a, left, middle - 1);
  std::pair<int, int> r2 = findClosestPair(a, middle, right);

  // Calculate distance between each pair
  float d1 = findDistance(a[r1.first], a[r1.second]);
  float d2 = findDistance(a[r2.first], a[r2.second]);

  // Store minimum distance
  float min;
  std::pair<int, int>& shortest = r1;

  if (d1 <= d2) {
    min = d1;
    shortest = r1;
  } else {
    min = d2;
    shortest = r2;
  }

  Quicksort::sort(a, left, right, lessThanOrEqualY);

  for (int i = left; i <= right; i++) {
    int k = i + 1;

    if ((a[i].x < a[middle].x + min) || (a[i].x > a[middle].x - min)) {
      // Point is within 'min' units of dividing line

      for (int j = left; j <= right; j++) {
        if ((i != j) && ((a[j].x < a[middle].x + min) || (a[j].x > a[middle].x - min))) {
          float f3 = findDistance(a[i], a[j]);

          if (f3 < min) {
            min = f3;
            shortest = std::pair<int, int>(i, j);
          }
        }
      }
    }
  }

  return shortest;
}

} // end namespace ClosestPair
