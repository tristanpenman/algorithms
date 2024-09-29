#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

//#define TRACE

#include "Bitstream.h"
#include "Huffman.h"

template<typename T>
FreqTable<T> build_frequency_table(std::ifstream &ifs)
{
  FreqTable<T> ft;

  while (!ifs.eof()) {
    T ch;
    ifs.get(ch);
    ft.increment(ch);
  }

  return ft;
}

template<typename T>
std::shared_ptr<SplitNode<T>> build_huffman_tree(const FreqTable<T> &ft)
{
  PQ<T> pq;

  for (size_t i = 0; i < ft.size(); i++) {
    auto node = std::make_shared<ValueNode<T>>();
    node->value = i;
    node->weight = ft.count(i);
    pq.push(node);
  }

  while (pq.size() > 1) {
    // pop two lowest weight nodes in priority queue
    auto a = pq.top();
    pq.pop();
    auto b = pq.top();
    pq.pop();

    // combine them under one split node
    auto node = std::make_shared<SplitNode<T>>();
    node->left = b;
    node->right = a;
    node->weight = a->weight + b->weight;
    pq.push(node);
  }

  // should only have one node left
  auto node = pq.top();
  pq.pop();

  // it will be a split node...
  // TODO: unless there was only ever one value
  auto split = std::dynamic_pointer_cast<SplitNode<T>>(node);
  assert(split);

  return split;
}

template<typename T>
void encode_huffman_tree(const std::shared_ptr<SplitNode<T>> &node, BitstreamWriter &writer)
{
  auto lsplit = std::dynamic_pointer_cast<SplitNode<T>>(node->left);
  if (lsplit) {
    writer.write_bit(0);
    encode_huffman_tree(lsplit, writer);
  } else {
    writer.write_bit(1);
    auto lvalue = std::dynamic_pointer_cast<ValueNode<T>>(node->left);
    writer.write_value(lvalue->value);
  }

  auto rsplit = std::dynamic_pointer_cast<SplitNode<T>>(node->right);
  if (rsplit) {
    writer.write_bit(0);
    encode_huffman_tree(rsplit, writer);
  } else {
    writer.write_bit(1);
    auto rvalue = std::dynamic_pointer_cast<ValueNode<T>>(node->right);
    writer.write_value(rvalue->value);
  }
}

template<typename T>
void compress_data(const Dict<T> &dict, std::istream &ifs, BitstreamWriter &writer, uint32_t len)
{
  writer.write_value<uint32_t>(len);

  // encode
  size_t read = 0;
  while (!ifs.eof()) {
    T ch;
    if (ifs.get(ch)) {
        read++;
        auto code = dict.lookup(ch);
#ifdef TRACE
        std::cout << code_to_string(code) << " (0x" << std::hex << int(static_cast<typename std::make_unsigned<T>::type>(ch)) << std::dec << ")" << std::endl;
#endif
        for (auto bit: code) {
            if (!writer.write_bit(bit)) {
                throw std::runtime_error("failed to write bit");
            }
        }
    }
  }

  // read stats
  std::cout << "Bytes read: " << read << std::endl;

  writer.flush();

  // write stats
  std::cout << "Bits written: " << writer.bits_written() << std::endl;
  std::cout << "Bits flushed: " << writer.bits_flushed() << std::endl;
}

void usage(char *arg0)
{
  std::cout << arg0 << " <input> [output]" << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    usage(argv[0]);
    return 1;
  }

  std::ifstream ifs(argv[1], std::ios::binary);
  if (ifs.bad()) {
    std::cerr << "Failed to open input file: " << argv[1] << std::endl;
    return 1;
  }

  std::ofstream ofs(argv[2], std::ios::binary);
  if (ofs.bad()) {
    std::cerr << "Failed to open output file: " << argv[2] << std::endl;
    return 1;
  }

  BitstreamWriter writer(ofs);
  writer.write_string(MAGIC_BYTES);

  std::cout << "Building frequency table..." << std::endl;
  auto ft = build_frequency_table<char>(ifs);
#ifdef TRACE
  print_table(ft);
#endif

  std::cout << "Building huffman tree..." << std::endl;
  auto ht = build_huffman_tree(ft);
#ifdef TRACE
  print_tree(ht, 0);
  std::cout << std::endl;
#endif

  std::cout << "Building dictionary..." << std::endl;
  Dict<char> dict;
  build_dictionary(ht, std::vector<bool>(), dict);

  std::cout << "Encoding huffman tree..." << std::endl;
  writer.write_bit(0);
  encode_huffman_tree(ht, writer);

  // store file length
  ifs.clear();
  ifs.seekg(0, std::ios::end);
  uint32_t len = ifs.tellg();

  // reset to beginning of input
  ifs.clear();
  ifs.seekg(0, std::ios::beg);

  std::cout << "Compressing data..." << std::endl;
  compress_data(dict, ifs, writer, len);

  return 0;
}
