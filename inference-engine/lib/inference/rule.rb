module Inference
  class Rule
    attr_reader :antecedents, :consequent

    def initialize(antecedents, consequent)
      @antecedents = antecedents
      @consequent = consequent
    end

    def to_s
      "#{@antecedents.join(' & ')} => #{@consequent}"
    end
  end
end
