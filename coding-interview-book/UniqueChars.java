import java.util.HashSet;
import java.util.Set;

public class UniqueChars {

    public static boolean uniqueChars(String arg) {
        Set<Character> charsUsed = new HashSet<>(arg.length());
        for (int i = 0; i < arg.length(); i++) {
            if (!charsUsed.add(arg.charAt(i))) {
                return false;
            }
        }

        return true;
    }

    public static void main(String args[]) {
        String s = "tes  ";
        if (uniqueChars(s)) {
            System.out.println("Unique");
        } else {
            System.out.println("Not unique");
        }
    }
}
