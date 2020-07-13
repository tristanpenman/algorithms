public class DetectCycles {

    public static <E> LinkedListNode<E> detectCycle(LinkedListNode<E> leader) {
        LinkedListNode<E> chaser = leader;
        do {
            if (leader == null || leader.getNext() == null) {
                return null;
            }
            leader = leader.getNext().getNext();
            chaser = chaser.getNext();
        } while (leader != chaser);

        return leader;
    }

    public static <E> LinkedListNode<E> findStartOfCycle(LinkedListNode<E> list) {
        LinkedListNode<E> leader = detectCycle(list);
        if (leader == null) {
            return null;
        }

        LinkedListNode<E> chaser = list;
        while (leader != chaser) {
            leader = leader.getNext();
            chaser = chaser.getNext();
        }

        return chaser;
    }

    public static void main(String[] args) {
        LinkedListNode<Long> a = new LinkedListNode<>(0L);
        LinkedListNode<Long> b = new LinkedListNode<>(1L);
        LinkedListNode<Long> c = new LinkedListNode<>(2L);
        LinkedListNode<Long> d = new LinkedListNode<>(3L);
        d.setNext(a);
        a.setNext(b);
        b.setNext(c);
        c.setNext(d);

        LinkedListNode<Long> cycle = findStartOfCycle(a);
        if (cycle == null) {
            System.out.println("No cycles");
        } else {
            System.out.println("Cycle found at node " + cycle.getValue());
        }
    }
}
