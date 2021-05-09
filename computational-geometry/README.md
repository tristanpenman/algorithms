# Computational Geometry

These examples are based on pseudocode from the book [Computational Geometry](https://www.springer.com/gp/book/9783540779735).

## Convex Hull

This is an implementation of [Graham's scan](https://en.wikipedia.org/wiki/Graham_scan), an algorithm for finding the convex hull of a set of points.

Points are read from a file called `points.txt`.

A simple test program is included:

    make
    ./ConvexHull

### Expected Output

The output of the program should look like the following:

    Points:
    p00: (-5,-6)
    p01: (6,-4)
    p02: (5.5,-3)
    p03: (8,0)
    p04: (5,0)
    p05: (4,2)
    p06: (1,3)
    p07: (0,2)
    p08: (-1,1)
    p09: (-1.5,2)
    p10: (-1.5,6)
    p11: (-5.5,1.5)
    p12: (-8,-1)
    Convex hull:
    p00 - p01
    p01 - p03
    p03 - p10
    p10 - p12
    p12 - p00
