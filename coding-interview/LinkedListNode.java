import sun.awt.image.ImageWatched;

public class LinkedListNode<E> {

    private E value;
    private LinkedListNode<E> next;

    public LinkedListNode(E value) {
        this.value = value;
        this.next = null;
    }

    public LinkedListNode(E value, LinkedListNode<E> next) {
        this.value = value;
        this.next = next;
    }

    public E getValue() {
        return value;
    }

    public LinkedListNode<E> prepend(E value) {
        LinkedListNode<E> newHead = new LinkedListNode<>(value);
        newHead.next = this;
        return newHead;
    }

    public LinkedListNode<E> append(E value) {
        LinkedListNode<E> newTail = new LinkedListNode<>(value);
        LinkedListNode<E> current = this;
        while (current.next != null) {
            current = current.next;
        }
        current.next = newTail;
        return newTail;
    }

    public void remove() {
        if (next == null) {
            throw new IllegalStateException("Node cannot remove itself from end of list.");
        } else {
            value = next.value;
            next = next.next;
        }
    }

    public static <E> LinkedListNode<E> delete(LinkedListNode<E> head, E value) {
        if (head.value == value) {
            return head.next;
        }

        LinkedListNode<E> current = head;
        while (current.next != null) {
            if (current.next.value == value) {
                current.next = current.next.next;
                break;
            }
        }

        return head;
    }

    public void setNext(LinkedListNode<E> node) {
        this.next = node;
    }

    public LinkedListNode<E> getNext() {
        return next;
    }
}
