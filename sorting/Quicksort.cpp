#include <iostream>

#include "Quicksort.h"
#include "Util.h"

using namespace std;

bool lessThanOrEqual(int a, int b) {
  return a <= b;
}

int main(int argc, char* argv[]) {
  int a[] = {0, 20, 1, 3, 3, 9, 21};
  std::cout << "Input array: " << Util::streamArray(a, 0, 6) << std::endl;
  Quicksort::sort(a, 0, 6, lessThanOrEqual);
  std::cout << "Output array: " << Util::streamArray(a, 0, 6) << std::endl;
  return 0;
}
