import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class IsSubtree {

    public static <E> boolean areTreesEqual(BinaryTreeNode<E> left, BinaryTreeNode<E> right) {
        if (left == null && right == null) {
            return true;
        } else if (left == null || right == null) {
            return false;
        } else if (!left.getValue().equals(right.getValue())) {
            return false;
        }

        return areTreesEqual(left.getLeftChild(), right.getLeftChild()) &&
                areTreesEqual(left.getRightChild(), right.getRightChild());
    }

    public static <E> boolean isSubtreeOf(BinaryTreeNode<E> subtree, BinaryTreeNode<E> supertree) {
        if (supertree == null || subtree == null) {
            return false;
        }

        E subtreeRootValue = subtree.getValue();

        // Use breadth-first search to find candidate subtrees
        Queue<BinaryTreeNode<E>> frontier = new LinkedBlockingQueue<>();
        frontier.add(supertree);

        while (!frontier.isEmpty()) {
            BinaryTreeNode<E> current = frontier.poll();

            BinaryTreeNode<E> left = current.getLeftChild();
            if (left != null) {
                if (left.getValue().equals(subtreeRootValue)) {
                    if (areTreesEqual(left, subtree)) {
                        return true;
                    }
                }

                frontier.add(left);
            }

            BinaryTreeNode<E> right = current.getRightChild();
            if (right != null) {
                if (right.getValue().equals(subtreeRootValue)) {
                    if (areTreesEqual(right, subtree)) {
                        return true;
                    }
                }

                frontier.add(right);
            }
        }

        return false;
    }

    public static void main(String[] args) {

        BinaryTreeNode<Integer> supertree = new BinaryTreeNode<>(10);
        supertree.setLeftChild(new BinaryTreeNode<>(4));
        supertree.getLeftChild().setLeftChild(new BinaryTreeNode<>(3));
        supertree.getLeftChild().setRightChild(new BinaryTreeNode<>(6));
        supertree.getLeftChild().getRightChild().setLeftChild(new BinaryTreeNode<>(5));
        supertree.getLeftChild().getLeftChild().setLeftChild(new BinaryTreeNode<>(2));
        supertree.setRightChild(new BinaryTreeNode<>(11));

        BinaryTreeNode<Integer> subtree = new BinaryTreeNode<>(4);
        subtree.setLeftChild(new BinaryTreeNode<>(3));
        subtree.getLeftChild().setLeftChild(new BinaryTreeNode<>(2));
        subtree.setRightChild(new BinaryTreeNode<>(6));
        subtree.getRightChild().setLeftChild(new BinaryTreeNode<>(5));

        if (isSubtreeOf(subtree, supertree)) {
            System.out.println("Is a subtree");
        } else {
            System.out.println("Is not a subtree");
        }
    }
}
