#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

string read_from_file(const char *filename)
{
  FILE* file = fopen(filename, "r");
  if (!file) {
    cerr << "Failed to open file '" << filename << "'" << endl;
    exit(1);
  }

  // find the length of the entire file
  fseek(file, 0, SEEK_END);
  const size_t size = ftell(file);
  rewind(file);

  // read it into a string
  string str(size, 0);
  auto bytes_read = fread(str.data(), sizeof(char), size, file);
  if (bytes_read != size) {
    cerr << "Warning: bytes read (" << bytes_read << ") does not match file length (" << size << ")" << endl;
  }

  fclose(file);

  return str;
}
