public class AddLinkedLists {

    public static LinkedListNode<Long> addListsBERecursive(LinkedListNode<Long> listOne, LinkedListNode<Long> listTwo) {
        Long value = 0L;
        if (listOne != null || listTwo != null) {
            if (listOne != null) {
                value += listOne.getValue();
                listOne = listOne.getNext();
            }
            if (listTwo != null) {
                value += listTwo.getValue();
                listTwo = listTwo.getNext();
            }

            LinkedListNode<Long> next = addListsBERecursive(listOne, listTwo);
            Long carry = 0L;
            if (next != null) {
                while (next.getValue() >= 10) {
                    next = new LinkedListNode<>(next.getValue() - 10, next.getNext());
                    carry++;
                }
            }

            value += carry;

            return new LinkedListNode<>(value, next);
        }

        return null;
    }

    public static LinkedListNode<Long> addListsBE(LinkedListNode<Long> listOne, LinkedListNode<Long> listTwo) {
        // Pad lists if necessary
        LinkedListNode<Long> startOne = listOne;
        LinkedListNode<Long> startTwo = listTwo;

        while (listOne != null || listTwo != null) {
            if (listOne == null) {
                startOne = new LinkedListNode<>(0L, startOne);
            } else {
                listOne = listOne.getNext();
            }

            if (listTwo == null) {
                startTwo = new LinkedListNode<>(0L, startTwo);
            } else {
                listTwo = listTwo.getNext();
            }
        }

        return addListsBERecursive(startOne, startTwo);
    }

    public static LinkedListNode<Long> addListsLE(LinkedListNode<Long> listOne, LinkedListNode<Long> listTwo) {
        LinkedListNode<Long> markerOne = listOne;
        LinkedListNode<Long> markerTwo = listTwo;

        LinkedListNode<Long> result = null;
        LinkedListNode<Long> last = null;

        Long carry = 0L;
        while (markerOne != null || markerTwo != null) {
            Long value = 0L;
            if (markerOne != null) {
                value += markerOne.getValue();
                markerOne = markerOne.getNext();
            }
            if (markerTwo != null) {
                value += markerTwo.getValue();
                markerTwo = markerTwo.getNext();
            }
            value += carry;
            carry = 0L;
            while (value >= 10) {
                value -= 10;
                carry += 1;
            }
            if (last == null) {
                last = result = new LinkedListNode<>(value);
            } else {
                last = last.append(value);
            }
        }

        if (carry > 0) {
            last.append(carry);
        }

        return result;
    }

    public static void main(String[] args) {
        LinkedListNode<Long> listOne = new LinkedListNode<>(1L);
        listOne.append(2L).append(3L).append(4L);

        LinkedListNode<Long> listTwo = new LinkedListNode<>(2L);
        listTwo.append(4L).append(6L).append(8L).append(0L);

        LinkedListNode<Long> resultLE = addListsLE(listOne, listTwo);

        StringBuilder sLE = new StringBuilder();
        while (resultLE != null) {
            sLE.append(resultLE.getValue());
            resultLE = resultLE.getNext();
        }

        System.out.println(sLE.reverse());

        LinkedListNode<Long> resultBE = addListsBE(listOne, listTwo);

        StringBuilder sBE = new StringBuilder();
        while (resultBE != null) {
            sBE.append(resultBE.getValue());
            resultBE = resultBE.getNext();
        }

        System.out.print(sBE);
    }

}
