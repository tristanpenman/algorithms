#include <iostream>

using std::cout;
using std::endl;

// Recursive version of binary search
template<typename T>
const T* bsearch_recursive(const T* first, const T* last, const T& value) {

	if (first <= last) {

		// Calculate mid point
		int middle = (last - first) / 2;

		if (first[middle] > value) {
			// Search left subarray recursively
			return bsearch_recursive(first, first + middle - 1, value);
		} else if (first[middle] < value) {
			// Search right subarray recursively
			return bsearch_recursive(first + middle + 1 , last, value);
		}

		// Found value
		return &first[middle];
	}

	return NULL;
}

// Iterative version of binary search
template<typename T>
const T* bsearch_iterative(const T* first, const T* last, const T& value) {

	int min = 0, max = last - first;
	while (min <= max) {
		int mid = (min + max) / 2;
		if (first[mid] > value) {
			max = mid - 1;
		} else if (first[mid] < value) {
			min = mid + 1;
		} else {
			return &first[mid];
		}
	}

	return NULL;
}

int main(int argc, char** argv) {
	int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	cout << "Testing recursive binary search..." << endl;
	for (int i = 0; i < 10; i++) {
		cout << "Looking for: " << i << endl;
		const int* element = bsearch_recursive(array, array + 9, i);
		if (element != NULL) {
			cout << "  Found: " << *element << endl;
		} else {
			cout << "  Not found" << endl;
		}
	}
	
	cout << "Testing iterative binary search..." << endl;
	for (int i = 0; i < 10; i++) {
		cout << "Looking for: " << i << endl;
		const int* element = bsearch_iterative(array, array + 9, i);
		if (element != NULL) {
			cout << "  Found: " << *element << endl;
		} else {
			cout << "  Not found" << endl;
		}
	}

	return 0;
}
