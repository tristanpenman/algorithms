#include <iostream>
#include <utility>
#include <vector>

// This internal data structure used to contain the return values at
// various levels of recursion
template<typename Iterator, typename Value>
struct _max_subarray_candidate_t {
  Iterator begin;
  Iterator end;
  Value value;
};

// Template method that determines the maximum subarray that crosses
// the mid-point in a given range
template<typename Iterator, typename Value>
_max_subarray_candidate_t<Iterator, Value> _max_subarray_crossover(
    Iterator begin, Iterator middle, Iterator end) {
  if (end - begin == 1) {
    return (struct _max_subarray_candidate_t<Iterator, Value>)
      {begin, end, *begin};
  }

  Iterator itr = middle;
  Iterator left_itr = middle;
  Iterator right_itr = middle;

  int left_sum = 0;
  int right_sum = 0;
  int sum = 0;

  bool valid = false;

  // Find maximal left subarray
  do {
    sum += *itr;

    if (!valid || sum > left_sum) {
      left_sum = sum;
      left_itr = itr;
      valid = true;
    }
  } while (itr-- != begin);

  // Find maximal right subarray
  sum = 0;
  valid = false;
  itr = middle;
  while (++itr != end) {
    sum += *itr;

    if (!valid || sum > right_sum) {
      right_sum = sum;
      right_itr = itr;
      valid = true;
    }
  }

  return (struct _max_subarray_candidate_t<Iterator, Value>)
    {left_itr, right_itr + 1, left_sum + right_sum};
}

// Internal template method that recursively searches for the maximum
// subarray in a given range
template<typename Iterator, typename Value>
_max_subarray_candidate_t<Iterator, Value> _max_subarray(Iterator begin, Iterator end) {
  typedef struct _max_subarray_candidate_t<Iterator, Value> Candidate;

  if (begin == end) {
    // Range is empty
    return (Candidate){begin, end, 0};
  } else if (begin + 1 == end) {
    // Range contains just one element
    return (Candidate){begin, end, *begin};
  }

  // Find middle of container
  Iterator middle = begin + (end - begin) / 2;

  // Find maximum subarrays either side of middle element
  Candidate left = _max_subarray<Iterator, Value>(begin, middle);
  Candidate right = _max_subarray<Iterator, Value>(middle, end);

  // Find maximum subarray that crosses middle element
  Candidate crossover = _max_subarray_crossover<Iterator, Value>(begin, middle, end);

  if (left.begin != left.end &&
      left.value >= right.value && left.value >= crossover.value) {
    // Subarray on left side is greatest
    return left;
  }

  if (crossover.begin != crossover.end &&
      crossover.value >= left.value && crossover.value >= right.value) {
    // Subarray that crosses middle is greatest
    return crossover;
  }

  // Subarray on right side is greatest
  return right;
}

// Template method that kicks off a recursive search for the
// maximum subarray in a given range, as defined by two iterators
template<typename Iterator, typename Value>
std::pair<Iterator, Iterator> max_subarray(Iterator begin, Iterator end) {
  _max_subarray_candidate_t<Iterator, Value> result = _max_subarray<Iterator, Value>(begin, end);
  return make_pair(result.begin, result.end);
}

// ----------------------------------------------------------------------------

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Usage: " << std::endl
              << "  " << argv[0] << " [1] [2] ... [n]" << std::endl;
    return 1;
  }

  std::cerr << "Looking for greatest subarray in:" << std::endl;
  std::cerr << "  ";
  std::vector<int> values;
  for (int i = 1; i < argc; i++) {
    int v = atoi(argv[i]);
    values.push_back(v);
    std::cerr << v << " ";
  }
  std::cerr << std::endl;

  // Make things prettier
  typedef std::vector<int>::const_iterator Iterator;
  typedef std::pair<Iterator, Iterator> Result;

  // Find begin and end iterators for greatest subarray
  Result result = max_subarray<Iterator, int>(values.begin(), values.end());

  std::cerr << "Greatest subarray:" << std::endl;
  std::cerr << "  ";
  for (Iterator itr = result.first; itr != result.second; itr++) {
    std::cerr << *itr << " ";
  }
  std::cerr << std::endl;

  return 0;
}
