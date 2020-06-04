require_relative 'tree'

module PersistentTree
  class MapEnumeratorDecorator
    def enumerator(subject)
      @subject = subject
    end

    def respond_to_missing?(method_name, *args)
      @subject.respond_to?(method_name) || super
    end

    def method_missing(method_name, *args, &block)
      if @subject.respond_to? method_name
        @subject.send method_name, *args, &block
      else
        super
      end
    end
  end

  # Read only view of a Map at a given version
  class MapView
    include Enumerable

    # Internal data structure used to store key-value pairs
    Entry = Struct.new(:key, :value)
    private_constant :Entry

    # Allow Entry objects to be ordered by their key or by comparing their key to other values
    class Entry
      include Comparable

      def <=>(other)
        if other.is_a? Entry
          key <=> other.key
        else
          key <=> other
        end
      end
    end

    def initialize(tree, version, default_value, default_proc)
      @tree = tree
      @version = version
      @default_value = default_value
      @default_proc = default_proc
    end

    def [](key)
      found, value = fetch_without_default(key)
      if found
        value
      elsif @default_proc
        @default_proc.call(self, key)
      else
        @default_value
      end
    end

    def each
      if block_given?
        @tree.each(@version) do |item|
          yield item.key, item.value
        end
        self
      else
        to_enum
      end
    end

    def each_key(&block)
      if block_given?
        keys.each(&block)
        self
      else
        keys.each
      end
    end

    def fetch(*args)
      raise ArgumentError, "wrong number of arguments (#{args.length} for 1..2)" unless args.length.between?(1, 2)

      warn('block supersedes default value argument') if args.length == 2 && block_given?

      key = args[0]
      found, value = fetch_without_default(key)
      if found
        value
      elsif block_given?
        yield key
      elsif args.length == 2
        args[1]
      else
        raise KeyError, "key not found: #{key}"
      end
    end

    def flatten(*args)
      # avoid unnecessary work if possible
      return flatten_key_value_pairs if args.empty?

      raise ArgumentError, "wrong number of arguments (#{args.length} for 0..1)" if args.length > 1

      raise TypeError, "no implicit conversion of #{args[0].class.name} into Integer" \
        unless args[0].respond_to?(:to_int)

      level = args[0].to_int
      return to_a if level.zero? # Rely on implicit array conversion behaviour

      result = flatten_key_value_pairs # Otherwise, flatten recursively
      if (level - 1).positive?
        result.flatten!(level - 1)
      elsif level.negative?
        result.flatten!
      end
      result
    end

    def keys
      @tree.map(@version, &:key)
    end

    def include?(key)
      @tree.include?(@version, key)
    end

    def select
      if block_given?
        new_map = Map.new
        @tree.each(@version) do |item|
          new_map.store item.key, item.value if yield item.key, item.value
        end
        new_map
      else
        to_enum(:select)
      end
    end

    def size
      @tree.size(@version)
    end

    alias each_pair each
    alias has_key? include?
    alias key? include?
    alias member? include?

    private

    def fetch_without_default(key)
      [true, @tree.find(@version, key).value]
    rescue KeyError
      [false, nil]
    end

    def flatten_key_value_pairs
      result = Array.new(size * 2)
      index = 0
      each do |key, value|
        result[index] = key
        result[index + 1] = value
        index += 2
      end
      result
    end
  end

  class Map < MapView
    attr_reader :default_proc

    def initialize(*args, &block)
      default_proc = nil
      default_value = nil
      if block_given?
        raise ArgumentError, "wrong number of arguments (#{args.length} for 0)" unless args.empty?

        if block.is_a? Proc
          default_proc = block
        else
          default_proc = Proc.new
        end

        raise TypeError, "default_proc takes two arguments (2 for #{default_proc.arity})" \
          if default_proc.lambda? && default_proc.arity != 2

      elsif args.length == 1
        default_value = args[0]
      elsif !args.empty?
        raise ArgumentError, "wrong number of arguments (#{args.length} for 0..1)"
      end
      super(PersistentTree::Tree.new, 0, default_value, default_proc)
    end

    def []=(key, value)
      store(key, value)
    end

    def default(key = nil)
      if @default_value
        @default_value
      elsif @default_proc && key
        @default_proc.call(self, key)
      end
    end

    def default=(value)
      @default_proc = nil
      @default_value = value
    end

    def default_proc=(proc)
      raise TypeError, "wrong default_proc type #{proc.class.name} (expected Proc)" \
        if proc.nil? || !proc.respond_to?(:to_proc)

      default_proc = proc.to_proc
      unless default_proc.is_a?(Proc)
        raise TypeError,
              "can't convert #{proc.class.name} to Proc (#{proc.class.name}#to_proc gives #{default_proc.class.name})"
      end

      raise TypeError, "default_proc takes two arguments (2 for #{default_proc.arity})" \
        if default_proc.lambda? && default_proc.arity != 2

      @default_value = nil
      @default_proc = default_proc
    end

    def store(key, value)
      if key.is_a?(String) && !key.frozen?
        key_to_store = key.freeze
      else
        key_to_store = key
      end
      @tree.insert_or_replace(Entry.new(key_to_store, value)).value
      @version = @tree.version
      value
    end

    def version(*args)
      if args.length == 1
        requested_version = args[0]
        raise TypeError, 'requested version should be an integer' unless requested_version.is_a? Integer

        raise RangeError, "requested version (#{requested_version}) exceeds current version (#{@tree.version})" \
          if requested_version > @tree.version

        MapView.new(@tree, requested_version, @default_value, @default_proc)
      elsif args.empty?
        @tree.version
      else
        raise ArgumentError, "wrong number of arguments(#{args.length} for 0..1)"
      end
    end
  end
end
