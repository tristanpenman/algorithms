#include <iostream>
#include <string>

#include "OpenHashTable.h"

struct PathologicalHash {
  size_t operator()(const std::string &s) {
    return s.size();
  }
};

int main() {
  OpenHashTable<std::string, std::string, PathologicalHash> hashTable;

  std::cout << "insert abc: " << hashTable.insert("abc", "123") << std::endl;
  {
    auto result = hashTable.lookup("abc");
    std::cout << "lookup abc: " << (result ? *result : "") << std::endl;
  }
  std::cout << "insert def: " << hashTable.insert("def", "456") << std::endl;
  {
    auto result = hashTable.lookup("def");
    std::cout << "lookup def: " << (result ? *result : "") << std::endl;
  }
  {
    auto result = hashTable.lookup("abc");
    std::cout << "lookup abc: " << (result ? *result : "") << std::endl;
  }

  return 0;
}
