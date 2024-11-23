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

### ACM Coding Challenges

Solutions for 2007 ACM Coding Challenges, written in Python. These are very old, and were written to target Python 2. They will probably be incompatible with Python 3, without at least some modification.

[[code](./acm-2007)]

### AVL Tree

An AVL tree implementation, written in C, that I extracted from an old project. The original code was much more complicated, because it was intended to be thread-safe. The new code has been simplified, and all concurrency-related logic has been removed.

This directory also includes an AVL tree cheatsheet that I wrote up while extracting and tidying up the code.

[[code](./avl-tree/)] [[cheatsheet](./avl-tree/cheatsheet/)]

### Coding Interview Book

Solutions for coding interview questions from the book [Cracking the Coding Interview](http://www.crackingthecodinginterview.com/). These are all written in Java.

[[code](./coding-interview-book)]

### Computational Geometry

Algorithms based on pseudocode from the book [Computational Geometry](https://www.springer.com/gp/book/9783540779735). Written in C++.

[[code](./computational-geometry)]

### Interview Questions

Code samples that have been adapted from real interview questions that I have attempted. These were all from companies that no longer exist, so I don't feel bad publishing them here. These are all written in C++.

[[code](./interview-questions/)]

### Intro to Algorithms

Exercises from Introduction to Algorithms (3rd edition) - otherwise known as CLRS, after the initials of the authors.

This is a collection of Jupyter notebooks.

[[code](./intro-to-algorithms/)]

### Matrix Multiplication

Examples of matrix multiplication, ranging from a naive sequential implementation, to MPI-based implementations that can be distributed across multiple nodes and computed on GPUs.

[[code](./matrix-multiplication/)]

### Miscellaneous

Finally, there are various experiments, grouped by the kind of algorithm or data structure that I was learning about:

* [Arrays](./arrays)
* [Compression](./compression)
* [Hash Tables](./hash-tables)
* [Heaps](./heaps)
* [Sorting](./sorting)
