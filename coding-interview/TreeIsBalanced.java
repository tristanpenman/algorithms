import java.util.function.BiPredicate;

public class TreeIsBalanced {

    public static <E> int subtreeDepth(BinaryTreeNode<E> node, BiPredicate<Integer, Integer> terminationPredicate) {
        int leftDepth = 0;
        if (node.getLeftChild() != null) {
            leftDepth = subtreeDepth(node.getLeftChild(), terminationPredicate);
            if (leftDepth == -1) {
                return -1;
            }
        }

        int rightDepth = 0;
        if (node.getRightChild() != null) {
            rightDepth = subtreeDepth(node.getRightChild(), terminationPredicate);
            if (rightDepth == -1) {
                return -1;
            }
        }

        if (terminationPredicate.test(leftDepth, rightDepth)) {
            return -1;
        }

        return Math.max(leftDepth, rightDepth) +
                ((node.getLeftChild() != null || node.getRightChild() != null) ? 1 : 0);
    }

    public static <E> boolean treeIsBalanced(BinaryTreeNode<E> node) {
        return subtreeDepth(node,
                (Integer leftDepth, Integer rightDepth) -> Math.abs(leftDepth - rightDepth) > 1) != -1;
    }

    public static void main(String[] args) {
        BinaryTreeNode<Integer> root = new BinaryTreeNode<>(10);
        root.setLeftChild(new BinaryTreeNode<>(4));
        root.getLeftChild().setLeftChild(new BinaryTreeNode<>(3));
        //root.getLeftChild().getLeftChild().setLeftChild(new BinaryTreeNode<>(2));
        root.setRightChild(new BinaryTreeNode<>(11));

        System.out.println(treeIsBalanced(root));
    }

}
