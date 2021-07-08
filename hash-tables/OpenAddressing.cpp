#include <iostream>
#include <string>

#include "OpenAddressing.h"

struct PathologicalHashFn
{
  size_t operator()(const std::string &s)
  {
    return s.size();
  }
};

int main()
{
  OpenAddressing<std::string, std::string, PathologicalHashFn> ht;

  std::cout << "insert abc: " << ht.insert("abc", "123") << std::endl;
  {
    auto result = ht.lookup("abc");
    std::cout << "lookup abc: " << (result ? *result : "") << std::endl;
  }
  std::cout << "insert def: " << ht.insert("def", "456") << std::endl;
  {
    auto result = ht.lookup("def");
    std::cout << "lookup def: " << (result ? *result : "") << std::endl;
  }
  {
    auto result = ht.lookup("abc");
    std::cout << "lookup abc: " << (result ? *result : "") << std::endl;
  }

  return 0;
}
