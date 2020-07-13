#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Burst {
  int start;
  int end;
  int qps;
};

int main() {
  Burst data[] = {
    {10, 20, 1},
    {15, 25, 3},
    { 0,  9, 4},
    { 8, 29, 3}
  };

  vector<pair<int, int> > start_and_end;
  for (int i = 0; i < 4; i++) {
    start_and_end.push_back(make_pair(data[i].start, data[i].qps));
    start_and_end.push_back(make_pair(data[i].end, -data[i].qps));
  }

  sort(start_and_end.begin(), start_and_end.end());

  int sum = 0;
  int maxsum = 0;
  for (vector<pair<int,int> >::iterator itr = start_and_end.begin(); itr != start_and_end.end(); itr++) {
    sum += itr->second;
    if (sum > maxsum) {
      maxsum = sum;
    }
  }

  cout << maxsum << endl;

  return 0;
}
