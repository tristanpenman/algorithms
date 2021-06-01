# frozen_string_literal: true

require 'set'

module PGM
  #
  # Represents an individual node, or random variable, in a model
  #
  # Example string representation for a node with two incoming edges, and no outgoing edges:
  #
  #   { Pollution, Smoker } -> Cancer -> { }
  #
  # Or for nodes with no incoming edges and only outgoing edges:
  #
  #   { } -> Pollution -> { Cancer }
  #   { } -> Smoker -> { Cancer }
  #
  class Node
    attr_accessor :cpd
    attr_reader :label, :incoming_edges, :outgoing_edges

    def initialize(label)
      @label = label
      @incoming_edges = Set.new
      @outgoing_edges = Set.new
    end

    def to_s
      [write_set(@incoming_edges), @label, write_set(@outgoing_edges)].join(' -> ')
    end

    def write_set(set)
      set.empty? ? '{ }' : "{ #{set.to_a.sort.join(', ')} }"
    end
  end
end
