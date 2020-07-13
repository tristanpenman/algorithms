public class SortStack {

    public static void sortStack(Stack<Integer> s) {
        Stack<Integer> w = new Stack<>();
        while (!s.isEmpty()) {
            Integer current = s.pop();
            int count = 0;
            while (!w.isEmpty() && current < w.peek()) {
                s.push(w.pop());
                count++;
            }
            w.push(current);
            for (int i = 0; i < count; ++i) {
                w.push(s.pop());
            }
        }
        while (!w.isEmpty()) {
            s.push(w.pop());
        }
    }

    public static void main(String[] args) {
        Stack<Integer> s = new Stack<>();
        s.push(10);
        s.push(2);
        s.push(5);
        s.push(5);
        s.push(3);
        s.push(8);
        s.push(9);
        s.push(1);

        sortStack(s);
        sortStack(s);

        while (!s.isEmpty()) {
            System.out.println(s.pop());
        }

        s.push(1);
        s.push(2);
        s.push(3);

        sortStack(s);

        while (!s.isEmpty()) {
            System.out.println(s.pop());
        }
    }

}
