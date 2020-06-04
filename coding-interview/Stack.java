public class Stack<E> {
    LinkedListNode<E> top;

    public boolean isEmpty() {
        return top == null;
    }

    public E peek() {
        if (top == null) {
            throw new IllegalStateException("Stack is empty");
        }

        return top.getValue();
    }

    public E pop() {
        if (top == null) {
            throw new IllegalStateException("Stack is empty");
        }

        E value = top.getValue();
        top = top.getNext();
        return value;
    }

    public void push(E value) {
        if (top == null) {
            top = new LinkedListNode<>(value);
        } else {
            top = new LinkedListNode<>(value, top);
        }
    }

}
