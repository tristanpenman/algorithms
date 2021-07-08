# Hash Tables

Experiments relating to hash tables.

## Open Addressing

There is a hash table implemented in C++, using open addressing. The example uses a pathological hash function that always returns a hash value that is the length of the input string.

Compile and run using:

    make
    ./OpenAddressing

Expected output:

    insert abc: 3
    lookup abc: 123
    insert def: 4
    lookup def: 456
    lookup abc: 123
