require_relative '../spec_helper'

describe PersistentTree::Algorithms do
  describe '.find' do
    #
    # Scenario:
    #
    #         o
    #    1)  / \
    #
    context 'root node with no children or modifications' do
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, nil, 4) }
      context 'search for a value in the tree (4) at version 1' do
        let!(:result) { described_class.find(root_node, 1, 4) }
        it 'returns the correct value' do
          expect(result.value).to eq(4)
        end
      end

      [2, 3, 5, 6].each do |value|
        context "search for a value not in the tree (#{value}) at version 1" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 1, value) }.to raise_error(KeyError)
          end
        end
      end
    end

    #
    # Scenario:
    #
    #          o
    #    1)   / \
    #        o
    #
    context 'root node with one left child but no modifications to either node' do
      let!(:left) { PersistentTree::Node.with_no_mods(nil, nil, 3) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(left, nil, 4) }

      [3, 4].each do |value|
        context "search for a value in the tree (#{value}) at version 1" do
          let!(:result) { described_class.find(root_node, 1, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [1, 2, 5, 6].each do |value|
        context "search for a value not in the tree (#{value}) at version 1" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 1, value) }.to raise_error(KeyError)
          end
        end
      end
    end

    #
    # Scenario:
    #
    #         o
    #    1)  / \
    #           o
    #
    context 'root node with one right child but no modifications to either node' do
      let!(:right) { PersistentTree::Node.with_no_mods(nil, nil, 5) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, right, 4) }

      [4, 5].each do |value|
        context "search for a value in the tree (#{value}) at version 1" do
          let!(:result) { described_class.find(root_node, 1, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [2, 3, 6, 7].each do |value|
        context "search for a value not in the tree (#{value}) at version 1" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 1, value) }.to raise_error(KeyError)
          end
        end
      end
    end

    #
    # Scenario:
    #
    #          o
    #    1)   / \
    #        o   o
    #
    context 'root node with one left and one right child but no modifications to any node' do
      let!(:left) { PersistentTree::Node.with_no_mods(nil, nil, 3) }
      let!(:right) { PersistentTree::Node.with_no_mods(nil, nil, 5) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(left, right, 4) }

      [3, 4, 5].each do |value|
        context "search for a value in the tree (#{value}) at version 1" do
          let!(:result) { described_class.find(root_node, 1, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [1, 2, 6, 7].each do |value|
        context "search for a value not in the tree (#{value}) at version 1" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 1, value) }.to raise_error(KeyError)
          end
        end
      end
    end

    #
    # Scenario:
    #
    #          o*
    #    1)   / \
    #        o   o
    #
    context 'root node with one left and one right child and a modification to the left sub-tree' do
      let!(:left_v1) { PersistentTree::Node.with_no_mods(nil, nil, 3) }
      let!(:left_v2) { PersistentTree::Node.with_no_mods(nil, nil, 2) }
      let!(:right) { PersistentTree::Node.with_no_mods(nil, nil, 5) }
      let!(:root_node) { PersistentTree::Node.with_left_mod(left_v1, right, 4, left_v2, 2) }

      [3, 4, 5].each do |value|
        context "search for a value in the tree (#{value}) at version 1" do
          let!(:result) { described_class.find(root_node, 1, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [1, 2, 6, 7].each do |value|
        context "search for a value not in the tree (#{value}) at version 1" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 1, value) }.to raise_error(KeyError)
          end
        end
      end

      [2, 4, 5].each do |value|
        context "search for a value in the tree (#{value}) at version 2" do
          let!(:result) { described_class.find(root_node, 2, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [1, 3, 6, 7].each do |value|
        context "search for a value not in the tree (#{value}) at version 2" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 2, value) }.to raise_error(KeyError)
          end
        end
      end
    end

    #
    # Scenario:
    #
    #          o*
    #    1)   / \
    #        o   o
    #
    context 'root node with one left and one right child and a modification to the right sub-tree' do
      let!(:left) { PersistentTree::Node.with_no_mods(nil, nil, 3) }
      let!(:right_v1) { PersistentTree::Node.with_no_mods(nil, nil, 5) }
      let!(:right_v2) { PersistentTree::Node.with_no_mods(nil, nil, 6) }
      let!(:root_node) { PersistentTree::Node.with_right_mod(left, right_v1, 4, right_v2, 2) }

      [3, 4, 5].each do |value|
        context "search for a value in the tree (#{value}) at version 1" do
          let!(:result) { described_class.find(root_node, 1, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [1, 2, 6, 7].each do |value|
        context "search for a value not in the tree (#{value}) at version 1" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 1, value) }.to raise_error(KeyError)
          end
        end
      end

      [3, 4, 6].each do |value|
        context "search for a value in the tree (#{value}) at version 2" do
          let!(:result) { described_class.find(root_node, 2, value) }
          it 'returns the correct value' do
            expect(result.value).to eq(value)
          end
        end
      end

      [1, 2, 5, 7].each do |value|
        context "search for a value not in the tree (#{value}) at version 2" do
          it 'raises a KeyError' do
            expect { described_class.find(root_node, 2, value) }.to raise_error(KeyError)
          end
        end
      end
    end
  end

  describe '.insert' do
    #
    # Scenario:
    #
    #         o
    #    1)  / \
    #
    #
    context 'insert existing value into tree with just a root node' do
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, nil, root_value) }

      it 'raises exception if an existing value is inserted' do
        expect { described_class.insert(root_node, 1, root_value) }.to raise_error(KeyError)
      end
    end

    #
    # Scenario:
    #
    #         o               o*
    #    1)  / \   ->   2)   / \
    #                       o
    #
    context 'insert existing value into tree with a root node and one left child' do
      let!(:left_child_value) { 8 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_left_mod(nil, nil, root_value, left_child, 2) }

      it 'raises exception if an existing value is inserted' do
        expect { described_class.insert(root_node, 1, root_value) }.to raise_error(KeyError)
      end
    end

    #
    # Scenario:
    #
    #         o               o*
    #    1)  / \   ->   2)   / \
    #                       o
    #
    context 'insert existing value into tree with a root node and one right child' do
      let!(:right_child_value) { 12 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_right_mod(nil, nil, root_value, right_child, 2) }

      it 'raises exception if an existing value is inserted' do
        expect { described_class.insert(root_node, 1, root_value) }.to raise_error(KeyError)
      end
    end

    #
    # Scenario:
    #
    #         o               o*
    #    1)  / \   ->   2)   / \
    #                       o
    #
    context 'insert into left sub-tree of root node with no existing children' do
      let!(:root_value) { 10 }
      let!(:inserted_value) { 9 }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, nil, root_value) }
      let!(:returned_node) { described_class.insert(root_node, 1, inserted_value) }

      it 'returns the root node' do
        expect(returned_node).to eq(root_node)
      end

      it 'leaves the left sub-tree of the root node unchanged at version 1' do
        left_child = root_node.get_left_child(1)
        expect(left_child).to be_nil
      end

      it 'stores the value in a new node in the left sub-tree of the root node at version 2' do
        left_child = root_node.get_left_child(2)
        expect(left_child).to_not be_nil
        expect(left_child.value).to eq(inserted_value)
      end

      it 'leaves the right sub-tree of the root node unchanged at version 1' do
        right_child = root_node.get_right_child(1)
        expect(right_child).to be_nil
      end

      it 'leaves the right sub-tree of the root node unchanged at version 2' do
        right_child = root_node.get_right_child(2)
        expect(right_child).to be_nil
      end
    end

    #
    # Scenario:
    #
    #          o                 o*
    #         / \               / \
    #    1)  o      ->   2)    o*
    #                         / \
    #                        o
    #
    context 'insert into left side of left sub-tree of root node with a left-child and no modifications' do
      let!(:root_value) { 10 }
      let!(:left_child_value) { 9 }
      let!(:inserted_value) { 8 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(left_child, nil, root_value) }
      let!(:returned_node) { described_class.insert(root_node, 1, inserted_value) }

      it 'returns the root node' do
        expect(returned_node).to eq(root_node)
      end

      [1, 2].each do |version|
        it "leaves the left sub-tree of the original root node unchanged at version #{version}" do
          actual_left_child = root_node.get_left_child(version)
          expect(actual_left_child).to eq(left_child)
        end

        it "leaves the right sub-tree of the original root node unchanged at version #{version}" do
          actual_right_child = root_node.get_right_child(version)
          expect(actual_right_child).to be_nil
        end
      end

      it 'leaves the left sub-tree of the original left child (grand-child of root node) unmodified at version 1' do
        actual_left_grand_child = root_node.get_left_child(1).get_left_child(1)
        expect(actual_left_grand_child).to be_nil
      end

      it 'modifies the left sub-tree of the original left child (grand-child of root node) at version 2' do
        actual_left_grand_child = root_node.get_left_child(2).get_left_child(2)
        expect(actual_left_grand_child).to_not be_nil
        expect(actual_left_grand_child.value).to eq(inserted_value)
      end
    end

    #
    # Scenario:
    #
    #          o                o*
    #         / \              / \
    #    1)  o      ->   2)   o*
    #                        / \
    #                           o
    #
    context 'insert into right side of left sub-tree of root node with a left-child and no modifications' do
      let!(:root_value) { 10 }
      let!(:left_child_value) { 8 }
      let!(:inserted_value) { 9 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(left_child, nil, root_value) }
      let!(:returned_node) { described_class.insert(root_node, 1, inserted_value) }

      it 'returns the root node' do
        expect(returned_node).to eq(root_node)
      end

      [1, 2].each do |version|
        it "leaves the left sub-tree and left-most grand child of the root node unchanged at version #{version}" do
          actual_left_child = root_node.get_left_child(version)
          expect(actual_left_child).to eq(left_child)
          expect(actual_left_child.get_left_child(version)).to be_nil
        end

        it "leaves the right sub-tree of the original root node unchanged at version #{version}" do
          actual_right_child = root_node.get_right_child(version)
          expect(actual_right_child).to be_nil
        end
      end

      it 'leaves the right sub-tree of the original left child (grand-child of root node) unchanged at version 1' do
        actual_grand_child = root_node.get_left_child(1).get_right_child(1)
        expect(actual_grand_child).to be_nil
      end

      it 'modifies the right sub-tree of the original left child (grand-child of root node) at version 2' do
        actual_grand_child = root_node.get_left_child(2).get_right_child(2)
        expect(actual_grand_child).to_not be_nil
        expect(actual_grand_child.value).to eq(inserted_value)
      end
    end

    #
    # Scenario:
    #
    #         o              o*
    #    1)  / \   ->   2)  / \
    #                          o
    #
    context 'insert into right sub-tree of root node with no existing children' do
      let!(:root_value) { 10 }
      let!(:inserted_value) { 11 }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, nil, root_value) }
      let!(:returned_node) { described_class.insert(root_node, 1, inserted_value) }

      it 'returns the root node' do
        expect(root_node).to eq(root_node)
      end

      it 'leaves the left sub-tree of the root node unchanged at version 1' do
        left_child = root_node.get_left_child(1)
        expect(left_child).to be_nil
      end

      it 'leaves the left sub-tree of the root node unchanged at version 2' do
        left_child = root_node.get_left_child(2)
        expect(left_child).to be_nil
      end

      it 'leaves the right sub-tree of the root node unchanged at version 1' do
        right_child = root_node.get_right_child(1)
        expect(right_child).to be_nil
      end

      it 'stores the value in a new node in the right sub-tree of the root node at version 2' do
        right_child = root_node.get_right_child(2)
        expect(right_child).to_not be_nil
        expect(right_child.value).to eq(inserted_value)
      end
    end

    #
    # Scenario:
    #
    #         o               o*
    #        / \             / \
    #    1)     o   ->   2)     o*
    #                          / \
    #                         o
    #
    context 'insert into left side of right sub-tree of root node with a right-child and no modifications' do
      let!(:root_value) { 10 }
      let!(:right_child_value) { 12 }
      let!(:inserted_value) { 11 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, right_child, root_value) }
      let!(:returned_node) { described_class.insert(root_node, 1, inserted_value) }

      it 'returns the root node' do
        expect(returned_node).to eq(root_node)
      end

      [1, 2].each do |version|
        it "leaves the left sub-tree of the original root node unchanged at version #{version}" do
          actual_left_child = root_node.get_left_child(version)
          expect(actual_left_child).to be_nil
        end

        it "leaves the right sub-tree (and right-most grand child) of the root node unchanged at version #{version}" do
          actual_right_child = root_node.get_right_child(version)
          expect(actual_right_child).to eq(right_child)
          expect(actual_right_child.get_right_child(version)).to be_nil
        end
      end

      it 'leaves the left sub-tree of the original right child (grand-child of root node) unmodified at version 1' do
        actual_grand_child = root_node.get_right_child(1).get_left_child(1)
        expect(actual_grand_child).to be_nil
      end

      it 'modifies the left sub-tree of the original right child (grand-child of root node) at version 2' do
        actual_grand_child = root_node.get_right_child(2).get_left_child(2)
        expect(actual_grand_child).to_not be_nil
        expect(actual_grand_child.value).to eq(inserted_value)
      end
    end

    #
    # Scenario:
    #
    #         o               o
    #        / \             / \
    #    1)     o   ->   2)     o*
    #                          / \
    #                             o
    #
    context 'insert into right side of right sub-tree of root node with a right-child and no modifications' do
      let!(:root_value) { 10 }
      let!(:right_child_value) { 11 }
      let!(:inserted_value) { 12 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, right_child, root_value) }
      let!(:returned_node) { described_class.insert(root_node, 1, inserted_value) }

      it 'returns the root node' do
        expect(returned_node).to eq(root_node)
      end

      it 'the root node has not been modified' do
        expect(returned_node.modified?).to be_falsey
      end

      [1, 2].each do |version|
        it "leaves the left sub-tree of the original root node unchanged at version #{version}" do
          actual_left_child = root_node.get_left_child(version)
          expect(actual_left_child).to be_nil
        end

        it "leaves the right sub-tree of the original root node unchanged at version #{version}" do
          actual_right_child = root_node.get_right_child(version)
          expect(actual_right_child).to eq(right_child)
        end
      end

      it 'leaves the right sub-tree of the original right child (grand-child of root node) unmodified at version 1' do
        actual_right_grand_child = root_node.get_right_child(1).get_right_child(1)
        expect(actual_right_grand_child).to be_nil
      end

      it 'modifies the right sub-tree of the original right child (grand-child of root node) at version 2' do
        actual_right_grand_child = root_node.get_right_child(2).get_right_child(2)
        expect(actual_right_grand_child).to_not be_nil
        expect(actual_right_grand_child.value).to eq(inserted_value)
      end
    end

    #
    # Scenario:
    #
    #         o               o*               n               o*              n
    #    1)  / \   ->   2)   / \    ->   3)   / \    ->   4)  / \   ->   5)   / \
    #                       o                o   o               o           o   o
    #
    context 'insert into left sub-tree of modified root node with a right child and a left modification' do
      # Set up the child nodes that exist at version 3
      let!(:left_child_value) { 3 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:right_child_value) { 7 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      # Set up the root node at version 4
      let!(:root_value) { 5 }
      let!(:root_node) { PersistentTree::Node.with_left_mod(left_child, right_child, root_value, nil, 4) }
      # Insert the new value at version 5
      let!(:inserted_value) { 3 }
      let!(:returned_node) { described_class.insert(root_node, 4, inserted_value) }

      it 'returns a new root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'the returned root node has the correct value' do
        expect(returned_node.value).to eq(root_value)
      end

      it 'the returned root node to have not been modified' do
        expect(returned_node.modified?).to be_falsey
      end

      (4..5).each do |version|
        # Since the new tree has no knowledge of previous versions, we expect it to contain the return the newly
        # inserted node when querying either version.
        it "the left child of the returned root node has the inserted value at version #{version}" do
          expect(returned_node.get_left_child(version).value).to eq(inserted_value)
        end

        it "the left child of the returned root node does not have a modification at version #{version}" do
          expect(returned_node.get_left_child(version).modified?).to be_falsey
        end

        # The right sub-tree should not have been modified
        it "the right child should be the same node at version #{version}" do
          expect(returned_node.get_right_child(version)).to eq(right_child)
        end

        it "the right child should not have any modification at #{version}" do
          expect(returned_node.get_right_child(version).modified?).to be_falsey
        end
      end
    end

    #
    # Scenario:
    #
    #         o               o*               n               o*              n
    #    1)  / \   ->   2)   / \    ->   3)   / \    ->   4)  / \   ->   5)   / \
    #                       o                o   o           o               o   o
    #
    context 'insert into right sub-tree of modified root node with a left child and a right modification' do
      # Set up the child nodes that exist at version 3
      let!(:left_child_value) { 3 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:right_child_value) { 7 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      # Set up the root node at version 4
      let!(:root_value) { 5 }
      let!(:root_node) { PersistentTree::Node.with_right_mod(left_child, right_child, root_value, nil, 4) }
      # Insert the new value at version 5
      let!(:inserted_value) { 7 }
      let!(:returned_node) { described_class.insert(root_node, 4, inserted_value) }

      it 'returns a new root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'the returned root node has the correct value' do
        expect(returned_node.value).to eq(root_value)
      end

      it 'the returned root node to have not been modified' do
        expect(returned_node.modified?).to be_falsey
      end

      (4..5).each do |version|
        # The left sub-tree should not have been modified
        it "the left child should be the same node at version #{version}" do
          expect(returned_node.get_left_child(version)).to eq(left_child)
        end

        it "the left child should not have any modification at #{version}" do
          expect(returned_node.get_left_child(version).modified?).to be_falsey
        end

        # Since the new tree has no knowledge of previous versions, we expect it to contain the return the newly
        # inserted node when querying either version.
        it "the right child of the returned root node has the inserted value at version #{version}" do
          expect(returned_node.get_right_child(version).value).to eq(inserted_value)
        end

        it "the right child of the returned root node does not have a modification at version #{version}" do
          expect(returned_node.get_right_child(version).modified?).to be_falsey
        end
      end
    end

    #
    # Scenario:
    #
    #       o                o*               o*                n
    #      / \              / \              / \               / \
    #  1)        ->   2)   o      ->   3)   o*     ->   4)    n
    #                     / \              / \               / \
    #                                         o             n   o
    #
    context 'insert into left-most sub-tree of left side of root node, where both ancestors have a modification' do
      # Set up the tree at version 3
      let!(:grand_child_value) { 4 }
      let!(:grand_child) { PersistentTree::Node.with_no_mods(nil, nil, grand_child_value) }
      let!(:left_child_value) { 3 }
      let!(:left_child) { PersistentTree::Node.with_right_mod(nil, nil, left_child_value, grand_child, 3) }
      let!(:root_value) { 5 }
      let!(:root_node) { PersistentTree::Node.with_left_mod(nil, nil, root_value, left_child, 2) }
      # Insert the new value at version 4
      let!(:inserted_value) { 2 }
      let!(:returned_node) { described_class.insert(root_node, 3, inserted_value) }

      it 'returns a new root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'returns a new root node with the correct value' do
        expect(returned_node.value).to eq(root_value)
      end

      it 'returns a new root node with no modifications' do
        expect(returned_node.modified?).to be_falsey
      end

      (1..4).each do |version|
        it "the left child of the new root node should have the correct value at version #{version}" do
          expect(returned_node.get_left_child(version).value).to eq(left_child_value)
        end

        it "the left child of the new root node is also a new node at version #{version}" do
          expect(returned_node.get_left_child(version)).to_not eq(left_child)
        end

        it "the left child of the new root node does not have any modifications at version #{version}" do
          expect(returned_node.get_left_child(version).modified?).to be_falsey
        end

        it "the left-most grand child should be set at version #{version}" do
          expect(returned_node.get_left_child(version).get_left_child(version)).to_not be_nil
        end

        it "the left-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_left_child(version).get_left_child(version).value).to eq(inserted_value)
        end

        it "the right-most grand child should be set at version #{version}" do
          expect(returned_node.get_left_child(version).get_right_child(version)).to_not be_nil
        end

        it "the right-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_left_child(version).get_right_child(version).value).to eq(grand_child_value)
        end

        it "the right-most grand child has no modification at version #{version}" do
          expect(returned_node.get_left_child(version).get_right_child(version).modified?).to be_falsey
        end
      end
    end

    #
    # Scenario:
    #
    #       o                o*               o*                n
    #      / \              / \              / \               / \
    #  1)        ->   2)   o      ->   3)   o*     ->   4)    n
    #                     / \              / \               / \
    #                                     o                 o   n
    #
    context 'insert into right-most sub-tree of left side of root node, where both ancestors have a modification' do
      # Set up the tree at version 3
      let!(:grand_child_value) { 2 }
      let!(:grand_child) { PersistentTree::Node.with_no_mods(nil, nil, grand_child_value) }
      let!(:left_child_value) { 3 }
      let!(:left_child) { PersistentTree::Node.with_left_mod(nil, nil, left_child_value, grand_child, 3) }
      let!(:root_value) { 5 }
      let!(:root_node) { PersistentTree::Node.with_left_mod(nil, nil, root_value, left_child, 2) }
      # Insert the new value at version 4
      let!(:inserted_value) { 4 }
      let!(:returned_node) { described_class.insert(root_node, 3, inserted_value) }

      it 'returns a new root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'returns a new root node with the correct value' do
        expect(returned_node.value).to eq(root_value)
      end

      it 'returns a new root node with no modifications' do
        expect(returned_node.modified?).to be_falsey
      end

      (1..4).each do |version|
        it "the left child of the new root node should have the correct value at version #{version}" do
          expect(returned_node.get_left_child(version).value).to eq(left_child_value)
        end

        it "the left child of the new root node is also a new node at version #{version}" do
          expect(returned_node.get_left_child(version)).to_not eq(left_child)
        end

        it "the left child of the new root node does not have any modifications at version #{version}" do
          expect(returned_node.get_left_child(version).modified?).to be_falsey
        end

        it "the left-most grand child should be set at version #{version}" do
          expect(returned_node.get_left_child(version).get_left_child(version)).to_not be_nil
        end

        it "the left-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_left_child(version).get_left_child(version).value).to eq(grand_child_value)
        end

        it "the left-most grand child has no modification at version #{version}" do
          expect(returned_node.get_left_child(version).get_left_child(version).modified?).to be_falsey
        end

        it "the right-most grand child should be set at version #{version}" do
          expect(returned_node.get_left_child(version).get_right_child(version)).to_not be_nil
        end

        it "the right-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_left_child(version).get_right_child(version).value).to eq(inserted_value)
        end
      end
    end

    #
    # Scenario:
    #
    #       o              o*               o*               n
    #      / \            / \              / \              / \
    #  1)        ->   2)     o    ->   3)     o*   ->   4)     n
    #                       / \              / \              / \
    #                                           o            n   o
    #
    context 'insert into left-most sub-tree of right side of root node, where both ancestors have a modification' do
      # Set up the tree at version 3
      let!(:grand_child_value) { 8 }
      let!(:grand_child) { PersistentTree::Node.with_no_mods(nil, nil, grand_child_value) }
      let!(:right_child_value) { 7 }
      let!(:right_child) { PersistentTree::Node.with_right_mod(nil, nil, right_child_value, grand_child, 3) }
      let!(:root_value) { 5 }
      let!(:root_node) { PersistentTree::Node.with_right_mod(nil, nil, root_value, right_child, 2) }
      # Insert the new value at version 4
      let!(:inserted_value) { 6 }
      let!(:returned_node) { described_class.insert(root_node, 3, inserted_value) }

      it 'returns a new root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'returns a new root node with the correct value' do
        expect(returned_node.value).to eq(root_value)
      end

      it 'returns a new root node with no modifications' do
        expect(returned_node.modified?).to be_falsey
      end

      (1..4).each do |version|
        it "the right child of the new root node should have the correct value at version #{version}" do
          expect(returned_node.get_right_child(version).value).to eq(right_child_value)
        end

        it "the right child of the new root node is also a new node at version #{version}" do
          expect(returned_node.get_right_child(version)).to_not eq(right_child)
        end

        it "the right child of the new root node does not have any modifications at version #{version}" do
          expect(returned_node.get_right_child(version).modified?).to be_falsey
        end

        it "the left-most grand child should be set at version #{version}" do
          expect(returned_node.get_right_child(version).get_left_child(version)).to_not be_nil
        end

        it "the left-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_right_child(version).get_left_child(version).value).to eq(inserted_value)
        end

        it "the right-most grand child should be set at version #{version}" do
          expect(returned_node.get_right_child(version).get_right_child(version)).to_not be_nil
        end

        it "the right-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_right_child(version).get_right_child(version).value).to eq(grand_child_value)
        end

        it "the right-most grand child has no modification at version #{version}" do
          expect(returned_node.get_right_child(version).get_right_child(version).modified?).to be_falsey
        end
      end
    end

    #
    # Scenario:
    #
    #       o              o*               o*               n
    #      / \            / \              / \              / \
    #  1)        ->   2)     o    ->   3)     o*   ->   4)     n
    #                       / \              / \              / \
    #                                       o                o   n
    #
    context 'insert into right-most sub-tree of right side of root node, where both ancestors have a modification' do
      # Set up the tree at version 3
      let!(:left_grand_child_value) { 6 }
      let!(:left_grand_child) { PersistentTree::Node.with_no_mods(nil, nil, left_grand_child_value) }
      let!(:right_child_value) { 7 }
      let!(:right_child) { PersistentTree::Node.with_left_mod(nil, nil, right_child_value, left_grand_child, 3) }
      let!(:root_value) { 5 }
      let!(:root_node) { PersistentTree::Node.with_right_mod(nil, nil, root_value, right_child, 2) }
      # Insert the new value at version 4
      let!(:inserted_value) { 8 }
      let!(:returned_node) { described_class.insert(root_node, 3, inserted_value) }

      it 'returns a new root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'returns a new root node with the correct value' do
        expect(returned_node.value).to eq(root_value)
      end

      it 'returns a new root node with no modifications' do
        expect(returned_node.modified?).to be_falsey
      end

      (1..4).each do |version|
        it "the right child of the new root node should have the correct value at version #{version}" do
          expect(returned_node.get_right_child(version).value).to eq(right_child_value)
        end

        it "the right child of the new root node is also a new node at version #{version}" do
          expect(returned_node.get_right_child(version)).to_not eq(right_child)
        end

        it "the right child of the new root node does not have any modifications at version #{version}" do
          expect(returned_node.get_right_child(version).modified?).to be_falsey
        end

        it "the left-most grand child should be set at version #{version}" do
          expect(returned_node.get_right_child(version).get_left_child(version)).to_not be_nil
        end

        it "the left-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_right_child(version).get_left_child(version).value).to eq(left_grand_child_value)
        end

        it "the right-most grand child should be set at version #{version}" do
          expect(returned_node.get_right_child(version).get_right_child(version)).to_not be_nil
        end

        it "the right-most grand child has the correct value at version #{version}" do
          expect(returned_node.get_right_child(version).get_right_child(version).value).to eq(inserted_value)
        end

        it "the right-most grand child has no modification at version #{version}" do
          expect(returned_node.get_right_child(version).get_right_child(version).modified?).to be_falsey
        end
      end
    end
  end

  describe '.insert!' do
    ComplexValue = Struct.new(:key, :metadata)
    class ComplexValue
      include Comparable
      def <=>(other)
        if other.is_a? ComplexValue
          key <=> other.key
        else
          key <=> other
        end
      end
    end

    #
    # Scenario:
    #
    #         o
    #    1)  / \
    #
    #
    context 'insert existing value into tree with just a root node' do
      let!(:root_value) { ComplexValue.new(10, 'a') }
      let!(:inserted_value) { ComplexValue.new(10, 'b') }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, nil, root_value) }
      let!(:returned_node) { described_class.insert!(root_node, 1, inserted_value) }

      it 'returns a node' do
        expect(returned_node).to_not be_nil
      end

      it 'returns a node that is not the same as the existing root node' do
        expect(returned_node).to_not eq(root_node)
      end

      it 'returns a node whose value is the new updated value' do
        expect(returned_node.value.key).to eq(inserted_value.key)
      end

      it 'the metadata of the updated value has been preserved' do
        expect(root_node.value.metadata).to eq('a')
      end

      it 'the metadata of the updated value has been preserved' do
        expect(returned_node.value.metadata).to eq('b')
      end

      it 'find returns the previous value at version 1' do
        expect(PersistentTree::Algorithms.find(root_node, 1, 10).value).to eq(root_value)
      end

      it 'find returns the current value at version 2' do
        expect(PersistentTree::Algorithms.find(root_node, 2, 10).value).to eq(inserted_value)
      end
    end
  end

  describe '.traverse' do
    def items_as_array(node, version)
      output = []
      described_class.traverse(node, version) do |child|
        output << child
      end
      output
    end

    context 'tree with one node, no modifications' do
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_no_mods(nil, nil, root_value) }
      let!(:results) { items_as_array(root_node, 1) }

      it 'output should contain one node' do
        expect(results).to eq([root_node])
      end
    end

    context 'tree with two nodes, where root has left child modification at version 2' do
      let!(:left_child_value) { 8 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_left_mod(nil, nil, root_value, left_child, 2) }

      context 'output at version 1' do
        let!(:results) { items_as_array(root_node, 1) }

        it 'yields for root node, but not left child' do
          expect(results).to eq([root_node])
        end
      end

      context 'output at version 2' do
        let!(:results) { items_as_array(root_node, 2) }

        it 'yields for root node and left child in correct order' do
          expect(results).to eq([left_child, root_node])
        end
      end
    end

    context 'tree with two nodes, where root has right child modification at version 2' do
      let!(:right_child_value) { 12 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_right_mod(nil, nil, root_value, right_child, 2) }

      context 'output at version 1' do
        let!(:results) { items_as_array(root_node, 1) }

        it 'yields for root node, but not left child' do
          expect(results).to eq([root_node])
        end
      end

      context 'output at version 2' do
        let!(:results) { items_as_array(root_node, 2) }

        it 'yields for root node and left child in correct order' do
          expect(results).to eq([root_node, right_child])
        end
      end
    end

    context 'tree with three nodes, no modifications' do
      let!(:left_child_value) { 8 }
      let!(:left_child) { PersistentTree::Node.with_no_mods(nil, nil, left_child_value) }
      let!(:right_child_value) { 12 }
      let!(:right_child) { PersistentTree::Node.with_no_mods(nil, nil, right_child_value) }
      let!(:root_value) { 10 }
      let!(:root_node) { PersistentTree::Node.with_no_mods(left_child, right_child, root_value) }
      let!(:results) { items_as_array(root_node, 1) }

      it 'output should contain the three nodes, in order' do
        expect(results).to eq([left_child, root_node, right_child])
      end
    end
  end
end
