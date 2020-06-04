Gem::Specification.new do |s|
  s.name        = 'persistent_tree'
  s.version     = '0.0.0'
  s.date        = '2015-03-25'
  s.summary     = 'Partially Persistent Tree for Ruby'
  s.description = "Partially Persistent Tree, based on Okasaki's thesis 'Purely Functional Data Structures'"
  s.authors     = ['Tristan Penman']
  s.email       = 'tristan@tristanpenman.com'
  s.files       = ['lib/persistent_tree.rb', 'lib/persistent_tree/map.rb']
  s.homepage    = 'http://rubygems.org/gems/persistent_tree'
  s.license     = 'MIT'

  s.add_dependency 'rake'
  s.add_dependency 'rubocop'
  s.add_dependency 'rspec'
  s.add_dependency 'simplecov'
end
