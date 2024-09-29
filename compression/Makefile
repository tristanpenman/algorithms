CPP=g++
CPP_FLAGS=-O2 -std=c++17 # -ggdb

.PHONY: clean test

all: BitstreamTest HuffmanCompress HuffmanDecompress

BitstreamTest: BitstreamTest.cpp Bitstream.h
	$(CPP) $(CPP_FLAGS) -o BitstreamTest BitstreamTest.cpp

HuffmanCompress: HuffmanCompress.cpp Bitstream.h Huffman.h
	$(CPP) $(CPP_FLAGS) -o HuffmanCompress HuffmanCompress.cpp

HuffmanDecompress: HuffmanDecompress.cpp Bitstream.h Huffman.h
	$(CPP) $(CPP_FLAGS) -o HuffmanDecompress HuffmanDecompress.cpp

clean:
	rm -f BitstreamTest HuffmanCompress HuffmanDecompress
	rm -rf *.huff *.out

test: BitstreamTest HuffmanCompress HuffmanDecompress
	./BitstreamTest
	./HuffmanCompress test.bmp test.bmp.huff
	./HuffmanDecompress test.bmp.huff test.bmp.out
	diff test.bmp test.bmp.out
	./HuffmanCompress test.txt test.txt.huff
	./HuffmanDecompress test.txt.huff test.txt.out
	diff test.txt test.txt.out
