module PersistentTree
  ##
  # Represents a node in a partially-persistent data structure
  #
  class Node
    # Exception for cases where it is not permitted to call a particular method
    class InvariantViolationError < RuntimeError
    end

    # Internal data structure used to track node modifications
    Mod = Struct.new(:left, :ptr, :version)
    private_constant :Mod

    # Default constructor is private to enforce usage of factory methods
    private_class_method :new

    # value can be read, but not modified
    attr_reader :value

    # parent should only be modified by instances of this class

    protected

    attr_writer :parent

    public

    attr_reader :parent

    ##
    # Private constructor to initialise a Node
    #
    def initialize(left_child, right_child, value, mod)
      @parent = nil
      if left_child
        raise ArgumentError, 'Left child would have value greater than parent node' \
          if left_child.value > value

        @left_child = left_child
        left_child.parent = self
      else
        @left_child = nil
      end

      if right_child
        raise ArgumentError, 'Right child would have value less than parent node' \
          if right_child.value < value

        @right_child = right_child
        right_child.parent = self
      else
        @right_child = nil
      end

      @value = value
      if mod&.ptr
        raise ArgumentError, 'Modified left child would have value greater than parent node' \
          if mod.left && (mod.ptr.value > value)

        raise ArgumentError, 'Modified right child would have value less than parent node' \
          if !mod.left && (mod.ptr.value < value)

        mod.ptr.parent = self
      end
      @mod = mod
    end

    ##
    # Construct a Node with no modifications
    #
    def self.with_no_mods(left_child, right_child, value)
      new(left_child, right_child, value, nil)
    end

    ##
    # Construct an internal Node with an existing modification to the left sub-tree
    #
    def self.with_left_mod(left_child, right_child, value, left_child_mod, mod_version)
      new(left_child, right_child, value, Mod.new(true, left_child_mod, mod_version))
    end

    ##
    # Construct an internal Node with an existing modification to the right sub-tree
    #
    def self.with_right_mod(left_child, right_child, value, right_child_mod, mod_version)
      new(left_child, right_child, value, Mod.new(false, right_child_mod, mod_version))
    end

    ##
    # Return a Node's left child
    #
    # If the Node has been modified *and* the requested version is greater than or equal to the version in which
    # it was modified, then the updated child Node will be returned.
    #
    # In all other cases, the original left child Node will be returned.
    #
    def get_left_child(version)
      if @mod && (@mod.version <= version) && @mod.left
        @mod.ptr
      else
        @left_child
      end
    end

    ##
    # Return a Node's right child
    #
    # If the Node has been modified *and* the requested version is greater than or equal to the version in which
    # it was modified, then the updated child Node will be returned.
    #
    # In all other cases, the original right child Node will be returned.
    #
    def get_right_child(version)
      if @mod && (@mod.version <= version) && !@mod.left
        @mod.ptr
      else
        @right_child
      end
    end

    def modified?
      @mod != nil
    end

    ##
    # Update a Node with a new left child at a given version
    #
    # A Node can only have one modification, so calling this method on an already-modified Node will raise an error
    #
    def mod_left_child(child, version)
      raise ArgumentError, 'Modified left child would have value greater than parent node' if child.value > @value

      raise InvariantViolationError, 'Node already contains a modification' if @mod

      @mod = Mod.new(true, child, version)
      child.parent = self
    end

    ##
    # Update a Node with a new right child at a given version
    #
    # A Node can only have one modification, so calling this method on an already-modified Node will raise an error
    #
    def mod_right_child(child, version)
      raise ArgumentError, 'Modified right child would have value less than parent node' if child.value < @value

      raise InvariantViolationError, 'Node already contains a modification' if @mod

      @mod = Mod.new(false, child, version)
      child.parent = self
    end
  end
end
