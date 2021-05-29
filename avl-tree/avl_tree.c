#include <stdint.h>
#include <string.h>

#include "avl_tree.h"

#define AVL_HEIGHT_LIMIT 20

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

// re-balance after deletion
#define AVL_REMOVE_REBALANCE(root_ptr, dir, done) do { \
  avl_tree_node_t* n = root_ptr->child_ptrs[!dir];     \
  int bal = dir == 0 ? -1 : +1;                        \
  if (n->balance == -bal) {                            \
    root_ptr->balance = n->balance = 0;                \
    AVL_SINGLE_ROTATION(root_ptr, dir);                \
  } else if (n->balance == bal) {                      \
    AVL_ADJUST_BALANCE(root_ptr, !dir, -bal);          \
    AVL_DOUBLE_ROTATION(root_ptr, dir);                \
  } else {                                             \
    root_ptr->balance = -bal;                          \
    n->balance = bal;                                  \
    AVL_SINGLE_ROTATION(root_ptr, dir);                \
    done = 1;                                          \
  }                                                    \
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
  avl_tree_iterator_ptr_t head_itr_ptr;      // linked list of iterators
  uint64_t nonce;                            // determines iterator validity after operations
} avl_tree_t;

typedef struct avl_tree_iterator {
  avl_tree_ptr_t tree_ptr;                      // tree that the iterator belongs to
  avl_tree_node_t *node_ptrs[AVL_HEIGHT_LIMIT]; // path to the node
  int depth;                                    // depth of path
  avl_tree_iterator_ptr_t next_itr_ptr;         // next iterator in linked list
  avl_tree_iterator_ptr_t prev_itr_ptr;         // previous iterator
  uint64_t nonce;                               // nonce to determine whether tree has changed
  int side;                                     // which side of the tree are we on
} avl_tree_iterator_t;

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
  tree_ptr->nonce++;

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

  tree_ptr->size++;
  tree_ptr->nonce++;

  return true;
}

// ----------------------------------------------------------------------------
//
// Iterator helpers
//
// ----------------------------------------------------------------------------

void avl_tree_iterator_init(avl_tree_iterator_ptr_t itr_ptr, avl_tree_ptr_t tree_ptr) {
  itr_ptr->tree_ptr = tree_ptr;
  itr_ptr->depth = 0;
  itr_ptr->nonce = tree_ptr->nonce;

  // set linked list pointers
  itr_ptr->next_itr_ptr = tree_ptr->head_itr_ptr;
  itr_ptr->prev_itr_ptr = NULL;

  // update old linked list head
  if (tree_ptr->head_itr_ptr) {
    tree_ptr->head_itr_ptr->prev_itr_ptr = itr_ptr;
  }

  // replace head
  tree_ptr->head_itr_ptr = itr_ptr;
}

avl_tree_iterator_ptr_t avl_tree_iterator_dive(avl_tree_ptr_t tree_ptr, int direction) {
  avl_tree_iterator_ptr_t itr_ptr = tree_ptr->callbacks_ptr->malloc_fn(sizeof(avl_tree_iterator_t));
  if (itr_ptr == NULL) {
    // error
    return NULL;
  }

  avl_tree_iterator_init(itr_ptr, tree_ptr);

  avl_tree_node_t *node_ptr = tree_ptr->root_ptr;
  while (node_ptr != NULL) {
    itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;
    node_ptr = node_ptr->child_ptrs[direction];
  }

  return itr_ptr;
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
  tree_ptr->size = 0;
  tree_ptr->user_ptr = user_ptr;
  tree_ptr->head_itr_ptr = NULL;
  tree_ptr->nonce = 0;

  return tree_ptr;
}

bool avl_tree_insert(avl_tree_ptr_t tree_ptr, void *data_ptr) {
  return tree_ptr->root_ptr == NULL ?
      avl_tree_insert_empty(tree_ptr, data_ptr) :
      avl_tree_insert_nonempty(tree_ptr, data_ptr);
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

  if (node_ptr == NULL) {
    return NULL;
  }

  return node_ptr->data_ptr;
}

