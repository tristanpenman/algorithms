#pragma once

namespace Quicksort {

//
// Swap two elements in an array a[] at a[left] and a[right]
//
template<typename T>
void swap(T* a, int left, int right) {
  T t = a[left];
  a[left] = a[right];
  a[right] = t;
}

//
// Partition an array a[] using the value at a[pivotIndex]
//
template<typename T>
int partition(T* a, int left, int right, int pivotIndex, bool(*lessThanOrEqual)(T, T)) {
  // Copy pivot value
  T pivotValue = a[pivotIndex];

  // Move pivot point to end
  swap(a, pivotIndex, right);

  int storeIndex = left;

  // Check all elements of array except for pivot value
  for (int i = left; i < right; ++i) {
    // Elements smaller than the pivot value are placed to the left
    if (lessThanOrEqual(a[i], pivotValue)) {
      // Place element in sorted part of the array
      swap(a, i, storeIndex);
      storeIndex++;
    }
  }

  swap(a, storeIndex, right);

  return storeIndex;
}

//
// Quick sort an array a[] between two indices, left and right
//
template<typename T>
void sort(T* a, int left, int right, bool(*lessThanOrEqual)(T, T)) {
  if (right > left) {
    int pivotNewIndex = partition(a, left, right, left, lessThanOrEqual);
    sort(a, left, pivotNewIndex - 1, lessThanOrEqual);
    sort(a, pivotNewIndex + 1, right, lessThanOrEqual);
  }
}

} // end namespace Quicksort
