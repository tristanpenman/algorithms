#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>

#include "Bitstream.h"

int main()
{
  const uint32_t expected = 0x43434444; // CCDD

  {
    std::ofstream ofs("test.huff", std::ios::binary);
    BitstreamWriter writer(ofs);
    if (!writer.write_value<uint32_t>(expected)) {
      std::cerr << "Failed to write value" << std::endl;
      return 1;
    }

    std::cout << "Wrote expected value: " << std::hex << expected << std::dec << std::endl;
    std::cout << "Wrote " << writer.bits_written() << " bits" << std::endl;
    std::cout << "  (Flushed " << writer.bits_flushed() << " bits)" << std::endl;
  }

  {
    std::ifstream ifs("test.huff", std::ios::binary);
    BitstreamReader reader(ifs);
    uint32_t actual = 0;
    if (!reader.read_value(actual)) {
      std::cerr << "Failed to read value" << std::endl;
      return 1;
    }

    std::cout << "Read actual value: " << std::hex << actual << std::dec << std::endl;
    std::cout << "Read " << reader.bits_read() << " bits" << std::endl;
    std::cout << "  (Filled " << reader.bits_filled() << " bits)" << std::endl;
  }

  return 0;
}
