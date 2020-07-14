# Sorting

Experiments and algorithms related to sorting.

## Quicksort

Our old favourite Quicksort. This is a fairly simple header-only quicksort implementation. A simple test program is included:

    make
    ./Quicksort

### Expected Output

    $ ./Quicksort
    Input array: 0, 20, 1, 3, 3, 9, 21
    Output array: 0, 1, 3, 3, 9, 20, 21

## Closest Pairs

This example shows how the Quicksort algorithm can be used as a primitive for constructing more sophisticated algorithms. This program takes a list of 2D points, and finds the closest pair:

    make
    ./ClosestPair

### Expected Output

    $ ./ClosestPair
    Input array: [0, 0], [2, 2], [3, 4], [6, 8], [8, 16], [10, 32], [12, 64]
    Closest pair: [2, 2] and [3, 4]
