public class StackBasedQueue<E> {
    private Stack<E> one = new Stack<>();
    private Stack<E> two = new Stack<>();

    public void enqueue(E value) {
        one.push(value);
    }

    public E dequeue() {
        if (two.isEmpty()) {
            while (!one.isEmpty()) {
                two.push(one.pop());
            }
        }

        return two.pop();
    }

    public boolean isEmpty() {
        return one.isEmpty() && two.isEmpty();
    }

    public static void main(String[] args) {
        StackBasedQueue<Integer> queue = new StackBasedQueue<>();
        queue.enqueue(1);
        queue.enqueue(2);
        queue.enqueue(3);
        queue.enqueue(4);

        while (!queue.isEmpty()) {
            System.out.println(queue.dequeue());
        }
    }
}
