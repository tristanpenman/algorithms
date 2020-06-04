module PersistentTree
  ##
  # Represents a partially-persistent tree
  #
  # This class behaves somewhat like an Aggregate (see Evans' Domain-Driven Design) in that the individual items
  # stored in the tree should only be accessed via its public interface.
  #
  # The actual classes and algorithms used to implement the tree can be found outside of this class. Nodes in the tree
  # are represented by instances of the Node class, which attempts to maintain various tree invariants. The algorithms
  # used to insert, remove and find items in the tree are implemented in Algorithms module. These implementation
  # details have been made public so that they can be tested more effectively, while allowing the Tree class to
  # provide a high level of encapsulation.
  #
  class Tree
    ##
    # Create an empty tree
    #
    def initialize
      @versions = []
    end

    ##
    # Yield for each item in the tree at a given version, as per an in-order traversal
    #
    def each(version)
      # Validate version number
      version = validate_non_negative_integer(version)
      raise RangeError, 'Requested version does not exist' if version > @versions.size

      # Early exit if version zero is requested
      if version.positive?
        Algorithms.traverse(@versions[version - 1], version) do |node|
          yield node.value
        end
      end
      nil
    end

    ##
    # Find an item in the tree at a given version
    #
    # Returns the stored value if found; raises KeyError otherwise
    #
    def find(version, key)
      # Validate version number
      version = validate_non_negative_integer(version)
      raise RangeError, 'Requested version does not exist' if version > @versions.size

      # Early exit if version zero is requested
      raise KeyError unless version.positive?

      Algorithms.find(@versions[version - 1], version, key).value
    end

    def include?(version, key)
      # Validate version number
      version = validate_non_negative_integer(version)
      raise RangeError, 'Requested version does not exist' if version > @versions.size

      # Early exit if version zero is requested
      return false if version.zero?

      Algorithms.include?(@versions[version - 1], version, key)
    end

    ##
    # Insert an item into the tree, but only if it is not equal to an existing item in the tree
    #
    def insert(item)
      @versions << if @versions.empty?
                     Node.with_no_mods(nil, nil, item)
                   else
                     Algorithms.insert(@versions[-1], @versions.size, item)
                   end
      item
    end

    ##
    # Insert an item into the tree, replacing an existing item if there is an equal item in the tree
    #
    def insert_or_replace(item)
      @versions << if @versions.empty?
                     Node.with_no_mods(nil, nil, item)
                   else
                     Algorithms.insert!(@versions[-1], @versions.size, item)
                   end
      item
    end

    def map(version)
      output = []
      each(version) { |value| output << yield(value) }
      output
    end

    def size(version)
      # Validate version number
      version = validate_non_negative_integer(version)
      raise RangeError, 'Requested version does not exist' if version > @versions.size

      # Early exit if version zero is requested
      if version.zero?
        0
      else
        Algorithms.size(@versions[version - 1], version)
      end
    end

    def version
      @versions.size
    end

    private_methods

    def validate_non_negative_integer(value)
      raise TypeError, 'Expected non-negative integer but got: nil' unless value

      value_as_str = value.to_s
      raise TypeError, "Expected value that could be parsed as an integer but got value of type: #{value.class}" \
        unless /\A-?\d+\z/.match?(value_as_str)

      value_as_int = value_as_str.to_i
      raise RangeError, "Expected non-negative integer, but got: #{value_as_int}" \
        unless value_as_int >= 0

      value_as_int
    end
  end
end
