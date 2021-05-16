require_relative './backward_chaining'
require_relative './forward_chaining'
require_relative './knowledge_base'
require_relative './truth_tables'

module Inference
  class Engine
    ALGS = {
      BC: BackwardChaining,
      FC: ForwardChaining,
      TT: TruthTables
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
      cls = ALGS[alg&.to_sym]
      raise 'Missing or invalid algorithm.' unless cls
      cls.new
    end

    def ask(statement)
      if @alg.ask(@kb, statement)
        "YES: #{@alg.working_set.join(', ')}"
      else
        'NO'
      end
    end

    def exec(statement)
      case @mode
      when :ask
        ask(statement)
      when :tell
        @kb.tell(statement)
      else
        raise 'Invalid mode. Statements can only be executed once a mode has been selected.'
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
