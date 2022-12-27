# Arrays

Array algorithms based on pseudocode from the book Introduction to Algorithms (aka CLRS).

## Max Subarray

Given an array of integers, find a sub-array that when summed, has the greatest sum of all possible sub-arrays:

    make
    ./MaxSubArray 3 4 1 -4 3 -3

### Expected Output

    $ ./MaxSubArray 3 4 1 -4 3 -3
    Looking for greatest sub-array in:
      3 4 1 -4 3 -3
    Greatest sub-array:
      3 4 1
