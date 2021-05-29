all: smoketest iterators

avl_tree.o: avl_tree.c avl_tree.h
	$(CC) -g -c -Wall -o avl_tree.o avl_tree.c

smoketest.o: avl_tree.h smoketest.c
	$(CC) -g -c -Wall -o smoketest.o smoketest.c

smoketest: avl_tree.o smoketest.o
	$(CC) -g -o smoketest avl_tree.o smoketest.o

iterators.o: avl_tree.h iterators.c
	$(CC) -g -c -Wall -o iterators.o iterators.c

iterators: avl_tree.o iterators.o
	$(CC) -g -o iterators avl_tree.o iterators.o

clean:
	rm -f *.o smoketest

