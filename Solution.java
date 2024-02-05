
import java.util.Arrays;
import java.util.PriorityQueue;

public class Solution {

    private record Query(int value, int originalIndex) {}

    private record Position(int row, int column, int value) {}

    private static final int[][] moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    private int[][] matrix;
    private boolean[][] visitedForAllQueries;
    private PriorityQueue<Position> minHeapForValue;

    private int rows;
    private int columns;

    public int[] maxPoints(int[][] matrix, int[] originalQueries) {
        this.matrix = matrix;
        this.rows = matrix.length;
        this.columns = matrix[0].length;

        return calculatePointsForAllQueries(originalQueries);
    }

    private int[] calculatePointsForAllQueries(int[] originalQueries) {
        Position start = new Position(0, 0, matrix[0][0]);
        Query[] queriesSortedByValueAscending = createQueriesSortedByValueAscending(originalQueries);

        minHeapForValue = new PriorityQueue<>((x, y) -> x.value - y.value);
        minHeapForValue.add(start);

        visitedForAllQueries = new boolean[rows][columns];
        visitedForAllQueries[start.row][start.column] = true;

        int pointsPreviousQuery = 0;
        int[] maxPoints = new int[originalQueries.length];

        for (Query query : queriesSortedByValueAscending) {
            maxPoints[query.originalIndex] = calculatePointsForCurrentQuery(query.value, pointsPreviousQuery);
            pointsPreviousQuery = maxPoints[query.originalIndex];
        }
        return maxPoints;
    }

    private int calculatePointsForCurrentQuery(int currentQueryValue, int pointsPreviousQuery) {
        int pointsCurrentQuery = pointsPreviousQuery;

        while (!minHeapForValue.isEmpty() && minHeapForValue.peek().value < currentQueryValue) {

            Position current = minHeapForValue.poll();
            ++pointsCurrentQuery;

            for (int[] move : moves) {
                int nextRow = current.row + move[0];
                int nextColumn = current.column + move[1];

                if (isInMatrix(nextRow, nextColumn) && !visitedForAllQueries[nextRow][nextColumn]) {
                    minHeapForValue.add(new Position(nextRow, nextColumn, matrix[nextRow][nextColumn]));
                    visitedForAllQueries[nextRow][nextColumn] = true;
                }
            }
        }
        return pointsCurrentQuery;
    }

    private Query[] createQueriesSortedByValueAscending(int[] originalQueries) {
        Query[] queriesSortedByValueAscending = new Query[originalQueries.length];

        for (int i = 0; i < originalQueries.length; ++i) {
            queriesSortedByValueAscending[i] = new Query(originalQueries[i], i);
        }
        Arrays.sort(queriesSortedByValueAscending, (x, y) -> x.value - y.value);

        return queriesSortedByValueAscending;
    }

    private boolean isInMatrix(int row, int column) {
        return row >= 0 && row < rows && column >= 0 && column < columns;
    }
}
