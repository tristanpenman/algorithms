#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

typedef struct {
  uint16_t key;
  uint32_t value;
} data_t;

static bool compare(void *left_ptr, void *right_ptr, void *user_ptr) {
  return ((data_t *) left_ptr)->key < ((data_t *) right_ptr)->key;
}

static void print(void *data_ptr, int balance, int depth) {
  if (depth == 0) {
    printf("ROOT: ");
  } else {
    for (int i = 0; i < depth; i++) {
      printf("  ");
    }
  }

  if (data_ptr == NULL) {
    printf("NIL\n");
  } else {
    printf("%d (%d): %d\n", ((data_t *) data_ptr)->key, balance, ((data_t *) data_ptr)->value);
  }
}

static void release(void *data_ptr, void *user_ptr) {
  free(data_ptr);
}

static void error(const char *message) {
  printf("Error: %s\n", message);
  exit(1);
}

static const avl_tree_callbacks_t callbacks = {
    malloc,
    free,
    compare,
    release,
    error
};

int main(int argc, char **argv) {
  // create a tree
  avl_tree_ptr_t tree_ptr = avl_tree_create(&callbacks, NULL);
  assert(tree_ptr != NULL);

  // insert some data
  for (int i = 0; i < 100; i++) {
    data_t *data_ptr = malloc(sizeof(data_t));
    data_ptr->key = i;
    data_ptr->value = i * 100;
    bool inserted = avl_tree_insert(tree_ptr, data_ptr);
    if (inserted) {
      printf("INSERT OK: %d, %d\n", data_ptr->key, data_ptr->value);
    } else {
      printf("INSERT FAILED: %d, %d\n", data_ptr->key, data_ptr->value);
      free(data_ptr);
    }
  }

  avl_tree_print(tree_ptr, print);

  // query the tree for some data
  data_t query = {10, 0};
  void *result = avl_tree_find(tree_ptr, &query);
  assert(result != NULL);

  avl_tree_iterator_ptr_t itr_ptr = avl_tree_find_iterator(tree_ptr, &query);
  assert(itr_ptr != NULL);
  assert(avl_tree_iterator_valid(itr_ptr));

  result = avl_tree_iterator_dereference(itr_ptr);
  assert(result != NULL);

  // check the data
  data_t *data_ptr = (data_t *) result;
  if (data_ptr) {
    printf("FOUND: %d, %d\n", data_ptr->key, data_ptr->value);
    assert(data_ptr->key == 10);
    assert(data_ptr->value == 1000);
  } else {
    printf("NOT FOUND: %d\n", query.key);
  }

  // remove the same node
  data_t *removed_ptr = avl_tree_remove(tree_ptr, &query);
  assert(removed_ptr);
  printf("REMOVED: %d, %d\n", removed_ptr->key, removed_ptr->value);
  assert(removed_ptr->key == 10);
  assert(removed_ptr->value == 1000);
  free(removed_ptr);

  // try to dereference the iterator again...
  result = avl_tree_iterator_dereference(itr_ptr);
  assert(result == NULL);

  // we could also check validity directly
  assert(!avl_tree_iterator_valid(itr_ptr));

  // explicitly free an iterator
  avl_tree_iterator_destroy(itr_ptr);

  // forward traversal of the tree using an iterator
  for (itr_ptr = avl_tree_iterator_leftmost(tree_ptr); avl_tree_iterator_valid(itr_ptr);
      avl_tree_iterator_increment(itr_ptr)) {
    result = avl_tree_iterator_dereference(itr_ptr);
    assert(data_ptr);
    data_ptr = (data_t *) result;
    printf("%d, %d\n", data_ptr->key, data_ptr->value);
  }

  // backwards traversal
  for (itr_ptr = avl_tree_iterator_rightmost(tree_ptr); avl_tree_iterator_valid(itr_ptr);
      avl_tree_iterator_decrement(itr_ptr)) {
    result = avl_tree_iterator_dereference(itr_ptr);
    assert(data_ptr);
    data_ptr = (data_t *) result;
    printf("%d, %d\n", data_ptr->key, data_ptr->value);
  }

  // implicitly destroys any remaining iterators
  avl_tree_destroy(tree_ptr);

  return 0;
}
