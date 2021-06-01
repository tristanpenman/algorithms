# frozen_string_literal: true

module PGM
  #
  # Represents a directed edge between two nodes in a PGM
  #
  Edge = Struct.new(:from, :to) do
    def to_s
      "#{from} -> #{to}"
    end
  end
end
