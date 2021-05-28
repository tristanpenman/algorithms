#include "avl_tree.h"

// ----------------------------------------------------------------------------
//
// Rotation macros
//
// ----------------------------------------------------------------------------

#define AVL_SINGLE_ROTATION(root_ptr, dir) do {       \
  avl_tree_node_t* save = root_ptr->child_ptrs[!dir]; \
  root_ptr->child_ptrs[!dir] = save->child_ptrs[dir]; \
  save->child_ptrs[dir] = root_ptr;                   \
  root_ptr = save;                                    \
} while (0)

// two-way double rotation
#define AVL_DOUBLE_ROTATION(root_ptr, dir) do {                         \
  avl_tree_node_t* save = root_ptr->child_ptrs[!dir]->child_ptrs[dir];  \
  root_ptr->child_ptrs[!dir]->child_ptrs[dir] = save->child_ptrs[!dir]; \
  save->child_ptrs[!dir] = root_ptr->child_ptrs[!dir];                  \
  root_ptr->child_ptrs[!dir] = save;                                    \
  save = root_ptr->child_ptrs[!dir];                                    \
  root_ptr->child_ptrs[!dir] = save->child_ptrs[dir];                   \
  save->child_ptrs[dir] = root_ptr;                                     \
  root_ptr = save;                                                      \
} while (0)

// adjust balance before double rotation
#define AVL_ADJUST_BALANCE(root_ptr, dir, bal) do { \
  avl_tree_node_t* n1 = root_ptr->child_ptrs[dir];  \
  avl_tree_node_t* n2 = n1->child_ptrs[!dir];       \
  if (n2->balance == 0) {                           \
    root_ptr->balance = n->balance = 0;             \
  } else if (n2->balance == bal) {                  \
    root_ptr->balance = -bal;                       \
    n1->balance = 0;                                \
  } else {                                          \
    root_ptr->balance = 0;                          \
    n1->balance = bal;                              \
  }                                                 \
  n2->balance = 0;                                  \
} while (0)

// re-balance after insertion
#define AVL_INSERT_REBALANCE(root_ptr, dir) do {  \
  avl_tree_node_t* n = root_ptr->child_ptrs[dir]; \
  int bal = dir == 0 ? -1 : +1;                   \
  if (n->balance == bal) {                        \
    root_ptr->balance = n->balance = 0;           \
    AVL_SINGLE_ROTATION(root_ptr, !dir);          \
  } else {                                        \
    AVL_ADJUST_BALANCE(root_ptr, dir, bal);       \
    AVL_DOUBLE_ROTATION(root_ptr, !dir);          \
  }                                               \
} while (0)

// ----------------------------------------------------------------------------
//
// Internal structs
//
// ----------------------------------------------------------------------------

typedef struct avl_tree_node {
  struct avl_tree_node *child_ptrs[2]; // left and right children
  signed char balance;                 // balance factor
  void *data_ptr;                      // actual data in this node
} avl_tree_node_t;

typedef struct avl_tree {
  const avl_tree_callbacks_t *callbacks_ptr; // user-defined callback functions
  avl_tree_node_t *root_ptr;                 // root of the tree
  void *user_ptr;                            // additional user-defined data
  size_t size;                               // number of nodes in the tree
} avl_tree_t;

// ----------------------------------------------------------------------------
//
// Node helpers
//
// ----------------------------------------------------------------------------

static bool avl_tree_node_apply(
    avl_tree_node_t *node_ptr, avl_tree_apply_fn_t apply_fn, void *user_ptr
) {
  return node_ptr == NULL || (
      avl_tree_node_apply(node_ptr->child_ptrs[0], apply_fn, user_ptr) &&
      apply_fn(node_ptr->data_ptr, user_ptr) &&
      avl_tree_node_apply(node_ptr->child_ptrs[1], apply_fn, user_ptr));
}

static void avl_tree_node_destroy(avl_tree_node_t *node_ptr, avl_tree_ptr_t tree_ptr) {
  if (node_ptr != NULL) {
    avl_tree_node_destroy(node_ptr->child_ptrs[0], tree_ptr);
    avl_tree_node_destroy(node_ptr->child_ptrs[1], tree_ptr);
    tree_ptr->callbacks_ptr->release_fn(node_ptr->data_ptr, tree_ptr->user_ptr);
    tree_ptr->callbacks_ptr->free_fn(node_ptr);
  }
}

static void avl_tree_node_init(avl_tree_node_t *node_ptr, void *data_ptr) {
  node_ptr->balance = 0;
  node_ptr->child_ptrs[0] = NULL;
  node_ptr->child_ptrs[1] = NULL;
  node_ptr->data_ptr = data_ptr;
}

static void avl_tree_node_print(avl_tree_node_t *node_ptr, avl_tree_print_fn_t print_fn, int depth) {
  if (node_ptr == NULL) {
    print_fn(NULL, 0, depth);
  } else {
    print_fn(node_ptr->data_ptr, node_ptr->balance, depth);
    avl_tree_node_print(node_ptr->child_ptrs[0], print_fn, depth + 1);
    avl_tree_node_print(node_ptr->child_ptrs[1], print_fn, depth + 1);
  }
}

// ----------------------------------------------------------------------------
//
// Tree helpers
//
// ----------------------------------------------------------------------------

