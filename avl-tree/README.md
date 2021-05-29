# AVL Tree

An AVL Tree implemented in C.

## Smoke Test

A simple test program has been included. You can build and run this using:

    make smoketest
    ./smoketest

### Output

The output should look something like this:

    $ ./smoketest
    ROOT: NIL
    INSERT OK: 1, 100
    ROOT: 1 (0): 100
      NIL
      NIL
    INSERT OK: 4, 400
    ROOT: 1 (1): 100
      NIL
      4 (0): 400
        NIL
        NIL
    INSERT OK: 2, 200
    ROOT: 2 (0): 200
      1 (0): 100
        NIL
        NIL
      4 (0): 400
    ...

This shows debug output as various operations are applied to the tree.

## Iterators

This AVL tree also supports iterators, loosely inspired by the iterators available in C++.

They support operations such as increment and decrement, both of which are in-place.

A tree tracks the iterators that refer to it, and if the tree is modified in any way, existing iterators will be marked invalid. This is designed to make usage of invalid iterators easier to detect, rather than having the program blow up with invalid memory access errors.

There is a separate test program for iterators:

    make iterators
    ./iterators

This example demonstrates finding the iterator for a given query, as well as increment and decrement operators.
