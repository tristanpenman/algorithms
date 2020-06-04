require 'simplecov'
SimpleCov.start

require 'rspec'
require_relative '../lib/persistent_tree'

RSpec.configure do |config|
  config.expect_with :rspec do |c|
    c.syntax = :expect
  end
end
