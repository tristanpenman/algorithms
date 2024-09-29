#pragma once

#include <cstdint>
#include <iostream>

class BitstreamReader
{
public:
  explicit BitstreamReader(std::istream& ifs)
    : _ifs(ifs)
    , _buffer(0)
    , _count(0)
    , _filled(0)
    , _read()
  {

  }

  [[nodiscard]] size_t bits_filled() const
  {
    return _filled;
  }

  [[nodiscard]] size_t bits_read() const
  {
    return _read;
  }

  // always reads 8 bits into buffer
  bool fill()
  {
    if (!_ifs) {
      return false;
    }

    _ifs.read((char *) &_buffer, 1);
    if (!_ifs) {
        return false;
    }

    _filled += 8;
    _count = 8;

    return true;
  }

  bool read_bit(int &bit)
  {
    if (_count == 0) {
      if (!fill()) {
          return false;
      }
    }

    _count--;
    _read++;

    bit = _buffer >> _count;
    bit &= 0x1;

    return true;
  }

  template<typename T>
  bool read_value(T &value)
  {
    value = 0;
    auto sz = sizeof(T) * 8;

    for (int i = 0; i < sz; i++) {
      int bit;
      if (!read_bit(bit)) {
        return false;
      }

      value <<= 1;
      value |= bit & 0x1;
    }

    return true;
  }

private:
  std::istream& _ifs;

  uint8_t _buffer;
  size_t  _count;
  size_t  _filled;
  size_t  _read;
};

class BitstreamWriter
{
public:
  explicit BitstreamWriter(std::ostream& ofs)
    : _ofs(ofs)
    , _buffer(0)
    , _count(0)
    , _flushed(0)
    , _written(0)
  {

  }

  ~BitstreamWriter()
  {
    flush();
  }

  [[nodiscard]] size_t bits_flushed() const
  {
    return _flushed;
  }

  // includes flushed bits and those in buffer
  [[nodiscard]] size_t bits_written() const
  {
    return _written;
  }

  // flushing always writes 8 bits
  bool flush()
  {
    if (_count > 0) {
      _buffer <<= 8 - _count;
      _ofs.write((const char*) &_buffer, 1);
      if (!_ofs) {
        return false;
      }

      _flushed += 8;
      _buffer = 0;
      _count = 0;
    }

    _ofs.flush();

    return true;
  }

  bool write_bit(int bit)
  {
    _buffer = (_buffer << 1) | (bit & 0x1);
    _count++;
    _written++;

    if (_count == 8) {
      if (!flush()) {
        return false;
      }
    }

    return true;
  }

  bool write_string(const char *str)
  {
    int i = 0;
    char c;
    while ((c = str[i++]) != 0) {
      if (!write_value(c)) {
        return false;
      }
    }

    return true;
  }

  template<typename T>
  bool write_value(const T &value)
  {
    auto sz = sizeof(T) * 8;
    for (int i = 0; i < sz; i++) {
      auto bit = value >> (sz - i - 1);
      if (!write_bit(bit)) {
        return false;
      }
    }

    return true;
  }

private:
  std::ostream& _ofs;

  uint8_t _buffer;
  size_t  _count;
  size_t  _flushed;
  size_t  _written;
};
