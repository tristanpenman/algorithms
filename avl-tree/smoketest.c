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

static const avl_tree_callbacks_t callbacks = {
    malloc,
    free,
    compare,
    release
};

int main(int argc, char **argv) {
  // create a tree
  avl_tree_ptr_t tree_ptr = avl_tree_create(&callbacks, NULL);
  assert(tree_ptr != NULL);
  avl_tree_print(tree_ptr, print);

  // values
  int values[] = {1, 4, 2, 10, 8, 3, 7};

  // insert some data
  for (int i = 0; i < 7; i++) {
    data_t *data_ptr = malloc(sizeof(data_t));
    data_ptr->key = values[i];
    data_ptr->value = values[i] * 100;
    bool inserted = avl_tree_insert(tree_ptr, data_ptr);
    if (inserted) {
      printf("INSERT OK: %d, %d\n", data_ptr->key, data_ptr->value);
    } else {
      printf("INSERT FAILED: %d, %d\n", data_ptr->key, data_ptr->value);
      free(data_ptr);
    }

    avl_tree_print(tree_ptr, print);
  }

  // query the tree for some data
  data_t query = {10, 0};
  void *result = avl_tree_find(tree_ptr, &query);
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

  avl_tree_clear(tree_ptr);

  printf("CLEARED\n");
  avl_tree_print(tree_ptr, print);

  // cleanup
  avl_tree_destroy(tree_ptr);

  return 0;
}
