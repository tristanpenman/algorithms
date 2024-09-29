#pragma once

#include <array>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

#define MAGIC_BYTES "TPHE"

template<typename T>
class FreqTable
{
  using U = typename std::make_unsigned<T>::type;

  static const size_t szBytes = sizeof(U);
  static const size_t szBits = szBytes * 8;
  static const size_t szTable = 1 << szBits;

  std::array<uint32_t, szTable> counts = {};

public:
  uint32_t count(T t) const
  {
    return counts[static_cast<U>(t)];
  }

  void increment(T t)
  {
    counts[static_cast<U>(t)]++;
  }

  size_t size() const
  {
    return szTable;
  }
};

template<typename T>
class Dict
{
  using U = typename std::make_unsigned<T>::type;

public:
  void add(std::vector<bool> prefix, T value)
  {
    lut.insert(std::make_pair(value, prefix));
    rev_lut.insert(std::make_pair(prefix, value));
  }

  std::vector<bool> lookup(const T& value) const
  {
    auto itr = lut.find(value);
    if (itr == lut.end()) {
      throw std::runtime_error("failed to find value");
    }

    return itr->second;
  }

  bool lookup(const std::vector<bool>& codeword, T &t) const
  {
    auto itr = rev_lut.find(codeword);
    if (itr == rev_lut.end()) {
      return false;
    }

    t = itr->second;
    return true;
  }

  std::unordered_map<T, std::vector<bool>> lut;
  std::unordered_map<std::vector<bool>, T> rev_lut;
};

template<typename T>
struct Node
{
  virtual ~Node() {}

  uint32_t weight;
};

template<typename T>
struct SplitNode: public Node<T>
{
  std::shared_ptr<Node<T>> left;
  std::shared_ptr<Node<T>> right;
};

template<typename T>
struct ValueNode: public Node<T>
{
  T value;
};

template<typename T>
struct CompareNodes
{
  constexpr bool operator()(
    const std::shared_ptr<Node<T>>& lhs,
    const std::shared_ptr<Node<T>>& rhs
  ) const {
    return lhs->weight > rhs->weight;
  }
};

template<typename T>
struct PQ: public std::priority_queue<
    std::shared_ptr<Node<T>>,
    std::vector<std::shared_ptr<Node<T>>>,
    CompareNodes<T>
  >
{

};

// HELPERS

template<typename T>
void build_dictionary(const std::shared_ptr<SplitNode<T>> &node, const std::vector<bool> &prefix, Dict<T> &dict)
{
    auto lprefix = prefix;
    lprefix.push_back(false);

    auto lsplit = std::dynamic_pointer_cast<SplitNode<T>>(node->left);
    if (lsplit) {
        build_dictionary(lsplit, lprefix, dict);
    } else {
        auto value = std::dynamic_pointer_cast<ValueNode<T>>(node->left);
        dict.add(lprefix, value->value);
    }

    auto rprefix = prefix;
    rprefix.push_back(true);

    auto rsplit = std::dynamic_pointer_cast<SplitNode<T>>(node->right);
    if (rsplit) {
        build_dictionary(rsplit, rprefix, dict);
    } else {
        auto value = std::dynamic_pointer_cast<ValueNode<T>>(node->right);
        dict.add(rprefix, value->value);
    }
}

// DEBUGGING

#ifdef TRACE

#include <iostream>
#include <string>
#include <vector>

template<typename T>
void print_table(const FreqTable<T> &ft)
{
  for (size_t i = 0; i < ft.size(); i++) {
    std::cout << i << ": " << ft.count(i) << std::endl;
  }
}

template<typename T>
void print_node(std::shared_ptr<Node<T>> &node, const char *prefix, int depth)
{
  using U = typename std::make_unsigned<T>::type;

  for (int i = 0; i < depth + 1; i++) {
    std::cout << "  ";
  }

  std::cout << prefix << ": ";
  auto split = std::dynamic_pointer_cast<SplitNode<T>>(node);
  if (split) {
    print_tree(split, depth + 1);
    return;
  }

  auto value = std::dynamic_pointer_cast<ValueNode<T>>(node);
  if (value) {
    std::cout << value->weight << " ";
    std::cout << "(0x" << std::hex << int(static_cast<U>(value->value)) << std::dec << ")";
  }
}

template<typename T>
void print_tree(std::shared_ptr<SplitNode<T>> &node, int depth)
{
  std::cout << node->weight << " (+)" << std::endl;
  print_node(node->left, "left", depth);
  std::cout << std::endl;
  print_node(node->right, "right", depth);
}

std::string code_to_string(const std::vector<bool>& v)
{
  std::string out("0b");
  out.reserve(v.size() + 2);
  for (bool b : v) {
    out += b ? '1' : '0';
  }

  return out;
}

#endif
