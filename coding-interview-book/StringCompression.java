/**
 * Created by Tristan on 25/11/2015.
 */
public class StringCompression {

    public static String compressString(String input) {
        if (input.isEmpty()) {
            return "";
        }

        StringBuilder output = new StringBuilder();

        char lastChar = input.charAt(0);
        int consecutiveChars = 1;

        for (int i = 1; i < input.length(); i++) {
            char currentChar = input.charAt(i);
            if (lastChar == currentChar && consecutiveChars < 9) {
                consecutiveChars++;
            } else {
                output.append(lastChar);
                output.append(consecutiveChars);
                lastChar = currentChar;
                consecutiveChars = 1;
            }
        }

        output.append(lastChar);
        output.append(consecutiveChars);

        if (output.length() >= input.length()) {
            return input;
        }

        return output.toString();
    }

    public static void main(String args[]) {
        System.out.println(compressString("testthissssoooonnneeeeeeeeee"));
        System.out.println(compressString("test"));
    }

}