void *avl_tree_remove(avl_tree_ptr_t tree_ptr, void *query_ptr) {
  if (tree_ptr->root_ptr == NULL) {
    return NULL;
  }

  avl_tree_node_t *node_ptr = tree_ptr->root_ptr;

  avl_tree_node_t *up[AVL_HEIGHT_LIMIT];
  int upd[AVL_HEIGHT_LIMIT];
  int top = 0;
  int done = 0;

  // Search down tree and save path
  while (1) {
    if (node_ptr == NULL) {
      return NULL;
    }

    bool less_than = tree_ptr->callbacks_ptr->compare_fn(node_ptr->data_ptr, query_ptr, tree_ptr->user_ptr);
    if (!less_than) {
      bool greater_than = tree_ptr->callbacks_ptr->compare_fn(query_ptr, node_ptr->data_ptr, tree_ptr->user_ptr);
      if (!greater_than) {
        break;
      }
    }

    upd[top] = less_than;
    up[top++] = node_ptr;

    node_ptr = node_ptr->child_ptrs[upd[top - 1]];
  }

  // remove the node
  if (node_ptr->child_ptrs[0] == NULL || node_ptr->child_ptrs[1] == NULL) {
    // which child is not null
    int dir = node_ptr->child_ptrs[0] == NULL;

    // fix parent
    if (top == 0) {
      tree_ptr->root_ptr = node_ptr->child_ptrs[dir];
    } else {
      up[top - 1]->child_ptrs[upd[top - 1]] = node_ptr->child_ptrs[dir];
    }

    // destroy node
    tree_ptr->callbacks_ptr->free_fn(node_ptr);

  } else {
    // find the in-order successor
    avl_tree_node_t *heir = node_ptr->child_ptrs[1];

    // save this path too
    upd[top] = 1;
    up[top++] = node_ptr;

    while (heir->child_ptrs[0] != NULL) {
      upd[top] = 0;
      up[top++] = heir;
      heir = heir->child_ptrs[0];
    }

    // swap node contents
    void *save = node_ptr->data_ptr;
    node_ptr->data_ptr = heir->data_ptr;
    heir->data_ptr = save;

    // unchild successor and fix parents
    up[top - 1]->child_ptrs[up[top - 1] == node_ptr] = heir->child_ptrs[1];

    // destroy
    tree_ptr->callbacks_ptr->free_fn(heir);
  }

  // walk back up the search path
  while (--top >= 0 && !done) {
    // update balance factors
    up[top]->balance += upd[top] != 0 ? -1 : +1;

    // terminate or re-balance as necessary
    if (up[top]->balance == -1 || up[top]->balance == 1) {
      break;
    } else if (up[top]->balance < -1 || up[top]->balance > -1) {
      AVL_REMOVE_REBALANCE(up[top], upd[top], done);

      if (top == 0) {
        tree_ptr->root_ptr = up[0];
      } else {
        up[top - 1]->child_ptrs[upd[top - 1]] = up[top];
      }
    }
  }

  tree_ptr->size--;
  tree_ptr->nonce++;

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
  // free iterators
  avl_tree_iterator_ptr_t itr_ptr = tree_ptr->head_itr_ptr;
  while (itr_ptr) {
    avl_tree_iterator_ptr_t next_itr_ptr = itr_ptr->next_itr_ptr;
    tree_ptr->callbacks_ptr->free_fn(itr_ptr);
    itr_ptr = next_itr_ptr;
  }

  // free tree
  avl_tree_node_destroy(tree_ptr->root_ptr, tree_ptr);
  tree_ptr->callbacks_ptr->free_fn(tree_ptr);
}

// ----------------------------------------------------------------------------
//
// Iterator API
//
// ----------------------------------------------------------------------------

avl_tree_iterator_ptr_t avl_tree_find_iterator(avl_tree_ptr_t tree_ptr, void *query_ptr) {
  avl_tree_iterator_ptr_t itr_ptr = tree_ptr->callbacks_ptr->malloc_fn(sizeof(avl_tree_iterator_t));
  if (itr_ptr == NULL) {
    // error
    return NULL;
  }

  avl_tree_iterator_init(itr_ptr, tree_ptr);

  avl_tree_node_t *node_ptr = tree_ptr->root_ptr;
  while (node_ptr != NULL) {
    if (tree_ptr->callbacks_ptr->compare_fn(query_ptr, node_ptr->data_ptr, tree_ptr->user_ptr)) {
      // query is < node, want to go left
      itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;
      node_ptr = node_ptr->child_ptrs[0];
    } else if (tree_ptr->callbacks_ptr->compare_fn(node_ptr->data_ptr, query_ptr, tree_ptr->user_ptr)) {
      // query is > node, want to go right
      itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;
      node_ptr = node_ptr->child_ptrs[1];
    } else {
      // must be equal
      itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;
      break;
    }
  }

  if (node_ptr == 0) {
    // node was not found; discard stack and return invalid iterator
    itr_ptr->depth = 0;
  }

  return itr_ptr;
}

avl_tree_iterator_ptr_t avl_tree_iterator_leftmost(avl_tree_ptr_t tree_ptr) {
  return avl_tree_iterator_dive(tree_ptr, 0);
}

avl_tree_iterator_ptr_t avl_tree_iterator_rightmost(avl_tree_ptr_t tree_ptr) {
  return avl_tree_iterator_dive(tree_ptr, 1);
}

