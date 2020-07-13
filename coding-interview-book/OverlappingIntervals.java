import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class OverlappingIntervals {

    private static class Interval {
        public int begin;
        public int end;   /* inclusive */
        public Interval(int begin, int end) {
            this.begin = begin;
            this.end = end;
        }
    }

    public static int mostOpenIntervals(List<Interval> intervals) {
        int[] open = new int[intervals.size()];
        int[] close = new int[intervals.size()];

        int i = 0;
        for(Interval interval : intervals) {
            open[i] = interval.begin;
            close[i] = interval.end;
            i++;
        }

        Arrays.sort(open);
        Arrays.sort(close);

        int numberOfOpenIntervals = 0;
        int maxNumberOfOpenIntervals = 0;
        int beginningOfMax = 0;

        int beginIdx = 0;
        int endIdx = 0;
        while(beginIdx < open.length && endIdx < close.length) {
            if (open[beginIdx] <= close[endIdx]) {
                numberOfOpenIntervals++;
                if (numberOfOpenIntervals > maxNumberOfOpenIntervals) {
                    maxNumberOfOpenIntervals = numberOfOpenIntervals;
                    beginningOfMax = open[beginIdx];
                }
                beginIdx++;
            } else {
                numberOfOpenIntervals--;
                endIdx++;
            }
        }

        return beginningOfMax;
    }

    public static void main(String[] args) {
        List<Interval> intervals = new ArrayList<>();
        intervals.add(new Interval(1, 8));
        intervals.add(new Interval(2, 5));
        intervals.add(new Interval(6, 7));

        int result = mostOpenIntervals(intervals);

        System.out.println(result);
    }

}
