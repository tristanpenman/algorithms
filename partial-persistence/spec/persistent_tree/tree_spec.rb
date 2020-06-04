require_relative '../spec_helper'

describe PersistentTree::Tree do
  context 'empty tree' do
    let!(:tree) { described_class.new }

    context 'after construction' do
      it 'expect current version to be 0' do
        expect(tree.version).to eq(0)
      end

      it 'expect that finding an item using a nil version to raise a TypeError' do
        expect { tree.find(nil, 'some value') }.to raise_error(TypeError)
      end

      it 'expect that finding an item in a version less than than zero to raise a RangeError' do
        expect { tree.find(-1, 'some value') }.to raise_error(RangeError)
      end

      it 'expect that finding an item in a version greater than zero to raise a RangeError' do
        expect { tree.find(1, 'some value') }.to raise_error(RangeError)
      end

      it 'expect that finding an item in a version that is not an integer to raise a TypeError' do
        expect { tree.find('some version', 'some value') }.to raise_error(TypeError)
      end

      it 'yields no values when each is called' do
        expect { |b| tree.each(0, &b) }.to_not yield_control
      end
    end

    context 'after inserting an item' do
      let!(:item) { 5 }

      before do
        tree.insert(item)
      end

      it 'expect that finding an item using a nil version to raise a TypeError' do
        expect { tree.find(nil, 'some value') }.to raise_error(TypeError)
      end

      it 'expect that finding an item in a version less than than zero to raise a RangeError' do
        expect { tree.find(-1, 'some value') }.to raise_error(RangeError)
      end

      it 'expect that finding an item in a version greater than 1 to raise a RangeError' do
        expect { tree.find(2, 'some value') }.to raise_error(RangeError)
      end

      it 'expect current version to be 1' do
        expect(tree.version).to eq(1)
      end

      it 'expect tree to not find the inserted item at version 0' do
        expect { tree.find(0, item) }.to raise_error(KeyError)
      end

      it 'expect tree to find the inserted item at version 1' do
        expect(tree.find(1, item)).to eq(item)
      end

      it 'inserting the same item should raise an exception' do
        expect { tree.insert(item) }.to raise_error(KeyError)
      end

      it 'yields no values when each is called at version 0' do
        expect { |b| tree.each(0, &b) }.to_not yield_control
      end

      it 'yields exactly once, with the expected value, when each is called at version 1' do
        expect { |b| tree.each(1, &b) }.to yield_with_args(item)
      end
    end

    context 'after inserting 10 distinct items' do
      let!(:lower_limit) { 1 }
      let!(:upper_limit) { 10 }
      let!(:range) { 1..upper_limit }

      before do
        range.each do |item|
          tree.insert(item)
        end
      end

      it 'expect that finding an item using a nil version to raise a TypeError' do
        expect { tree.find(nil, 'some value') }.to raise_error(TypeError)
      end

      it 'expect that finding an item in a version less than than zero to raise a RangeError' do
        expect { tree.find(-1, 'some value') }.to raise_error(RangeError)
      end

      it 'expect that finding an item in a version greater than 10 to raise a RangeError' do
        expect { tree.find(11, 'some value') }.to raise_error(RangeError)
      end

      it 'expect current version to be 10' do
        expect(tree.version).to eq(10)
      end

      it 'expect tree to not find any of the inserted items at version 0' do
        range.each do |item|
          expect { tree.find(0, item) }.to raise_error(KeyError)
        end
      end

      it 'expect tree to not find any of the inserted items before the version at which they were inserted' do
        range.each do |item|
          ((lower_limit - 1)..(item - 1)).each do |version|
            expect { tree.find(version, item) }.to raise_error(KeyError)
          end
        end
      end

      it 'expect tree to find inserted items at or after the version at which they were inserted' do
        range.each do |item|
          (item..upper_limit).each do |version|
            expect(tree.find(version, item)).to eq(item)
          end
        end
      end

      it 'inserting any of the same items should raise an exception' do
        range.each do |item|
          expect { tree.insert(item) }.to raise_error(KeyError)
        end
      end

      it 'yields no values when each is called at version 0' do
        expect { |b| tree.each(0, &b) }.to_not yield_control
      end

      it 'yields correct array of values at subsequent versions' do
        (lower_limit..upper_limit).each do |value|
          expect { |b| tree.each(value, &b) }.to yield_successive_args(*(lower_limit..value))
        end
      end
    end
  end
end
