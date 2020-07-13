import java.util.Arrays;

public class IsPermutation {

    public static boolean isPermutation(String first, String second) {
        if (first.length() != second.length()) {
            return false;
        }

        char firstArr[] = first.toCharArray();
        Arrays.sort(firstArr);

        char secondArr[] = second.toCharArray();
        Arrays.sort(secondArr);

        return Arrays.equals(firstArr, secondArr);
    }

    public static void main(String args[]) {
        if (isPermutation("dog", "")) {
            System.out.println("Permutation");
        } else {
            System.out.println("Not permutation");
        }
    }

}
