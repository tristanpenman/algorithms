require_relative '../spec_helper'

describe PersistentTree::Node do
  describe '#get_left_child' do
    context 'node with no modifications' do
      let!(:left) { described_class.with_no_mods(nil, nil, 5) }
      let!(:node) { described_class.with_no_mods(left, nil, 6) }

      (1..3).each do |version|
        it "returns the node corresponding to the original left sub-tree at version #{version}" do
          expect(node.get_left_child(version)).to eq(left)
        end
      end
    end

    context 'node with a modification to the left sub-tree at version 2' do
      let!(:left_v1) { described_class.with_no_mods(nil, nil, 5) }
      let!(:left_v2) { described_class.with_no_mods(nil, nil, 4) }
      let!(:node) { described_class.with_left_mod(left_v1, nil, 6, left_v2, 2) }

      it 'returns the node corresponding to the original left sub-tree at version 1' do
        expect(node. get_left_child(1)).to eq(left_v1)
      end

      (2..3).each do |version|
        it "returns the node corresponding to the updated left sub-tree at version #{version}" do
          expect(node.get_left_child(version)).to eq(left_v2)
        end
      end
    end

    context 'node with a modification to the right sub-tree at version 2' do
      let!(:left) { described_class.with_no_mods(nil, nil, 5) }
      let!(:right) { described_class.with_no_mods(nil, nil, 7) }
      let!(:node) { described_class.with_right_mod(left, nil, 6, right, 2) }

      (1..3).each do |version|
        it "returns the node corresponding to the original left sub-tree at version #{version}" do
          expect(node.get_left_child(version)).to eq(left)
        end
      end
    end
  end

  describe '#get_right_child' do
    context 'node with no modifications' do
      let!(:right) { described_class.with_no_mods(nil, nil, 7) }
      let!(:node) { described_class.with_no_mods(nil, right, 6) }

      (1..3).each do |version|
        it "returns the node corresponding to the original right sub-tree at version #{version}" do
          expect(node.get_right_child(version)).to eq(right)
        end
      end
    end

    context 'node with a modification to the right sub-tree at version 2' do
      let!(:right_v1) { described_class.with_no_mods(nil, nil, 7) }
      let!(:right_v2) { described_class.with_no_mods(nil, nil, 8) }
      let!(:node) { described_class.with_right_mod(nil, right_v1, 6, right_v2, 2) }

      it 'returns the node corresponding to the original right sub-tree at version 1' do
        expect(node.get_right_child(1)).to eq(right_v1)
      end

      (2..3).each do |version|
        it "returns the node corresponding to the updated right sub-tree at version #{version}" do
          expect(node.get_right_child(version)).to eq(right_v2)
        end
      end
    end

    context 'node with a modification to the left sub-tree at version 2' do
      let!(:left) { described_class.with_no_mods(nil, nil, 5) }
      let!(:right) { described_class.with_no_mods(nil, nil, 7) }
      let!(:node) { described_class.with_left_mod(nil, right, 6, left, 2) }

      (1..3).each do |version|
        it "returns the node corresponding to the original left sub-tree at version #{version}" do
          expect(node.get_right_child(version)).to eq(right)
        end
      end
    end
  end

  describe '#mod_left_child' do
    context 'setting the left sub-tree of a node with no children and no modifications' do
      let!(:node) { described_class.with_no_mods(nil, nil, 6) }
      let!(:left) { described_class.with_no_mods(nil, nil, 3) }

      before do
        node.mod_left_child(left, 2)
      end

      it 'leaves the left sub-tree unchanged at version 1' do
        left_child = node.get_left_child(1)
        expect(left_child).to be_nil
      end

      (1..2).each do |version|
        it "leaves the right sub-tree unchanged at version #{version}" do
          right_child = node.get_right_child(version)
          expect(right_child).to be_nil
        end
      end

      (2..3).each do |version|
        it "sets the left sub-tree at version #{version}" do
          left_child = node.get_left_child(version)
          expect(left_child).to eq(left)
        end

        it "sets the parent of the left sub-tree at version #{version} to be the updated node" do
          left_child = node.get_left_child(version)
          expect(left_child.parent).to eq(node)
        end
      end
    end

    context 'setting the left-subtree of a node with an existing left sub-tree modification' do
      let!(:left) { described_class.with_no_mods(nil, nil, 3) }
      let!(:node) { described_class.with_left_mod(nil, nil, 6, left, 2) }
      let!(:child) { described_class.with_no_mods(nil, nil, 1) }

      it 'raises an error' do
        expect { node.mod_left_child(child, 2) }.to raise_error(described_class::InvariantViolationError)
      end
    end

    context 'setting the left-subtree of a node with an existing right sub-tree modification' do
      let!(:right) { described_class.with_no_mods(nil, nil, 7) }
      let!(:node) { described_class.with_right_mod(nil, nil, 6, right, 2) }
      let!(:child) { described_class.with_no_mods(nil, nil, 1) }

      it 'raises an error' do
        expect { node.mod_left_child(child, 2) }.to raise_error(described_class::InvariantViolationError)
      end
    end
  end

  describe '.mod_right_child' do
    context 'setting the left sub-tree of a node with no children and no modifications' do
      let!(:node) { described_class.with_no_mods(nil, nil, 6) }
      let!(:right) { described_class.with_no_mods(nil, nil, 8) }

      before do
        node.mod_right_child(right, 2)
      end

      it 'leaves the right sub-tree unchanged at version 1' do
        right_child = node.get_right_child(1)
        expect(right_child).to be_nil
      end

      (1..2).each do |version|
        it "leaves the left sub-tree unchanged at version #{version}" do
          left_child = node.get_left_child(version)
          expect(left_child).to be_nil
        end
      end

      (2..3).each do |version|
        it "sets the right sub-tree at version #{version}" do
          right_child = node.get_right_child(version)
          expect(right_child).to eq(right)
        end

        it "sets the parent of the right sub-tree at version #{version} to be the updated node" do
          right_child = node.get_right_child(version)
          expect(right_child.parent).to eq(node)
        end
      end
    end

    context 'setting the right-subtree of a node with an existing left sub-tree modification' do
      let!(:left) { described_class.with_no_mods(nil, nil, 3) }
      let!(:node) { described_class.with_left_mod(nil, nil, 6, left, 2) }
      let!(:child) { described_class.with_no_mods(nil, nil, 10) }

      it 'raises an error' do
        expect { node.mod_right_child(child, 2) }.to raise_error(described_class::InvariantViolationError)
      end
    end

    context 'setting the right-subtree of a node with an existing right sub-tree modification' do
      let!(:right) { described_class.with_no_mods(nil, nil, 8) }
      let!(:node) { described_class.with_right_mod(nil, nil, 6, right, 2) }
      let!(:child) { described_class.with_no_mods(nil, nil, 10) }

      it 'raises an error' do
        expect { node.mod_right_child(child, 2) }.to raise_error(described_class::InvariantViolationError)
      end
    end
  end
end
