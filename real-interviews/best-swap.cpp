#include <iostream>

int swappedIndex(unsigned int i, unsigned int s1, unsigned int s2)
{
	if (i == s1) {
		return s2;
	} else if (i == s2) {
		return s1;
	} else {
		return i;
	}
}

int compareSwaps(const char *input, size_t inputLen, unsigned int a1,
	unsigned int a2, unsigned int b1, unsigned int b2)
{
	for (unsigned int i = 0; i < inputLen; ++i) {
		char a = input[swappedIndex(i, a1, a2)];
		char b = input[swappedIndex(i, b1, b2)];;
		if (a < b) {
			return -1;
		} else if (a > b) {
			return 1;
		}
	}

	return 0;
}

std::pair<int, int> findBestSwap(const char *input, size_t inputLen)
{
	unsigned int best1 = 0, best2 = 0;
	for (unsigned int i = 0; i < inputLen - 1; ++i) {
		for (unsigned int j = 1; j < inputLen; j++) {
			if (compareSwaps(input, inputLen, best1, best2, i, j) < 0) {
				best1 = i;
				best2 = j;
			}
		}
	}

	return std::make_pair(best1, best2);
}

int main()
{
	std::string input;
	std::cin >> input;
	std::pair<unsigned int, unsigned int> bestSwap =
		findBestSwap(input.c_str(), input.size());
	std::cout << bestSwap.first << " " << bestSwap.second << std::endl;
	std::swap(input[bestSwap.first], input[bestSwap.second]);
	std::cout << input << std::endl;
	return 0;
}
