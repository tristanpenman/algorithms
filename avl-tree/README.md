# AVL Tree

An AVL Tree implemented in C.

## Smoke Test

A simple test program has been included. You can build and run this using:

    make
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
