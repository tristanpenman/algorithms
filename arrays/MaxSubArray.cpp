#include <iostream>
#include <utility>
#include <vector>

// This internal data structure used to contain the return values at
// various levels of recursion
template<typename Itr, typename Value>
struct MaxSubArrayCandidate
{
  Itr begin;
  Itr end;
  Value value;
};

// Template method that determines the maximum subarray that crosses
// the mid-point in a given range
template<typename Itr, typename Value>
MaxSubArrayCandidate<Itr, Value> maxSubArrayCrossover(Itr begin, Itr middle, Itr end)
{
  if (end - begin == 1) {
    return {begin, end, *begin};
  }

  Itr itr = middle;
  Itr left_itr = middle;
  Itr right_itr = middle;

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

  return {left_itr, right_itr + 1, left_sum + right_sum};
}

// Internal template method that recursively searches for the maximum
// subarray in a given range
template<typename Itr, typename Value>
MaxSubArrayCandidate<Itr, Value> recursiveMaxSubArray(Itr begin, Itr end)
{
  if (begin == end) {
    // Range is empty
    return {begin, end, 0};
  } else if (begin + 1 == end) {
    // Range contains just one element
    return {begin, end, *begin};
  }

  // Find middle of container
  auto middle = begin + (end - begin) / 2;

  // Find maximum subarrays either side of middle element
  auto left = recursiveMaxSubArray<Itr, Value>(begin, middle);
  auto right = recursiveMaxSubArray<Itr, Value>(middle, end);

  // Find maximum subarray that crosses middle element
  auto crossover = maxSubArrayCrossover<Itr, Value>(begin, middle, end);

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
template<typename Itr, typename Value>
std::pair<Itr, Itr> maxSubArray(Itr begin, Itr end)
{
  auto result = recursiveMaxSubArray<Itr, Value>(begin, end);

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
  typedef std::vector<int>::const_iterator Itr;

  // Find begin and end iterators for greatest subarray
  auto result = maxSubArray<Itr, int>(values.begin(), values.end());

  std::cerr << "Greatest subarray:" << std::endl;
  std::cerr << "  ";
  for (Itr itr = result.first; itr != result.second; itr++) {
    std::cerr << *itr << " ";
  }
  std::cerr << std::endl;

  return 0;
}
