import java.util.*;

public class KthLastInList {

    public static <E> LinkedListNode<E> kthLastInList(int k, LinkedListNode<E> head) {
        if (k < 1) {
            throw new IllegalArgumentException("k must be greater than or equal to 1");
        }

        LinkedListNode<E> leader = head;
        for (int headStart = 0; headStart < k - 1; headStart++) {
            leader = leader.getNext();
            if (leader == null) {
                throw new IllegalArgumentException("Linked list must be at least " + k + " items in length");
            }
        }

        LinkedListNode<E> chaser = head;
        while ((leader = leader.getNext()) != null) {
            chaser = chaser.getNext();
        }

        return chaser;
    }

    public static void main(String[] args) {

        LinkedListNode<Integer> head = new LinkedListNode<>(10);

        head.append(11).append(12).append(13);

        LinkedListNode<Integer> result = kthLastInList(4, head);

        System.out.println(result.getValue());
    }
}
