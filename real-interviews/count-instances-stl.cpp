#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
  int array[] = {0, 1, 2, 3, 3, 3, 3, 3, 4, 5};

  cout << "Testing recursive binary search..." << endl;
  for (int i = 0; i < 10; i++) {
        std::pair<int*, int*> result = equal_range(array, array + 9, i);
        size_t count = distance(result.first, result.second);
    cout << " " << i << ": " << count << " instances" << endl;
  }

  return 0;
}
