require_relative '../spec_helper'

describe PersistentTree::Map do
  let!(:map) { described_class.new }

  shared_examples 'enumeration of key/value pairs' do |method_name|
    let!(:lower_limit) { 1 }
    let!(:upper_limit) { 10 }
    let!(:range) { 1..upper_limit }

    before do
      range.each do |value|
        map["key-#{value}"] = value
      end
    end

    context 'when a block is provided' do
      it 'returns itself once iteration is complete' do
        (lower_limit..upper_limit).each do |value|
          view = map.version(value)
          expect(view.send(method_name) { |_| }).to be(view)
        end
      end

      it 'yields correct array of values at subsequent versions' do
        expected = []
        (lower_limit..upper_limit).each do |value|
          expected << ["key-#{value}", value]
          # Sort expected values to ensure they follow a lexical ordering
          expected.sort!
          expect { |b| map.version(value).send(method_name, &b) }.to yield_successive_args(*expected)
        end
      end
    end

    context 'when a block is not provided' do
      it 'returns an enumerator' do
        (lower_limit..upper_limit).each do |value|
          expect(map.version(value).send(method_name)).to be_a(Enumerator)
        end
      end

      it 'returned enumerator yields the correct array of values at subsequent versions' do
        expected = []
        (lower_limit..upper_limit).each do |value|
          expected << ["key-#{value}", value]
          # Sort expected values to ensure they follow a lexical ordering
          expected.sort!
          enumerator = map.version(value).send(method_name)
          expect { |b| enumerator.each(&b) }.to yield_successive_args(*expected)
        end
      end

      it 'returned enumerator returns an equivalent iterator when each is called without a block' do
        (lower_limit..upper_limit).each do |value|
          enumerator = map.version(value).send(method_name)
          expect(enumerator.each.to_a).to match(enumerator.to_a)
        end
      end
    end
  end

  shared_examples 'test for key membership' do |method_name|
    let!(:lower_limit) { 1 }
    let!(:upper_limit) { 10 }
    let!(:range) { 1..upper_limit }

    before do
      range.each do |value|
        map["key-#{value}"] = value
      end
    end

    it 'returns false for all keys that do yet exist at a given version' do
      (lower_limit..(upper_limit - 1)).each do |version|
        ((version + 1)..upper_limit).each do |key|
          expect(map.version(version).send(method_name, "key-#{key}")).to be_falsey
        end
      end
    end

    it 'returns false for various keys that should not exist' do
      (lower_limit..upper_limit).each do |version|
        ['', 'test', '123', '\n', '--', 'key-', '??*'].each do |key|
          expect(map.version(version).send(method_name, "key-#{key}")).to be_falsey
        end
      end
    end

    it 'returns true for all keys that exist a given version' do
      (lower_limit..upper_limit).each do |version|
        (lower_limit..version).each do |key|
          expect(map.version(version).send(method_name, "key-#{key}")).to be_truthy
        end
      end
    end

    it 'returns true for all keys that exist at the latest version' do
      (lower_limit..upper_limit).each do |key|
        expect(map.send(method_name, "key-#{key}")).to be_truthy
      end
    end
  end

  describe '.new' do
    context 'with a map-level default value lambda with an incorrect number of arguments' do
      it 'should return cause an error to be raised' do
        expect { described_class.new(&->(_) { 'test' }) }.to raise_error(TypeError)
      end
    end
  end

  describe '#[]' do
    context 'when called on a map containing two associations, but no map-level default value' do
      before do
        map.store('a', '1')
        map.store('b', '4')
      end

      context 'with no arguments' do
        it 'should raise an error' do
          expect { map[] }.to raise_error(ArgumentError)
        end
      end

      context 'with one argument, for a key that exists' do
        it 'should retrieve the value previously associated with a key' do
          expect(map['a']).to eq('1')
          expect(map['b']).to eq('4')
        end
      end

      context 'with one argument, for a key that does not exist' do
        it 'should return nil' do
          expect(map['c']).to be_nil
        end
      end
    end

    context 'when called on a map containing two associations with a map-level default value' do
      let!(:map) { described_class.new('2') }
      before do
        map.store('a', '1')
        map.store('b', '4')
      end

      context 'with one argument, for a key that does not exist' do
        it 'should return the Map-level default value' do
          expect(map['c']).to eq('2')
        end
      end
    end

    context 'when called on an empty map' do
      context 'with a map-level default value lambda with the correct number of arguments' do
        let!(:value) { 'test' }
        let!(:map) { described_class.new(&->(_, _) { value }) }

        it 'should return the value returned by the lambda' do
          expect(map['c']).to be(value)
        end
      end

      context 'with a map-level default value block with the correct number of arguments' do
        let!(:value) { 'test' }
        let!(:map) { described_class.new { |_, _| value } }

        it 'should return the value generated by the block' do
          expect(map['c']).to eq('test')
        end
      end

      context 'with a map-level default value block with the wrong number of arguments' do
        let!(:value) { 'test' }
        let!(:map) { described_class.new { value } }

        it 'should return the value generated by the block' do
          expect(map['c']).to be(value)
        end
      end

      context 'with a map-level default value that is actually a proc' do
        let!(:value) { 'test' }
        let!(:map) { described_class.new ->(_, _) { value } }

        it 'should return the proc instead of the value that it would generate if called' do
          expect(map['c']).to be_a(Proc)
        end
      end
    end

    context 'when called on a map containing two associations with a map-level default value block' do
      let!(:map) { described_class.new { |_, key| "#{key}-value" } }

      before do
        map.store('a', '1')
        map.store('b', '4')
      end

      context 'with one argument, for a key that does not exist' do
        it 'should return the value generated by the Map-level default value block' do
          expect(map['c']).to eq('c-value')
        end
      end
    end
  end

  describe '#[]=' do
    it 'should allow values to be associated with new keys' do
      expect((map['a'] = '1').to_s).to eq('1')
      expect((map['b'] = '4').to_s).to eq('4')
    end

    it 'should allow values to be replace those associated with existing keys' do
      expect((map['a'] = '1').to_s).to eq('1')
      expect((map['a'] = '2').to_s).to eq('2')
    end

    it 'should freeze unfrozen strings used as keys' do
      a = 'a'.dup
      expect(a.frozen?).to be_falsey
      expect((map[a] = '1').to_s).to eq('1')
      expect(map.keys[0].frozen?).to be_truthy
    end
  end

  describe '#default' do
    context 'on a map without a default value' do
      context 'when called without a key' do
        it 'returns nil' do
          expect(map.default).to be_nil
        end
      end

      context 'when called with a key' do
        it 'returns nil' do
          expect(map.default('Key')).to be_nil
        end
      end
    end

    context 'on a map with a default value' do
      let!(:map) { described_class.new 'Value' }

      context 'when called without a key' do
        it 'returns the default value' do
          expect(map.default).to eq('Value')
        end
      end

      context 'when called with a key' do
        it 'returns the default value' do
          expect(map.default('Key')).to eq('Value')
        end
      end
    end

    context 'on a map with a default proc' do
      let!(:map) { described_class.new { |_, key| "Value of #{key}" } }

      context 'when called without a key' do
        it 'returns nil' do
          expect(map.default).to be_nil
        end
      end

      context 'when called with a key' do
        it 'returns the value produced by the proc' do
          expect(map.default('Key1')).to eq('Value of Key1')
          expect(map.default('Key2')).to eq('Value of Key2')
        end
      end
    end
  end

  describe '#default=' do
    context 'when called with nil' do
      before { map.default = nil }

      it 'subsequent calls to Map#default without a key return nil' do
        expect(map.default).to be_nil
      end

      it 'subsequent calls to Map#default with a key return nil' do
        expect(map.default('Key')).to be_nil
      end
    end

    context 'when called with a string value' do
      before { map.default = 'Value' }

      it 'subsequent calls to Map#default without a key return the same string value' do
        expect(map.default).to eq('Value')
      end

      it 'subsequent calls to Map#default with a key return the same string value' do
        expect(map.default('Key')).to eq('Value')
      end
    end

    context 'when called with a Proc' do
      before do
        map.default = proc do |hash, key|
          hash[key] = key + key
        end
      end

      it 'subsequent calls to Map#default without a key return the original Proc object' do
        expect(map.default).to be_a(Proc)
      end

      it 'subsequent calls to Map#default with a key return the original Proc object' do
        expect(map.default('Key')).to be_a(Proc)
      end
    end
  end

  describe '#default_proc' do
    context 'called on a map without a default value or default proc' do
      it 'returns nil' do
        expect(map.default_proc).to be_nil
      end
    end

    context 'called on a map with a default value' do
      let!(:map) { described_class.new 'Value' }

      it 'returns nil' do
        expect(map.default_proc).to be_nil
      end
    end

    context 'called on a map with a default proc' do
      let!(:map) { described_class.new { |_, k| k } }

      it 'returns a Proc' do
        expect(map.default_proc).to be_a(Proc)
      end
    end

    context 'called on a map with a default proc that was replaced by a default value' do
      let!(:map) { described_class.new { |_, k| k } }

      before { map.default = 'Value' }

      it 'returns nil' do
        expect(map.default_proc).to be_nil
      end
    end
  end

  describe '#default_proc=' do
    context 'called with a symbol, where to_proc returns a Proc' do
      let!(:receiver) { 'Test' }

      before { map.default_proc = :to_s }

      it 'subsequent calls to default_proc return a Proc that calls a method on its receiver' do
        expect(map.default_proc.call(receiver)).to eq('Test')
      end
    end

    context 'called with a value that does not support to_proc' do
      it 'should raise an exception' do
        expect { map.default_proc = 1 }.to raise_error(TypeError)
      end
    end

    context 'called with a value where to_proc returns a non-Proc object' do
      let!(:trojan) do
        value = 'Test'
        value.define_singleton_method(:to_proc) { 'Not a Proc' }
        value
      end

      it 'should raise an exception' do
        expect { map.default_proc = trojan }.to \
          raise_error(TypeError, 'can\'t convert String to Proc (String#to_proc gives String)')
      end
    end

    context 'called with a Proc' do
      let!(:test_proc) { proc { |_, _| 'test' } }

      it 'should return the Proc' do
        expect(map.default_proc = test_proc).to be(test_proc)
      end
    end

    context 'called with a lambda that expects two arguments' do
      let!(:test_lambda) { ->(_, _) { 'test' } }

      it 'should return the lambda' do
        expect(map.default_proc = test_lambda).to be(test_lambda)
      end
    end

    context 'called with a lambda that expects an incorrect number of arguments' do
      it 'should return cause an error to be raised' do
        expect { map.default_proc = ->(_) { 'test' } }.to raise_error(TypeError)
        expect { map.default_proc = ->(_, _, _) { 'test' } }.to raise_error(TypeError)
      end
    end
  end

  describe '#each' do
    include_examples 'enumeration of key/value pairs', 'each'
  end

  describe '#each_key' do
    let!(:lower_limit) { 1 }
    let!(:upper_limit) { 10 }
    let!(:range) { 1..upper_limit }

    before do
      range.each do |value|
        map["key-#{value}"] = value
      end
    end

    context 'when a block is provided' do
      it 'returns itself once iteration is complete' do
        (lower_limit..upper_limit).each do |value|
          view = map.version(value)
          expect(view.each_key { |_| }).to be(view)
        end
      end

      it 'yields correct array of values at subsequent versions' do
        expected = []
        (lower_limit..upper_limit).each do |value|
          expected << "key-#{value}"
          # Sort expected values to ensure they follow a lexical ordering
          expected.sort!
          expect { |b| map.version(value).each_key(&b) }.to yield_successive_args(*expected)
        end
      end
    end

    context 'when a block is not provided' do
      it 'returns an enumerator' do
        (lower_limit..upper_limit).each do |value|
          expect(map.version(value).each_key).to be_a(Enumerator)
        end
      end

      it 'returned enumerator yields the correct array of values at subsequent versions' do
        expected = []
        (lower_limit..upper_limit).each do |value|
          expected << "key-#{value}"
          # Sort expected values to ensure they follow a lexical ordering
          expected.sort!
          enumerator = map.version(value).each_key
          expect { |b| enumerator.each(&b) }.to yield_successive_args(*expected)
        end
      end

      it 'returned enumerator returns an equivalent iterator when each is called without a block' do
        (lower_limit..upper_limit).each do |value|
          enumerator = map.version(value).each_key
          expect(enumerator.each.to_a).to match(enumerator.to_a)
        end
      end
    end
  end

  describe '#each_pair' do
    include_examples 'enumeration of key/value pairs', 'each_pair'
  end

  describe '#fetch' do
    let(:block) { ->(k) { "#{k}-value" } }

    before do
      map.store('a', '1')
      map.store('b', '4')
    end

    context 'when a default value is provided' do
      it 'should retrieve the values previously associated with keys' do
        expect(map.fetch('a', '3')).to eq('1')
        expect(map.fetch('b', '3')).to eq('4')
      end

      it 'should return the default value when fetching a value for a non-existent key' do
        expect(map.fetch('c', '3')).to eq('3')
      end
    end

    context 'when a code block is provided' do
      it 'should retrieve the values previously associated with keys' do
        expect(map.fetch('a', &block)).to eq('1')
        expect(map.fetch('b', &block)).to eq('4')
      end

      it 'should defer to the block when fetching a value for a non-existent key' do
        expect(map.fetch('c', &block)).to eq('c-value')
      end
    end

    context 'when both a default value and a code block is provided' do
      before do
        @original_stderr = $stderr
        $stderr = StringIO.new
      end

      it 'should produce a warning about superseded default value argument' do
        map.fetch('a', '3', &block)
        $stderr.rewind
        expect($stderr.string.chomp).to eq('block supersedes default value argument')
      end

      it 'should retrieve the values previously associated with keys' do
        expect(map.fetch('a', '3', &block)).to eq('1')
        expect(map.fetch('b', '3', &block)).to eq('4')
      end

      it 'should defer to the block when fetching a value for a non-existent key' do
        expect(map.fetch('c', '3', &block)).to eq('c-value')
      end

      after do
        $stderr = @original_stderr
      end
    end

    context 'when neither a default value nor a code block is provided' do
      it 'should retrieve the values previously associated with keys' do
        expect(map.fetch('a')).to eq('1')
        expect(map.fetch('b')).to eq('4')
      end

      it 'should raise an error when fetching a value for a non-existent key' do
        expect { map.fetch('c') }.to raise_error(KeyError)
      end
    end
  end

  describe '#flatten' do
    before do
      map['a'] = [1, [2, 3]]
      map['b'] = 4
    end

    context 'called with the wrong number of arguments' do
      it 'raise an error' do
        expect { map.flatten(1, 2) }.to raise_error(ArgumentError)
      end
    end

    context 'called with a level argument that is negative' do
      it 'returns an array where each key-value pair has been recursively flattened as far as possible' do
        expect(map.flatten(-1)).to contain_exactly('a', 1, 2, 3, 'b', 4)
      end
    end

    context 'called with a level argument of zero' do
      it 'returns an array containing an entry for each key-value pair' do
        expect(map.flatten(0)).to contain_exactly(['a', [1, [2, 3]]], ['b', 4])
      end
    end

    context 'called with a level argument of one' do
      it 'returns an array where each key-value pair has been recursively flattened once' do
        expect(map.flatten(1)).to contain_exactly('a', [1, [2, 3]], 'b', 4)
      end
    end

    context 'called with a level argument of two' do
      it 'returns an array where each key-value pair has been recursively flattened twice' do
        expect(map.flatten(2)).to contain_exactly('a', 1, [2, 3], 'b', 4)
      end
    end

    context 'called with a level argument of three' do
      it 'returns an array where each key-value pair has been recursively flattened three times' do
        expect(map.flatten(3)).to contain_exactly('a', 1, 2, 3, 'b', 4)
      end
    end

    context 'called with an argument that cannot be converted to an Integer' do
      it 'raises an error' do
        expect { map.flatten('test') }.to raise_error(TypeError)
      end
    end

    context 'called without an argument' do
      it 'returns an array where each key-value pair has been recursively flattened once' do
        expect(map.flatten).to contain_exactly('a', [1, [2, 3]], 'b', 4)
      end
    end
  end

  describe '#has_key?' do
    include_examples 'test for key membership', 'has_key?'
  end

  describe '#include?' do
    include_examples 'test for key membership', 'include?'
  end

  describe '#key?' do
    include_examples 'test for key membership', 'key?'
  end

  describe '#keys' do
    context 'on a new map' do
      it 'should return an empty array' do
        expect(map.keys.size).to eq(0)
      end
    end

    context 'on a map containing two entries' do
      before do
        map.store('a', '1')
        map.store('b', '4')
      end

      it 'should return the keys belonging to those two entries' do
        keys = map.keys
        expect(keys.include?('a')).to be_truthy
        expect(keys.include?('b')).to be_truthy
      end
    end

    context 'on a map containing up to 10 items' do
      let!(:lower_limit) { 1 }
      let!(:upper_limit) { 10 }
      let!(:range) { 1..upper_limit }

      before do
        range.each do |value|
          map["key-#{value}"] = value
        end
      end

      it 'yields correct list of keys at subsequent versions' do
        (lower_limit..upper_limit).each do |outer|
          keys = map.version(outer).keys
          (lower_limit..outer).each do |inner|
            expect(keys.include?("key-#{inner}")).to be_truthy
          end
        end
      end
    end
  end

  describe '#member?' do
    include_examples 'test for key membership', 'member?'
  end

  describe '#select' do
    let!(:lower_limit) { 1 }
    let!(:upper_limit) { 10 }
    let!(:range) { 1..upper_limit }

    before do
      range.each do |value|
        map["key-#{value}"] = value
      end
    end

    context 'when a block is provided' do
      it 'returns a new map once iteration is complete' do
        (lower_limit..upper_limit).each do |value|
          view = map.version(value)
          expect(view.select { |_, _| true }).to be_a(PersistentTree::Map)
        end
      end

      it 'returns a new map with a version equal to the number of values selected' do
        (lower_limit..upper_limit).each do |version|
          view = map.version(version)
          num_selected = 0
          new_map = view.select do |_, value|
            if value % 2
              num_selected += 1
              true
            else
              false
            end
          end
          expect(new_map.version).to be(num_selected)
        end
      end

      it 'returns an empty map if the block never returns true' do
        (lower_limit..upper_limit).each do |version|
          view = map.version(version)
          new_map = view.select { |_, _| false }
          expect(new_map.size).to be(0)
        end
      end

      it 'returns an empty map with a version of zero when the block never returns true' do
        (lower_limit..upper_limit).each do |version|
          view = map.version(version)
          new_map = view.select { |_, _| false }
          expect(new_map.version).to be(0)
        end
      end

      (1..10).each do |mod_value|
        it "returns a new map that contains the expected set of key-value pairs (selected mod #{mod_value})" do
          (lower_limit..upper_limit).each do |version|
            view = map.version(version)
            new_map = view.select { |_, value| value % mod_value }
            expected_versions = (lower_limit..version).select { |candidate| candidate % mod_value }
            expected_pairs = expected_versions.map { |v| ["key-#{v}", v] }
            expect(new_map).to contain_exactly(*expected_pairs)
          end
        end
      end
    end

    context 'when a block is not provided' do
      it 'returns an enumerator' do
        (lower_limit..upper_limit).each do |value|
          expect(map.version(value).select).to be_a(Enumerator)
        end
      end

      it 'returned enumerator yields the correct array of unfiltered values at subsequent versions' do
        expected = []
        (lower_limit..upper_limit).each do |value|
          expected << ["key-#{value}", value]
          # Sort expected values to ensure they follow a lexical ordering
          expected.sort!
          enumerator = map.version(value).select
          expect { |b| enumerator.each(&b) }.to yield_successive_args(*expected)
        end
      end

      it 'returned enumerator yields the correct array of selected values at subsequent versions' do
        expected = []
        (lower_limit..upper_limit).each do |value|
          # Only expect values that are divisible by two
          expected << ["key-#{value}", value] if value.even?

          # Sort expected values to ensure they follow a lexical ordering
          expected.sort!
          enumerator = map.version(value).select

          # Check that enumerator can accept block that will only select values that are divisible by two
          expect { |b| enumerator.select { |_, v| v.even? }.map(&b) }.to yield_successive_args(*expected)
        end
      end

      it 'returned enumerator returns an equivalent iterator when select is called without a block' do
        (lower_limit..upper_limit).each do |value|
          enumerator = map.version(value).select
          lhs = enumerator.collect { |_, v| v.even? }
          rhs = enumerator.select.collect { |_, v| v.even? }
          expect(lhs.to_a).to match(rhs.to_a)
        end
      end
    end
  end

  describe '#size' do
    context 'on a new map' do
      it 'should return zero' do
        expect(map.size).to eq(0)
      end
    end
  end

  describe '#store' do
    it 'should allow values to be associated with new keys' do
      expect(map.store('a', '1').to_s).to eq('1')
      expect(map.store('b', '4').to_s).to eq('4')
    end

    it 'should allow values to be replace those associated with existing keys' do
      expect(map.store('a', '1').to_s).to eq('1')
      expect(map.store('a', '2').to_s).to eq('2')
    end

    it 'should clone and freeze unfrozen strings used as keys' do
      a = 'a'.dup
      expect(a.frozen?).to be_falsey
      expect(map.store(a, '1').to_s).to eq('1')
      expect(map.keys[0].frozen?).to be_truthy
    end
  end

  describe '#version' do
    context 'when called with a non-integral argument' do
      it 'should raise an error' do
        expect { map.version('a') }.to raise_error(TypeError)
      end
    end

    context 'when called with more than one argument' do
      it 'should raise an error' do
        expect { map.version(1, 2) }.to raise_error(ArgumentError)
      end
    end

    context 'when called on a new map' do
      context 'with no arguments' do
        it 'should return a version number of zero' do
          expect(map.version).to eq(0)
        end
      end

      context 'with one integer argument' do
        it 'should return a read-only view when version zero is requested' do
          expect(map.version(0).size).to eq(0)
        end

        it 'should raise an error if a version other than zero is requested' do
          expect { map.version(1) }.to raise_error(RangeError)
        end
      end
    end

    context 'when called on a map after two insertions' do
      before do
        map.store('a', '1')
        map.store('b', '4')
      end

      context 'with no arguments' do
        it 'should return a version of 2' do
          expect(map.version).to eq(2)
        end
      end

      context 'with one argument' do
        it 'should return a read-only view of the map at version 0 that reflects an empty map' do
          view = map.version(0)
          expect(view.size).to eq(0)
        end

        it 'should return a read-only view of the map at version 1 that reflects only the first insertion' do
          view = map.version(1)
          expect(view.fetch('a')).to eq('1')
          expect { view.fetch('b') }.to raise_error(KeyError)
        end

        it 'should return a read-only view of the map at version 2 that reflects both insertions' do
          view = map.version(2)
          expect(view.fetch('a').to_s).to eq('1')
          expect(view.fetch('b').to_s).to eq('4')
        end

        it 'should raise an error if a version greater than 2 is requested' do
          expect { map.version(3) }.to raise_error(RangeError)
        end
      end
    end
  end
end
