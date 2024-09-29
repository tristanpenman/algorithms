#include <cstring>
#include <fstream>
#include <iostream>

//#define TRACE

#include "Bitstream.h"
#include "Huffman.h"

template<typename T>
std::shared_ptr<SplitNode<T>> decode_huffman_tree(BitstreamReader &reader)
{
  auto node = std::make_shared<SplitNode<T>>();

  {
    // left
    int bit;
    if (!reader.read_bit(bit)) {
      throw std::runtime_error("failed to read left bit");
    }

    if (bit == 1) {
      // value
      T value;
      if (!reader.read_value(value)) {
        throw std::runtime_error("failed to read left value");
      }
      auto left = std::make_shared<ValueNode<T>>();
      left->value = value;
      left->weight = 0;
      node->left = left;
    } else {
      // split
      node->left = decode_huffman_tree<T>(reader);
    }
  }

  {
    // left
    int bit;
    if (!reader.read_bit(bit)) {
      throw std::runtime_error("failed to read right bit");
    }

    if (bit == 1) {
      // value
      T value;
      if (!reader.read_value(value)) {
        throw std::runtime_error("failed to read right value");
      }
      auto right = std::make_shared<ValueNode<T>>();
      right->value = value;
      right->weight = 0;
      node->right = right;
    } else {
      // split
      node->right = decode_huffman_tree<T>(reader);
    }
  }

  node->weight = 0;

  return node;
}

template<typename T>
void decompress_data(const Dict<T> &dict, BitstreamReader &reader, std::ostream &ofs, uint32_t len)
{
  std::vector<bool> codeword;
  codeword.reserve(16);
  size_t output_size = 0;

  int bit;
  while (output_size != len) {
    if (!reader.read_bit(bit)) {
      std::cout << "eos" << std::endl;
      return;
    }

    // Add bit to codeword
    codeword.push_back(bit);

    T value;
    if (dict.lookup(codeword, value)) {
      output_size++;
#ifdef TRACE
      std::cout << code_to_string(codeword) << " (0x" << std::hex << int(static_cast<typename std::make_unsigned<T>::type>(value)) << std::dec << ")" << std::endl;
#endif
      ofs.write(&value, sizeof(T));
      codeword.clear();
    }
  }

  std::cout << "Output " << output_size << " bytes" << std::endl;

  ofs.flush();
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

  BitstreamReader reader(ifs);
  uint32_t bytes = 0;
  if (!reader.read_value(bytes)) {
    if (memcmp(reinterpret_cast<char *>(bytes), MAGIC_BYTES, 4) != 0) {
      std::cerr << "Magic bytes are invalid" << std::endl;
      return 1;
    }
  }

  std::cout << "Decoding huffman tree..." << std::endl;
  int bit;
  if (!reader.read_bit(bit)) {
    std::cerr << "Failed to read first bit of huffman tree" << std::endl;
    return 1;
  }
  if (bit == 1) {
    std::cerr << "Expected first bit to be 0" << std::endl;
    return 1;
  }

  auto ht = decode_huffman_tree<char>(reader);
#ifdef TRACE
  print_tree(ht, 0);
#endif

  std::cout << "Building dictionary..." << std::endl;
  Dict<char> dict;
  build_dictionary(ht, std::vector<bool>(), dict);

  uint32_t len;
  if (!reader.read_value<uint32_t>(len)) {
    std::cerr << "Failed to read length" << std::endl;
    return 1;
  }

  std::cout << "Decompressing data..." << std::endl;
  decompress_data(dict, reader, ofs, len);

  return 0;
}
