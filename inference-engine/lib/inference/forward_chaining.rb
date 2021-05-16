module Inference
  class ForwardChaining
    attr_reader :working_set

    def initialize
      @rules = []
      @working_set = []
    end

    def ask(kb, query)
      @rules = kb.rules.clone
      @working_set = kb.facts.clone

      iterate(query)
    end

    private

    def iterate(query)
      return true if @working_set.include?(query)

      @rules.each do |rule|
        if rule.antecedents.all? { |antecedent| @working_set.include?(antecedent) }
          @working_set << rule.consequent
          @rules.delete(rule)
          return true if iterate(query)
        end
      end
    end
  end
end
