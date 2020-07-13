import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ExpandSpaces {

    public static int whereToStart(char text[]) throws IllegalArgumentException {
        int s = 0;
        int d = text.length - 1;
        while (s <= d) {
            if (text[s] == ' ') {
                if (d >= 0 && text[d] != ' ') {
                    d = -1;
                } else if (d > 0 && d > s + 1 && text[d - 1] == ' ') {
                    d -= 2;
                } else {
                    throw new IllegalArgumentException("String does not contain a valid number of trailing spaces.");
                }
            }
            s++;
        }

        return d;
    }

    public static void expandSpaces(char text[]) throws IllegalArgumentException {
        int src = whereToStart(text);
        int dest = text.length - 1;
        while (src >= 0) {
            char c = text[src--];
            if (c == ' ') {
                text[dest] = '0';
                text[dest - 1] = '2';
                text[dest - 2] = '%';
                dest -= 3;
            } else {
                text[dest] = c;
                dest--;
            }
        }
    }

    private static class WhereToStartTestCase {
        public char[] input;
        public Integer expected;

        public WhereToStartTestCase(String input, Integer expected) {
            this.input = input.toCharArray();
            this.expected = expected;
        }
    }

    private static class ExpandSpacesTestCase {
        public char[] input;
        public char[] expected;

        public ExpandSpacesTestCase(String input, String expected) {
            this.input = input.toCharArray();
            this.expected = expected.toCharArray();
        }
    }

    public static void main(String args[]) {
        List<WhereToStartTestCase> whereToStartTestCases = new ArrayList<>();
        whereToStartTestCases.add(new WhereToStartTestCase(" ", null));
        whereToStartTestCases.add(new WhereToStartTestCase("  ", null));
        whereToStartTestCases.add(new WhereToStartTestCase("   ", 0));
        whereToStartTestCases.add(new WhereToStartTestCase(" hello  ", 5));
        whereToStartTestCases.add(new WhereToStartTestCase("hello ", null));
        whereToStartTestCases.add(new WhereToStartTestCase("hello   ", 5));
        whereToStartTestCases.add(new WhereToStartTestCase("hello world  ", 10));

        for (WhereToStartTestCase testCase : whereToStartTestCases) {
            try {
                int actual = whereToStart(testCase.input);
                if (testCase.expected == null) {
                    System.err.println("Failed for input '" + new String(testCase.input) + "; " +
                            "expected exception to be thrown");
                } else if (testCase.expected != actual) {
                    System.err.println("Failed for input '" + new String(testCase.input) + "'; expected " +
                            testCase.expected.toString() + ", but actual was " + actual);
                }
            } catch (Exception ex) {
                if (testCase.expected != null) {
                    System.err.println("Failed for input '" + new String(testCase.input) + "'; expected " +
                            testCase.expected.toString() + ", but exception was thrown");
                }
            }
        }

        List<ExpandSpacesTestCase> expandSpacesTestCases = new ArrayList<>();
        expandSpacesTestCases.add(new ExpandSpacesTestCase("", ""));
        expandSpacesTestCases.add(new ExpandSpacesTestCase("   ", "%20"));
        expandSpacesTestCases.add(new ExpandSpacesTestCase("hello   ", "hello%20"));
        expandSpacesTestCases.add(new ExpandSpacesTestCase("hello world  ", "hello%20world"));
        expandSpacesTestCases.add(new ExpandSpacesTestCase("hello world     ", "hello%20world%20"));
        expandSpacesTestCases.add(new ExpandSpacesTestCase("hello  world       ", "hello%20%20world%20"));
        expandSpacesTestCases.add(new ExpandSpacesTestCase("hello  world          ", "hello%20%20world%20%20"));
        expandSpacesTestCases.add(new ExpandSpacesTestCase(" hello  world            ", "%20hello%20%20world%20%20"));

        for (ExpandSpacesTestCase testCase : expandSpacesTestCases) {
            char[] actual = testCase.input.clone();
            try {
                expandSpaces(actual);
                if (!Arrays.equals(actual, testCase.expected)) {
                    System.err.println("Failed for input '" + new String(testCase.input) + "'; expected '" +
                            Arrays.toString(testCase.expected) + "'");
                }
            } catch (Exception ex) {
                System.err.println("Failed for input '" + new String(testCase.input) +
                        "'; invalid number of trailing spaces");
            }
        }
    }

}
