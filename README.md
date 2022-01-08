# Algorithms, Data Structures, etc.

This repo contains implementations of various algorithms and data structures, mostly written in C++, Java, Ruby and Python. There are solutions for various interview questions, as well as some experimental code that I wrote to internalise concepts.

## Coding Interview Questions

In [coding-interview-book](./coding-interview-book) you'll find solutions for coding interview questions from the book [Cracking the Coding Interview](http://www.crackingthecodinginterview.com/). These are all written in Java.

Solutions for real interview questions can be found in [real-interviews](./real-interviews). These are all written in C++.

## ACM Coding Challenges

Solutions for the 2007 ACM Coding Challenges can be found in [acm-2007](./acm-2007). These are all written in Python. They may not be compatible with Python 3...

## Introduction to Algorithms

Some algorithms based on pseudocode from the book [Introduction to Algorithms](https://mitpress.mit.edu/books/introduction-algorithms-third-edition) (aka CLRS) can be found in [intro-to-algorithms](./intro-to-algorithms).

## Computational Geometry

Algorithms based on pseudocode from the book [Computational Geometry](https://www.springer.com/gp/book/9783540779735) can be found in [computational-geometry](./computational-geometry).

## AVL Tree

In [avl-tree](./avl-tree) you'll find C code for an AVL tree implementation that I extracted from an old project. The original code was much more complicated, because it was intended to be thread-safe. The new code has been simplified, and all concurrency-related logic has been removed.

## Matrix Multiplication

Although matrix multiplication does not involve particularly tricky algorithms, it is interesting to look at the different ways it can be sped up in practice. In [matrix-multiplication](./matrix-multiplication) you'll find examples that range from a naive sequential implementation, to MPI-based implementations that can be distributed across multiple nodes and computed on GPUs.

## Miscellaneous

Finally, there are various experiments, grouped by the kind of algorithm or data structure that I was learning about:

* [Hash Tables](./hash-tables)
* [Heaps](./heaps)
* [Sorting](./sorting)