bool avl_tree_iterator_valid(avl_tree_iterator_ptr_t itr_ptr) {
  return itr_ptr->depth > 0 && itr_ptr->nonce == itr_ptr->tree_ptr->nonce;
}

void avl_tree_iterator_decrement(avl_tree_iterator_ptr_t itr_ptr) {
  if (!avl_tree_iterator_valid(itr_ptr)) {
    // error
    return;
  }

  avl_tree_node_t *node_ptr = itr_ptr->node_ptrs[itr_ptr->depth - 1];

  // if node has left child, find right-most child of that subtree
  if (node_ptr->child_ptrs[0]) {
    node_ptr = node_ptr->child_ptrs[0];
    itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;

    avl_tree_node_t *left_ptr = node_ptr->child_ptrs[1];
    while (left_ptr) {
      node_ptr = left_ptr;
      itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;
      left_ptr = node_ptr->child_ptrs[1];
    }

    return;
  }

  // if node is right child of parent, return to parent
  if (itr_ptr->depth > 1 && itr_ptr->node_ptrs[itr_ptr->depth - 2]->child_ptrs[1] == itr_ptr->node_ptrs[itr_ptr->depth - 1]) {
    itr_ptr->depth--;
    return;
  }

  // node must be left child of parent, follow chain until node is right child parent, or the root
  while (itr_ptr->depth > 1 && itr_ptr->node_ptrs[itr_ptr->depth - 2]->child_ptrs[0] == itr_ptr->node_ptrs[itr_ptr->depth - 1]) {
    itr_ptr->depth--;
  }
  // then go up one level
  if (itr_ptr->depth > 0) {
    itr_ptr->depth--;
  }
}

void avl_tree_iterator_increment(avl_tree_iterator_ptr_t itr_ptr) {
  if (!avl_tree_iterator_valid(itr_ptr)) {
    // error
    return;
  }

  avl_tree_node_t *node_ptr = itr_ptr->node_ptrs[itr_ptr->depth - 1];

  // if node has right child, find left-most child of that subtree
  if (node_ptr->child_ptrs[1]) {
    node_ptr = node_ptr->child_ptrs[1];
    itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;

    avl_tree_node_t *left_ptr = node_ptr->child_ptrs[0];
    while (left_ptr) {
      node_ptr = left_ptr;
      itr_ptr->node_ptrs[itr_ptr->depth++] = node_ptr;
      left_ptr = node_ptr->child_ptrs[0];
    }

    return;
  }

  // if node is left child of parent, return to parent
  if (itr_ptr->depth > 1 && itr_ptr->node_ptrs[itr_ptr->depth - 2]->child_ptrs[0] == itr_ptr->node_ptrs[itr_ptr->depth - 1]) {
    itr_ptr->depth--;
    return;
  }

  // node must be right child of parent, follow chain until node is left child parent, or the root
  while (itr_ptr->depth > 1 && itr_ptr->node_ptrs[itr_ptr->depth - 2]->child_ptrs[1] == itr_ptr->node_ptrs[itr_ptr->depth - 1]) {
     itr_ptr->depth--;
  }
  // then go up one level
  if (itr_ptr->depth > 0) {
    itr_ptr->depth--;
  }
}

void *avl_tree_iterator_dereference(avl_tree_iterator_ptr_t itr_ptr) {
  if (avl_tree_iterator_valid(itr_ptr)) {
    return itr_ptr->node_ptrs[itr_ptr->depth - 1]->data_ptr;
  }

  return NULL;
}

void avl_tree_iterator_destroy(avl_tree_iterator_ptr_t itr_ptr) {
  if (itr_ptr == itr_ptr->tree_ptr->head_itr_ptr) {
    // iterator is head of tree
    if (itr_ptr->next_itr_ptr) {
      itr_ptr->next_itr_ptr->prev_itr_ptr = NULL;
    }
    itr_ptr->tree_ptr->head_itr_ptr = itr_ptr->next_itr_ptr;
  } else {
    itr_ptr->prev_itr_ptr->next_itr_ptr = NULL;
    if (itr_ptr->next_itr_ptr) {
      itr_ptr->next_itr_ptr->prev_itr_ptr = itr_ptr->prev_itr_ptr;
    }
  }

  itr_ptr->tree_ptr->callbacks_ptr->free_fn(itr_ptr);
}

// ----------------------------------------------------------------------------
//
// Debug API
//
// ----------------------------------------------------------------------------

void avl_tree_print(avl_tree_ptr_t tree_ptr, avl_tree_print_fn_t print_fn) {
  avl_tree_node_print(tree_ptr->root_ptr, print_fn, 0);
}
