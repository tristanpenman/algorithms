require_relative 'node'

module PersistentTree
  ##
  # Contains publicly visible algorithms used by the Tree class
  #
  # These algorithms are exposed so that they are more testable and re-usable
  #
  module Algorithms
    class << self
      ##
      # Find a value in the tree represented by the given root node
      #
      # Returns the stored value if found; raises KeyError otherwise
      #
      def find(root_node, min_mod_version, key)
        current_node = root_node
        while current_node
          if current_node.value > key
            current_node = current_node.get_left_child(min_mod_version)
          elsif current_node.value < key
            current_node = current_node.get_right_child(min_mod_version)
          else
            return current_node
          end
        end

        raise KeyError
      end

      ##
      # Test for existence of a value in a tree represented by the given root node
      #
      # Returns true if the value is found; false otherwise
      #
      def include?(root_node, min_mod_version, key)
        current_node = root_node
        while current_node
          if current_node.value > key
            current_node = current_node.get_left_child(min_mod_version)
          elsif current_node.value < key
            current_node = current_node.get_right_child(min_mod_version)
          else
            return true
          end
        end
        false
      end

      ##
      # Inserts a value into a partially-persistent tree rooted at the given node, creating a new version of the tree
      #
      # This method allows existing values to be replaced. Note that this will result in a new version of the tree
      # being created, in which the new item takes the place of the existing item.
      #
      # Returns the root node that should be used for queries on the new version of the tree.
      #
      def insert!(root_node, root_version, item)
        insert_with_options(root_node, root_version, item, allow_replacement: true)
      end

      ##
      # Inserts a value into a partially-persistent tree rooted at the given node, creating a new version of the tree
      #
      # Existing items will **not** be replaced.
      #
      # Returns the root node that should be used for queries on the new version of the tree.
      #
      def insert(root_node, root_version, item)
        insert_with_options(root_node, root_version, item)
      end

      ##
      # Returns the size of the data structure
      #
      def size(root_node, root_version)
        count = 1
        if (child = root_node.get_left_child(root_version))
          count += size(child, root_version)
        end
        if (child = root_node.get_right_child(root_version))
          count += size(child, root_version)
        end
        count
      end

      ##
      # Perform an in-order traversal of the tree, yielding for each node in the tree
      #
      def traverse(node, query_version, &block)
        left_child = node.get_left_child(query_version)
        Algorithms.traverse(left_child, query_version, &block) if left_child
        yield node
        right_child = node.get_right_child(query_version)
        Algorithms.traverse(right_child, query_version, &block) if right_child
        nil
      end

      private

      # noinspection RubyScope
      def insert_with_options(root_node, root_version, item, user_options = {})
        default_options = { allow_replacement: false }
        options = default_options.merge(user_options)
        # Determine location of the new node relative to other nodes in the tree
        current_node = root_node
        parent_node = nil
        leaf_node = nil
        while current_node
          if current_node.value > item
            parent_node = current_node
            current_node = current_node.get_left_child(root_version)
          elsif current_node.value < item
            parent_node = current_node
            current_node = current_node.get_right_child(root_version)
          elsif options[:allow_replacement]
            leaf_node = PersistentTree::Node.with_no_mods(current_node.get_left_child(root_version),
                                                          current_node.get_right_child(root_version),
                                                          item)
            break
          else
            raise KeyError, 'Item already exists in latest version of tree'
          end
        end

        # Create a new leaf node
        leaf_node ||= PersistentTree::Node.with_no_mods(nil, nil, item)

        # Update sub-tree pointers in ancestors, going all the way back up to the root if necessary
        current_node = parent_node
        while current_node
          if current_node.value > item
            if current_node.modified?
              # Create a new parent node, which will be the leaf node on the next iteration
              leaf_node = PersistentTree::Node.with_no_mods(leaf_node,
                                                            current_node.get_right_child(root_version),
                                                            current_node.value)
              current_node = current_node.parent
            else
              current_node.mod_left_child(leaf_node, root_version + 1)
              return root_node
            end
          elsif current_node.modified?
            # Create a new parent node, which will be the leaf node on the next iteration
            leaf_node = PersistentTree::Node.with_no_mods(current_node.get_left_child(root_version),
                                                          leaf_node,
                                                          current_node.value)
            current_node = current_node.parent
          else
            current_node.mod_right_child(leaf_node, root_version + 1)
            return root_node
          end
        end

        # When the tree has been modified up to the root, as is the case when the loop above terminates naturally,
        # leaf_node will contain a reference to the new root node
        leaf_node
      end
    end
  end
end
