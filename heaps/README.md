# Heaps

Experiments relating to heaps.

## Bubble-Down Heap

This is a simple heap, implemented in C++. The main purpose of the program is to demonstrate bubble-down heap construction, from an array. The heap is constructed from the command line arguments passed in, and the output is the sequence in which those elements are removed from the heap.

Compile and run using:

    make
    ./BubbleDownHeap [element1 element2 ...]

Example output for `BubbleDownHeap 1 5 A C 2 3`:

    C
    A
    5
    3
    2
    1
