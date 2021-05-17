require_relative 'backward_chaining'
require_relative 'forward_chaining'
require_relative 'knowledge_base'
require_relative 'truth_table'

module Inference
  class Engine
    ALGORITHMS = {
      BC: BackwardChaining,
      FC: ForwardChaining,
      TT: TruthTable
    }.freeze

    def initialize(alg)
      @alg = algorithm(alg)
      @kb = KnowledgeBase.new
      @mode = nil
    end

    def parse(line)
      output = []
      statements(line).each do |statement|
        case statement.upcase
        when 'ASK'
          @mode = :ask
        when 'FACTS'
          @kb.facts
        when 'RULES'
          @kb.rules
        when 'TELL'
          @mode = :tell
        else
          output << exec(statement)
        end
      end
      output
    end

    private

    def algorithm(alg)
      cls = ALGORITHMS[alg&.to_sym]
      raise 'Missing or invalid algorithm.' unless cls
      cls.new
    end

    def ask(statement)
      return 'NO' unless @alg.ask(@kb, statement)

      if @alg.respond_to?(:working_set)
        "YES: #{@alg.working_set.join(', ')}"
      else
        "YES: #{@alg.models.length}"
      end
    end

    def exec(statement)
      case @mode
      when :ask
        ask(statement)
      when :tell
        @kb.tell(statement)
      else
        raise 'Statements can only be executed once a mode (ASK or TELL) has been activated.'
      end
    end

    def statements(line)
      line.strip.split(
        /\s*[;]\s*  # semicolon, optionally surrounded by whitespace
        |           # or
        \s{2,}      # two or more whitespace characters
        |           # or
        [\r\n]+     # any number of newline characters
        /x)
    end
  end
end
