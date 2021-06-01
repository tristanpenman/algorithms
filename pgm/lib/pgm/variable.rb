# frozen_string_literal: true

module PGM
  #
  # Represents a variable in a tabular CPD
  #
  Variable = Struct.new(:label, :cardinality)
end
