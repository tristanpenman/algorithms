public class BinaryTreeNode<E> {

    private E value;

    BinaryTreeNode<E> left;
    BinaryTreeNode<E> right;

    public BinaryTreeNode(E value) {
        this.value = value;
    }

    public BinaryTreeNode<E> getLeftChild() {
        return left;
    }

    public BinaryTreeNode<E> getRightChild() {
        return right;
    }

    public E getValue() {
        return value;
    }

    public void setLeftChild(BinaryTreeNode<E> left) {
        this.left = left;
    }

    public void setRightChild(BinaryTreeNode<E> right) {
        this.right = right;
    }
}
