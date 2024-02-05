
using System;
using System.Collections.Generic;
using customDataStructures;

public class Solution
{
    private static readonly int[][] moves = new int[4][]
    {
        new int[]{-1, 0}, new int[]{1, 0}, new int[]{0, -1}, new int[]{0, 1}
    };

    private int[][] matrix = null!;
    private bool[,] visitedForAllQueries = null!;
    private PriorityQueue<Position, int> minHeapForValue = null!;

    private int rows;
    private int columns;

    public int[] MaxPoints(int[][] matrix, int[] originalQueries)
    {
        this.matrix = matrix;
        this.rows = matrix.Length;
        this.columns = matrix[0].Length;

        return calculatePointsForAllQueries(originalQueries);
    }

    private int[] calculatePointsForAllQueries(int[] originalQueries)
    {
        Position start = new Position(0, 0, matrix[0][0]);
        Query[] queriesSortedByValueAscending = createQueriesSortedByValueAscending(originalQueries);

        Comparer<int> comparatorAscendingValue = Comparer<int>.Create((x, y) => x.CompareTo(y));
        minHeapForValue = new PriorityQueue<Position, int>(comparatorAscendingValue);
        minHeapForValue.Enqueue(start, start.value);

        visitedForAllQueries = new bool[rows, columns];
        visitedForAllQueries[start.row, start.column] = true;

        int pointsPreviousQuery = 0;
        int[] maxPoints = new int[originalQueries.Length];

        foreach (Query query in queriesSortedByValueAscending)
        {
            maxPoints[query.originalIndex] = calculatePointsForCurrentQuery(query.value, pointsPreviousQuery);
            pointsPreviousQuery = maxPoints[query.originalIndex];
        }
        return maxPoints;
    }

    private int calculatePointsForCurrentQuery(int currentQueryValue, int pointsPreviousQuery)
    {
        int pointsCurrentQuery = pointsPreviousQuery;

        while (minHeapForValue.Count > 0 && minHeapForValue.Peek().value < currentQueryValue)
        {
            Position current = minHeapForValue.Dequeue();
            ++pointsCurrentQuery;

            foreach (int[] move in moves)
            {
                int nextRow = current.row + move[0];
                int nextColumn = current.column + move[1];

                if (isInMatrix(nextRow, nextColumn) && !visitedForAllQueries[nextRow, nextColumn])
                {
                    Position position = new Position(nextRow, nextColumn, matrix[nextRow][nextColumn]);
                    minHeapForValue.Enqueue(position, position.value);
                    visitedForAllQueries[nextRow, nextColumn] = true;
                }
            }
        }
        return pointsCurrentQuery;
    }

    private Query[] createQueriesSortedByValueAscending(int[] originalQueries)
    {
        Query[] queriesSortedByValueAscending = new Query[originalQueries.Length];

        for (int i = 0; i < originalQueries.Length; ++i)
        {
            queriesSortedByValueAscending[i] = new Query(originalQueries[i], i);
        }

        Array.Sort(queriesSortedByValueAscending, (x, y) => x.value - y.value);

        return queriesSortedByValueAscending;
    }

    private bool isInMatrix(int row, int column)
    {
        return row >= 0 && row < rows && column >= 0 && column < columns;
    }
}

namespace customDataStructures
{
    struct Query
    {
        public int value = 0;
        public int originalIndex = 0;

        public Query(int value, int originalIndex)
        {
            this.value = value;
            this.originalIndex = originalIndex;
        }
    }

    struct Position
    {
        public int row = 0;
        public int column = 0;
        public int value = 0;

        public Position(int row, int column, int value)
        {
            this.row = row;
            this.column = column;
            this.value = value;
        }
    }
}
