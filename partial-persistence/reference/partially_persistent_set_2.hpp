#ifndef __PARTIALLY_PERSISTENT_BINARY_TREE_2_HPP
#define __PARTIALLY_PERSISTENT_BINARY_TREE_2_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <optional>

template <class _Key,
          class _Compare = std::less<_Key>,
          class _Alloc = std::allocator<_Key>>
class partially_persistent_set_2
{
    struct node_t;

    typedef std::shared_ptr<node_t> node_ptr;

    struct mod_t
    {
        node_ptr left_child;
        node_ptr parent;
        node_ptr right_child;
        unsigned int version;
    };

    typedef std::shared_ptr<mod_t> mod_ptr;

    struct node_t
    {
        mod_ptr mod;

        node_ptr left_child;
        node_ptr parent;
        node_ptr right_child;

        _Key value;
    };

    std::map<size_t, node_ptr> roots;

    size_t version;

public:

    partially_persistent_set_2()
      : version(0)
    {

    }

    std::optional<_Key> find(unsigned int queryVersion, const _Key &value) const
    {
        if (queryVersion == 0) {
            return {};
        }

        // Find latest root with a version number less than or equal queried version number
        auto root = roots.upper_bound(queryVersion);
        node_ptr node = std::prev(root, 1)->second;

        while (node) {
            if (_Compare()(value, node->value)) {
                if (node->mod && queryVersion >= node->mod->version) {
                    node = node->mod->left_child;
                } else {
                    node = node->left_child;
                }
            } else if (_Compare()(node->value, value)) {
                if (node->mod && queryVersion >= node->mod->version) {
                    node = node->mod->right_child;
                } else {
                    node = node->right_child;
                }
            } else {
                return node->value;
            }
        }

        return {};
    }

    std::pair<std::optional<_Key>, bool> insert(const _Key &value)
    {
        version++;

        if (roots.empty()) {
            node_ptr new_node(new node_t());
            new_node->value = value;
            auto root = std::make_pair(version, new_node);
            if (roots.insert(root).second) {
                return std::make_pair(value, true);
            } else {
                return std::make_pair(std::nullopt, false);
            }
        }

        node_ptr current_node = std::prev(roots.upper_bound(version - 1), 1)->second;
        node_ptr referencing_node;
        while (current_node) {
            referencing_node = current_node;
            if (_Compare()(value, current_node->value)) {
                if (current_node->mod) {
                    current_node = current_node->mod->left_child;
                } else {
                    current_node = current_node->left_child;
                }
            } else if (_Compare()(current_node->value, value)) {
                if (current_node->mod) {
                    current_node = current_node->mod->right_child;
                } else {
                    current_node = current_node->right_child;
                }
            } else {
                return std::make_pair(current_node->value, false);
            }
        }

        // Create new leaf node
        node_ptr leaf_node(new node_t());
        leaf_node->value = value;
        leaf_node->parent = referencing_node;

        // Update the left-child or right-child pointer of the parent
        current_node = leaf_node;
        while (referencing_node) {
            node_ptr new_node;
            if (_Compare()(current_node->value, referencing_node->value)) {
                if (referencing_node->mod) {
                    new_node.reset(new node_t());
                    new_node->left_child = current_node;
                    new_node->parent = referencing_node->mod->parent;
                    new_node->right_child = referencing_node->mod->right_child;
                    new_node->value = referencing_node->value;
                } else {
                    referencing_node->mod.reset(new mod_t());
                    referencing_node->mod->left_child = current_node;
                    referencing_node->mod->parent = referencing_node->parent;
                    referencing_node->mod->right_child = referencing_node->right_child;
                    referencing_node->mod->version = version;
                    break;
                }
            } else {
                if (referencing_node->mod) {
                    new_node.reset(new node_t());
                    new_node->left_child = referencing_node->mod->left_child;
                    new_node->parent = referencing_node->mod->parent;
                    new_node->right_child = current_node;
                    new_node->value = referencing_node->value;
                } else {
                    referencing_node->mod.reset(new mod_t());
                    referencing_node->mod->left_child = referencing_node->left_child;
                    referencing_node->mod->parent = referencing_node->parent;
                    referencing_node->mod->right_child = current_node;
                    referencing_node->mod->version = version;
                    break;
                }
            }

            if (new_node) {
                if (new_node->parent) {
                    // Update the grandparent if we had to create a new node
                    referencing_node = new_node->parent;
                    current_node = current_node->parent;
                } else {
                    // Update the root to point to the new node
                    roots.insert(std::make_pair(version, new_node));
                    break;
                }
            }
        }

        return std::make_pair(value, true);
    }

    unsigned int currentVersion() const
    {
        return version;
    }
};

#endif
