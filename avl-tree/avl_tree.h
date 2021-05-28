#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <stdbool.h>
#include <stddef.h>

// ----------------------------------------------------------------------------
//
// Callbacks
//
// ----------------------------------------------------------------------------

// callback used to allocate memory; used to allocate memory for internal data structures
typedef void *(*avl_tree_malloc_fn_t)(size_t bytes);

// callback used to free memory; used to free internal data structures
typedef void (*avl_tree_free_fn_t)(void *);

// callback used to compare two nodes; should return true when left < right
typedef bool (*avl_tree_compare_fn_t)(void *left_ptr, void *right_ptr, void *user_ptr);

// callback used to release the data associated with a node
// no guarantees about order of traversal
typedef void (*avl_tree_release_fn_t)(void *data_ptr, void *user_ptr);

// callback used to apply an operation to the data associated with a node
typedef bool (*avl_tree_apply_fn_t)(void *data_ptr, void *user_ptr);

// convenience data structure
// allows callbacks to be swapped out dynamically
typedef struct {
  avl_tree_malloc_fn_t malloc_fn;
  avl_tree_free_fn_t free_fn;
  avl_tree_compare_fn_t compare_fn;
  avl_tree_release_fn_t release_fn;
} avl_tree_callbacks_t;

// ----------------------------------------------------------------------------
//
// Basic API
//
// ----------------------------------------------------------------------------

// handle for the tree itself
// avl_tree is treated as an opaque struct
struct avl_tree;
typedef struct avl_tree *avl_tree_ptr_t;

// create a new AVL tree using the callbacks provided
// user_ptr is user-defined data
avl_tree_ptr_t avl_tree_create(const avl_tree_callbacks_t *, void *user_ptr);

// attempts to insert data into the tree
// returns true if the data was inserted
bool avl_tree_insert(avl_tree_ptr_t, void *data_ptr);

// attempts to find data in the tree, using a query
// returns data pointer if found, NULL otherwise
void *avl_tree_find(avl_tree_ptr_t, void *query_ptr);

// remove and return the data that matches a particular query
void *avl_tree_remove(avl_tree_ptr_t, void *query_ptr);

// return the number of nodes in the tree
size_t avl_tree_size(avl_tree_ptr_t);

// apply a callback function to all nodes in the tree, using in order traversal
// callback can return false to terminate traversal
bool avl_tree_apply(avl_tree_ptr_t, avl_tree_apply_fn_t);

// erase all elements in the tree
void avl_tree_clear(avl_tree_ptr_t);

// releases all of the data in the tree, and frees memory for internal data structures
void avl_tree_destroy(avl_tree_ptr_t);

// ----------------------------------------------------------------------------
//
// Debug API
//
// ----------------------------------------------------------------------------

// callback to print a line of output from avl_tree_print
typedef void (*avl_tree_print_fn_t)(void *data_ptr, int balance, int depth);

// print an AVL tree
void avl_tree_print(avl_tree_ptr_t, avl_tree_print_fn_t print_fn);

#endif
