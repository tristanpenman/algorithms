#include <iostream>
#include <vector>

#include "BubbleDownHeap.h"

int main(int argc, char **argv) {
  std::vector<std::string> args;
  args.reserve(argc);
  for (int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }
  BubbleDownHeap<std::string> heap(args.data(), argc);
  while (heap.getSize() > 0) {
    auto val = heap.pop();
    std::cout << val << std::endl;
  }
  return 0;
}
