import java.util.Arrays;

public class RotateImageMatrix {

    public static void rotateImageMatrix(int[][] matrix, int n) {
        if (n < 1) {
            throw new IllegalArgumentException("Matrix side length must be positive");
        }

        for (int depth = 0; depth < n / 2; depth++) {
            for (int i = depth; i < n - depth - 1; i++) {
                int last = n - depth - 1;   // Index of last column or row

                // Save top
                int temp = matrix[depth][i];

                // Right -> top
                matrix[depth][i] = matrix[i][last];
                // Bottom -> right
                matrix[i][last] = matrix[last][n - i - 1];
                // Left -> bottom
                matrix[last][n - i - 1] = matrix[n - i - 1][depth];

                // Top -> left
                matrix[n - i - 1][depth] = temp;
            }
        }
    }

    public static void main(String args[]) {

        int[][] val = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

        System.out.println(Arrays.deepToString(val));

        rotateImageMatrix(val, 4);

        System.out.println(Arrays.deepToString(val));

        int[][] val2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        System.out.println(Arrays.deepToString(val2));

        rotateImageMatrix(val2, 3);

        System.out.println(Arrays.deepToString(val2));

    }

}