static bool avl_tree_insert_empty(avl_tree_ptr_t tree_ptr, void *data_ptr) {
  avl_tree_node_t *node_ptr = tree_ptr->callbacks_ptr->malloc_fn(sizeof(avl_tree_node_t));
  if (node_ptr == NULL) {
    // TODO: error
    return false;
  }

  avl_tree_node_init(node_ptr, data_ptr);
  tree_ptr->root_ptr = node_ptr;
  tree_ptr->size++;

  return true;
}

static bool avl_tree_insert_nonempty(avl_tree_ptr_t tree_ptr, void *data_ptr) {
  avl_tree_node_t *q = NULL;
  avl_tree_node_t *t = NULL;

  avl_tree_node_t *p = tree_ptr->root_ptr;
  avl_tree_node_t *s = tree_ptr->root_ptr;

  int dir = -1;

  // Search down the tree, saving re-balance points
  while (1) {
    if (tree_ptr->callbacks_ptr->compare_fn(data_ptr, p->data_ptr, tree_ptr->user_ptr)) {
      // node to insert < p
      dir = 0;
    } else if (tree_ptr->callbacks_ptr->compare_fn(p->data_ptr, data_ptr, tree_ptr->user_ptr)) {
      // node to insert > p
      dir = 1;
    } else {
      // node to insert == p
      return false;
    }

    q = p->child_ptrs[dir];
    if (q == NULL) {
      break;
    }

    if (q->balance != 0) {
      t = p;
      s = q;
    }

    p = q;
  }

  q = tree_ptr->callbacks_ptr->malloc_fn(sizeof(avl_tree_node_t));
  if (q == NULL) {
    // handle error
  }

  avl_tree_node_init(q, data_ptr);

  p->child_ptrs[dir] = q;

  // update balance factors
  for (p = s; p != q; p = p->child_ptrs[dir]) {
    dir = tree_ptr->callbacks_ptr->compare_fn(p->data_ptr, data_ptr, tree_ptr->user_ptr);
    if (dir) {
      // node to insert > p
      p->balance++;
    } else {
      // node to insert <= p
      p->balance--;
    }
  }

  // save re-balance point for parent fix
  q = s;

  // re-balance if necessary
  if (s->balance > 1 || s->balance < -1) {
    dir = tree_ptr->callbacks_ptr->compare_fn(s->data_ptr, data_ptr, tree_ptr->user_ptr);

    AVL_INSERT_REBALANCE(s, dir);
  }

  // fix parent
  if (q == tree_ptr->root_ptr) {
    tree_ptr->root_ptr = s;
  } else if (q == t->child_ptrs[1]) {
    t->child_ptrs[1] = s;
  } else {
    t->child_ptrs[0] = s;
  }

  return true;
}

// ----------------------------------------------------------------------------
//
// Basic API
//
// ----------------------------------------------------------------------------

avl_tree_ptr_t avl_tree_create(const avl_tree_callbacks_t *callbacks_ptr, void *user_ptr) {
  avl_tree_ptr_t tree_ptr = (avl_tree_ptr_t) callbacks_ptr->malloc_fn(sizeof(avl_tree_t));
  if (tree_ptr == NULL) {
    // TODO: error
    return NULL;
  }

  tree_ptr->callbacks_ptr = callbacks_ptr;
  tree_ptr->root_ptr = NULL;
  tree_ptr->user_ptr = user_ptr;

  return tree_ptr;
}

bool avl_tree_insert(avl_tree_ptr_t tree_ptr, void *data_ptr) {
  if (tree_ptr->root_ptr == NULL) {
    return avl_tree_insert_empty(tree_ptr, data_ptr);
  } else {
    return avl_tree_insert_nonempty(tree_ptr, data_ptr);
  }
}

void *avl_tree_find(avl_tree_ptr_t tree_ptr, void *query_ptr) {
  avl_tree_node_t *node_ptr = tree_ptr->root_ptr;
  while (node_ptr != NULL) {
    if (tree_ptr->callbacks_ptr->compare_fn(query_ptr, node_ptr->data_ptr, tree_ptr->user_ptr)) {
      // query is < node, want to go left
      node_ptr = node_ptr->child_ptrs[0];
    } else if (tree_ptr->callbacks_ptr->compare_fn(node_ptr->data_ptr, query_ptr, tree_ptr->user_ptr)) {
      // query is > node, want to go right
      node_ptr = node_ptr->child_ptrs[1];
    } else {
      // must be equal
      break;
    }
  }

  return node_ptr->data_ptr;
}

size_t avl_tree_size(avl_tree_ptr_t tree_ptr) {
  return tree_ptr->size;
}

bool avl_tree_apply(avl_tree_ptr_t tree_ptr, avl_tree_apply_fn_t apply_fn) {
  return avl_tree_node_apply(tree_ptr->root_ptr, apply_fn, tree_ptr->user_ptr);
}

void avl_tree_clear(avl_tree_ptr_t tree_ptr) {
  avl_tree_node_destroy(tree_ptr->root_ptr, tree_ptr);
  tree_ptr->root_ptr = NULL;
}

void avl_tree_destroy(avl_tree_ptr_t tree_ptr) {
  avl_tree_node_destroy(tree_ptr->root_ptr, tree_ptr);
  tree_ptr->callbacks_ptr->free_fn(tree_ptr);
}

// ----------------------------------------------------------------------------
//
// Debug API
//
// ----------------------------------------------------------------------------

void avl_tree_print(avl_tree_ptr_t tree_ptr, avl_tree_print_fn_t print_fn) {
  avl_tree_node_print(tree_ptr->root_ptr, print_fn, 0);
}
