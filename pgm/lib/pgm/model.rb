# frozen_string_literal: true

require_relative 'node'

module PGM
  #
  # Represents a Probabilistic Graphical Model (PGM)
  #
  class Model
    class ModelError < StandardError; end

    #
    # Edges between individual nodes, sorted by label of the outgoing edge, e.g:
    #
    #   Pollution -> Cancer
    #   Smoker -> Cancer
    #
    attr_reader :edges

    #
    # Lookup table of labelled nodes, each associated with a set of labels for all incoming edges, e.g:
    #
    #   { Pollution, Smoker } -> Cancer -> { }
    #   { } -> Pollution -> { Cancer }
    #   { } -> Smoker -> { Cancer }
    #
    attr_reader :nodes

    # most recent error after calling `valid?`
    attr_reader :error

    def initialize(*edges)
      @edges = sort_edges(edges)
      @nodes = reduce_edges(edges)
    end

    def add_cpd(cpd)
      node = @nodes[cpd.variable.label]
      raise ArgumentError, "node does not exist for label #{node.label}" unless node

      check_cpd_evidence!(cpd.evidence.map(&:label), node.incoming_edges)
      node.cpd = cpd
    end

    def to_s
      ['Edges:', edges_to_s, '', 'Nodes:', nodes_to_s, '', 'Valid:', valid?, ''].join("\n")
    end

    def validate!
      @nodes.each_value do |node|
        raise ModelError, "node '#{node.label}' does not have a CPD" unless node.cpd
      end

      # validate cardinality between nodes for each edge
      @edges.each do |edge|
        validate_cardinality!(@nodes[edge.to], @nodes[edge.from])
      end
    end

    def valid?
      @error = nil
      validate!
      true
    rescue ModelError => e
      @error = e
      false
    end

    private

    def check_cpd_evidence!(cpd_evidence, node_dependencies)
      cpd_evidence.each do |evidence|
        raise ArgumentError, "node is missing dependency for CPD evidence '#{evidence}'" \
          unless node_dependencies.include?(evidence)
      end

      node_dependencies.each do |dependency|
        raise ArgumentError, "CPD is missing evidence for node dependency '#{dependency}'" \
          unless cpd_evidence.include?(dependency)
      end
    end

    def edges_to_s
      @edges.map(&:to_s).join("\n")
    end

    def nodes_to_s
      @nodes.keys.sort.map { |key| @nodes[key].to_s }.join("\n")
    end

    def reduce_edges(edges)
      edges.each_with_object({}) do |edge, reduced|
        # create node for incoming edge
        (reduced[edge.to] ||= PGM::Node.new(edge.to)).incoming_edges.add(edge.from)

        # create node for outgoing edge
        (reduced[edge.from] ||= PGM::Node.new(edge.from)).outgoing_edges.add(edge.to)
      end
    end

    def sort_edges(edges)
      edges.sort_by(&:from)
    end

    def validate_cardinality!(to, from)
      expected = to.cpd.evidence.find { |ev| ev.label == from.label }.cardinality
      actual = from.cpd.variable.cardinality

      raise ModelError, "cardinality mismatch in CPDs of '#{from.label}' (#{actual}) and '#{to.label}' (#{expected})" \
        unless expected == actual
    end
  end
end
