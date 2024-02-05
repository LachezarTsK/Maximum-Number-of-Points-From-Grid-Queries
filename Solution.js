
/**
 * @param {number[][]} matrix
 * @param {number[]} originalQueries
 * @return {number[]}
 */
var maxPoints = function (matrix, originalQueries) {
    this.moves = [[-1, 0], [1, 0], [0, -1], [0, 1]];
    this.matrix = matrix;
    this.rows = matrix.length;
    this.columns = matrix[0].length;    
    this.visitedForAllQueries = Array.from(new Array(this.rows), () => new Array(this.columns).fill(false));

    // MinPriorityQueue<Position> 
    // const {MinPriorityQueue} = require('@datastructures-js/priority-queue');
    this.minHeapForValue = new MinPriorityQueue({compare: (x, y) => x.value - y.value});

    return calculatePointsForAllQueries(originalQueries);
};

/**
 * @param {number} value
 * @param {number} originalIndex
 */
function Query(value, originalIndex) {
    this.value = value;
    this.originalIndex = originalIndex;
}

/**
 * @param {number} row
 * @param {number} column
 * @param {number} value
 */
function Position(row, column, value) {
    this.row = row;
    this.column = column;
    this.value = value;
}

/**
 * @param {number[]} originalQueries
 * @return {number[]}
 */
function calculatePointsForAllQueries(originalQueries) {
    const start = new Position(0, 0, this.matrix[0][0]);
    const queriesSortedByValueAscending = createQueriesSortedByValueAscending(originalQueries);

    this.minHeapForValue.enqueue(start);
    this.visitedForAllQueries[start.row][start.column] = true;

    let pointsPreviousQuery = 0;
    const maxPoints = new Array(originalQueries.length).fill(0);

    for (let query of queriesSortedByValueAscending) {
        maxPoints[query.originalIndex] = calculatePointsForCurrentQuery(query.value, pointsPreviousQuery);
        pointsPreviousQuery = maxPoints[query.originalIndex];
    }
    return maxPoints;
}

/**
 * @param {number} currentQueryValue
 * @param {number} pointsPreviousQuery
 * @return {number}
 */
function  calculatePointsForCurrentQuery(currentQueryValue, pointsPreviousQuery) {
    let pointsCurrentQuery = pointsPreviousQuery;

    while (!this.minHeapForValue.isEmpty() && this.minHeapForValue.front().value < currentQueryValue) {

        const current = this.minHeapForValue.dequeue();
        ++pointsCurrentQuery;

        for (let move of this.moves) {
            const nextRow = current.row + move[0];
            const nextColumn = current.column + move[1];

            if (isInMatrix(nextRow, nextColumn) && !this.visitedForAllQueries[nextRow][nextColumn]) {
                this.minHeapForValue.enqueue(new Position(nextRow, nextColumn, this.matrix[nextRow][nextColumn]));
                visitedForAllQueries[nextRow][nextColumn] = true;
            }
        }
    }
    return pointsCurrentQuery;
}

/**
 * @param {number[]} originalQueries
 * @return {Query[]}
 */
function createQueriesSortedByValueAscending(originalQueries) {
    const queriesSortedByValueAscending = new Array(originalQueries.length);

    for (let i = 0; i < originalQueries.length; ++i) {
        queriesSortedByValueAscending[i] = new Query(originalQueries[i], i);
    }
    queriesSortedByValueAscending.sort((x, y) => x.value - y.value);

    return queriesSortedByValueAscending;
}

/**
 * @param {number} row
 * @param {number} column
 * @return {boolean}
 */
function isInMatrix(row, column) {
    return row >= 0 && row < this.rows && column >= 0 && column < this.columns;
}
