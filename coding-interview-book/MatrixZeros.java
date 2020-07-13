import java.util.Arrays;
import java.util.BitSet;

public class MatrixZeros {

    public static void infectWithZeros(int[][] m, int rows, int columns) {
        BitSet zeroCols = new BitSet(columns);
        BitSet zeroRows = new BitSet(rows);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                if (m[r][c] == 0) {
                    zeroCols.set(c);
                    zeroRows.set(r);
                }
            }
        }

        for (int c = zeroCols.nextSetBit(0); c >= 0; c = zeroCols.nextSetBit(c + 1)) {
            for (int r = 0; r < rows; r++) {
                m[r][c] = 0;
            }
        }

        for (int r = zeroRows.nextSetBit(0); r >= 0; r = zeroRows.nextSetBit(r + 1)) {
            for (int c = 0; c < columns; c++) {
                m[r][c] = 0;
            }
        }
    }

    public static void main(String args[]) {

        int[][] val = {{1, 0, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}};

        System.out.println(Arrays.deepToString(val));

        infectWithZeros(val, 4, 4);

        System.out.println(Arrays.deepToString(val));

    }
}
