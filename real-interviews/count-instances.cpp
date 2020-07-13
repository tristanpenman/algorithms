#include <iostream>

using std::cout;
using std::endl;

// Find the first instance of a given value in a sorted array
template<typename T>
const T* bsearch_first(const T* first, const T* last, const T& value) {

	int min = 0, max = last - first;
	while (min <= max) {
		int mid = (min + max) / 2;
		if (first[mid] == value && mid == min) {
			// Found first instance
			return &first[mid];
		} else if (first[mid] == value) {
			// Search left subarray (including current element)
			max = mid;
		} else if (first[mid] < value) {
			// Search right (not including current element)
			min = mid + 1;
		} else {
			// Search left (not including current element)
			max = mid - 1;
		}
	}

	return NULL;
}

// Find the last instance of a given value in a sorted array
template<typename T>
const T* bsearch_last(const T* first, const T* last, const T& value) {
	
	int min = 0, max = last - first;
	while (min <= max) {
		int mid = (min + max) / 2;
		if (first[mid] == value && mid == min) {
			return &first[mid];
		} else if (first[mid] == value) {
			// Search right subarray inclusive
			min = mid;
		} else if (first[mid] < value) {
			// Search right subarray
			min = mid + 1;
		} else {
			// Search left subarray
			max = mid - 1;
		}
	}

	return NULL;
}

// Returns the number of instances of a given value in a sorted array
template<typename T>
int count_instances(const T* first, const T* last, const T& value) {

	const T* left = bsearch_first(first, last, value);
	if (left == NULL) {
		return 0;
	}	

	return bsearch_last(first, last, value) - left + 1;

}

int main(int argc, char** argv) {
	int array[] = {0, 1, 2, 3, 3, 3, 3, 3, 4, 5};
	
	cout << "Testing recursive binary search..." << endl;
	for (int i = 0; i < 10; i++) {
		cout << " " << i << ": " << count_instances(array, array + 9, i) 
		     << " instances" << endl;
	}

	return 0;
}
