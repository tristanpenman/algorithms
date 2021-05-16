module Inference
  class BackwardChaining
    attr_reader :working_set

    def initialize
      @working_set = []
    end

    def ask(kb, query)
      @working_set = kb.facts.clone

      traverse(kb.rules, [query])
    end

    private

    def traverse(rules, queries)
      queries.all? do |query|
        @working_set.include?(query) || rules.one? do |rule|
          if rule.consequent == query && traverse(rules, rule.antecedents)
            @working_set << query
            true
          end
        end
      end
    end
  end
end
