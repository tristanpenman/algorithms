module Inference
  class BackwardChaining
    attr_reader :working_set

    def initialize
      @working_set = []
    end

    def ask(kb, query)
      @working_set = []

      traverse(kb.facts, kb.rules, [query])
    end

    private

    def traverse(facts, rules, queries)
      queries.all? do |query|
        if @working_set.include?(query)
          true
        elsif facts.include?(query)
          @working_set << query
          true
        else
          rules.one? do |rule|
            if rule.consequent == query && traverse(facts, rules - [rule], rule.antecedents)
              @working_set << query
              true
            end
          end
        end
      end
    end
  end
end
