import java.util.ArrayList;
import java.util.List;

public class TreeFromSortedArray {

    public static <E> BinaryTreeNode<E> treeFromSortedArray(List<E> sorted) {
        int s = sorted.size();
        if (s == 0) {
            return null;
        } else if (s == 1) {
            return new BinaryTreeNode<>(sorted.get(0));
        } else {
            int i = sorted.size() / 2;   // Rounds down
            BinaryTreeNode<E> node = new BinaryTreeNode<>(sorted.get(i));
            node.setLeftChild(treeFromSortedArray(sorted.subList(0, i)));
            node.setRightChild(treeFromSortedArray(sorted.subList(i + 1, s)));
            return node;
        }
    }

    public static void main(String[] args) {

        List<Integer> list = new ArrayList<>();
        list.add(1);
        list.add(2);
        list.add(3);
        list.add(4);
        list.add(5);
        list.add(6);

        BinaryTreeNode<Integer> root = treeFromSortedArray(list);

        System.out.println(root);
    }

}
