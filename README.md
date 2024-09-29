# Algorithms, Data Structures, etc.

This repo contains implementations of various algorithms and data structures, mostly written in C++, Java, Ruby and Python. There are solutions for various interview questions, as well as some experimental code that I wrote to internalise concepts.

### Contents

- [ACM Coding Challenges](#acm-coding-challenges)
- [AVL Tree](#avl-tree)
- [Coding Interview Book](#coding-interview-book)
- [Computational Geometry](#computational-geometry)
- [Interview Questions](#interview-questions)
- [Matrix Multiplication](#matrix-multiplication)
- [Miscellaneous](#miscellaneous)

## ACM Coding Challenges

Solutions for the 2007 ACM Coding Challenges can be found in [acm-2007](./acm-2007). These are all written in Python. Note: They may not be compatible with Python 3.

## AVL Tree

In [avl-tree](./avl-tree) you'll find C code for an AVL tree implementation that I extracted from an old project. The original code was much more complicated, because it was intended to be thread-safe. The new code has been simplified, and all concurrency-related logic has been removed.

## Coding Interview Book

In [coding-interview-book](./coding-interview-book) you'll find solutions for coding interview questions from the book [Cracking the Coding Interview](http://www.crackingthecodinginterview.com/). These are all written in Java.

## Compression

Various compression algorithms (e.g. Huffman) can be found in [compression](./compression/).

## Computational Geometry

Algorithms based on pseudocode from the book [Computational Geometry](https://www.springer.com/gp/book/9783540779735) can be found in [computational-geometry](./computational-geometry).

## Interview Questions

In [interview-questions](./interview-questions) you will find a code samples that have been adapted from real interview questions that I have attempted. These were all from companies that no longer exist, so I don't feel bad publishing them here.

These are all written in C++.

## Matrix Multiplication

Although matrix multiplication does not involve particularly tricky algorithms, it is interesting to look at the different ways it can be sped up in practice. In [matrix-multiplication](./matrix-multiplication) you'll find examples that range from a naive sequential implementation, to MPI-based implementations that can be distributed across multiple nodes and computed on GPUs.

## Miscellaneous

Finally, there are various experiments, grouped by the kind of algorithm or data structure that I was learning about:

* [Arrays](./arrays)
* [Hash Tables](./hash-tables)
* [Heaps](./heaps)
* [Sorting](./sorting)
