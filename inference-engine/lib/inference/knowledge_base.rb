require_relative './rule.rb'

module Inference
  class KnowledgeBase
    attr_reader :facts, :rules

    def initialize
      @facts = []
      @rules = []
    end

    def tell(statement)
      parts = statement.split('=>')
      case parts.length
      when 1
        @facts << parts[0].strip
      when 2
        left = parts[0].split('&').map(&:strip)
        right = parts[1].strip
        @rules << Rule.new(left, right)
      else
        raise 'Invalid statement'
      end
      nil
    end
  end
end
