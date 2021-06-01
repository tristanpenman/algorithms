# frozen_string_literal: true

require_relative 'printer'

module PGM
  #
  # Represents a tabular Conditional Probability Distribution (CPD)
  #
  # The CPD defines the probabilities of particular outcomes for a variable, given observations
  # of the variables that it is dependent on (i.e. the evidence).
  #
  # For example, assuming the variable 'Cancer' is dependent on 'Pollution' and 'Smoker', each of
  # which have two outcomes. Then if 'Cancer' also has two outcomes, then the string representation
  # of the table could look like this:
  #
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Smoker    | Smoker_0    | Smoker_0    | Smoker_1    | Smoker_1    |
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Pollution | Pollution_0 | Pollution_1 | Pollution_0 | Pollution_1 |
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Cancer_0  | 0.03        | 0.05        | 0.001       | 0.02        |
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Cancer_1  | 0.97        | 0.95        | 0.999       | 0.98        |
  #   +-----------+-------------+-------------+-------------+-------------+
  #
  class TabularCPD
    attr_reader :evidence, :variable

    def initialize(variable, evidence, data)
      expected_rows = variable.cardinality
      raise ArgumentError, "wrong number of rows; expected #{expected_rows}" unless data.length == expected_rows

      expected_cols = combinations(evidence)
      data.each.with_index do |row, index|
        raise ArgumentError, "wrong number of columns in row #{index}; expected #{expected_cols}" \
          unless row.length == expected_cols
      end

      @variable = variable
      @evidence = evidence
      @data = data
    end

    def to_s
      PGM::Printer.print(header(@evidence) + body(@data, @variable.label))
    end

    private

    def body(data, label)
      data.map.with_index do |row, index|
        row.map(&:to_s).unshift("#{label}_#{index}")
      end
    end

    def combinations(variables)
      variables.map(&:cardinality).inject(:*) || 1
    end

    def header(parents, cycles = 1)
      head, *tail = parents

      # base case
      return [] unless head

      # generate a row of data, including a label and a sequence of observations
      repeats = combinations(tail)
      cardinality = head.cardinality
      label = head.label
      row = [label] + sequence(cardinality, label, repeats) * cycles

      # recurse and prepend row
      header(tail, cycles * cardinality).unshift(row)
    end

    def sequence(cardinality, label, repeats)
      (0...cardinality).flat_map do |index|
        Array.new(repeats, "#{label}_#{index}")
      end
    end
  end
end
