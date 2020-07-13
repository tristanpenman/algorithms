public class Palindrome {

    public static <E> boolean isPalindrome(LinkedListNode<E> list) {
        LinkedListNode<E> leader = list;
        LinkedListNode<E> chaser = list;

        Stack<E> seen = new Stack<>();

        while (leader != null && leader.getNext() != null) {
            leader = leader.getNext().getNext();
            seen.push(chaser.getValue());
            chaser = chaser.getNext();
        }

        if (leader != null) {
            chaser = chaser.getNext();
        }

        while (chaser != null && !seen.isEmpty() && seen.pop().equals(chaser.getValue())) {
            chaser = chaser.getNext();
        }

        return chaser == null && seen.isEmpty();
    }

    public static void main(String[] args) {
        LinkedListNode<Integer> numbers = new LinkedListNode<>(10);
        numbers.append(2).append(5).append(5).append(2).append(10);

        LinkedListNode<Integer> current = numbers;
        StringBuilder str = new StringBuilder();
        while (current != null) {
            str.append(current.getValue());
            str.append(" ");
            current = current.getNext();
        }

        System.out.print(str);

        if (isPalindrome(numbers)) {
            System.out.println("Is palindrome");
        } else {
            System.out.println("Not a palindrome");
        }
    }

}
