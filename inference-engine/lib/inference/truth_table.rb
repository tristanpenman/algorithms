require 'set'

module Inference
  class TruthTable
    attr_reader :models

    def initialize
      @models = []
      @symbols = []
    end

    # TODO: pseudocode from AIMA suggests that query could actually be a rule, or even an entire knowledge-base
    def ask(kb, query)
      symbols = enumerate(kb, query)

      check(kb, query, symbols, {})
    end

    private

    def check(kb, query, symbols, model)
      if symbols.empty?
        if valid?(kb, model)
          # model must also be valid for the given query
          return false unless model[query]

          @models << model
        end

        return true
      end

      p, *rest = symbols

      check(kb, query, rest, union(model, p, true)) && check(kb, query, rest, union(model, p, false))
    end

    def enumerate(kb, query)
      s = ::Set.new(kb.facts)
      s << query
      kb.rules.each do |rule|
        s += rule.antecedents
        s << rule.consequent
      end
      s.to_a
    end

    def union(model, symbol, value)
      m = model.dup
      m[symbol] = value
      m
    end

    def valid?(kb, model)
      # model must account for stated facts
      return false unless kb.facts.all? { |fact| model[fact] }

      kb.rules.all? do |rule|
        # check model using implication
        rule.antecedents.all? { |antecedent| model[antecedent] } ? model[rule.consequent] : true
      end
    end
  end
end
