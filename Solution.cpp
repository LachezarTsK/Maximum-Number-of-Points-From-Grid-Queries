
#include <span>
#include <array>
#include <queue>
#include <ranges>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

namespace customDataStructures {

    struct Query {
        int value = 0;
        size_t originalIndex = 0;

        Query() = default;
        Query(int value, size_t originalIndex) : value {value}, originalIndex {originalIndex}{}
    };

    struct Position {
        size_t row = 0;
        size_t column = 0;
        int value = 0;

        Position() = default;
        Position(size_t row, size_t column, int value) : row {row}, column {column}, value {value}{}
    };

    struct comparatorAscendingValue {
        auto operator()(const Position& first, const Position& second) const {
            return first.value > second.value;
        }
    };
}

using namespace customDataStructures;

class Solution {
    
    inline static const array<array<int, 2>, 4> moves { {{-1, 0}, {1, 0}, {0, -1}, {0, 1}} };

    unique_ptr<vector<vector<int>>> matrix;
    vector<vector<bool>> visitedForAllQueries;
    priority_queue<Position, vector<Position>, comparatorAscendingValue> minHeapForValue;

    size_t rows = 0;
    size_t columns = 0;

public:
    vector<int> maxPoints(const vector<vector<int>>& matrix, const vector<int>& originalQueries) {
        this->matrix = make_unique<vector<vector<int>>>(matrix);
        this->rows = matrix.size();
        this->columns = matrix[0].size();

        return calculatePointsForAllQueries(originalQueries);
    }

private:
    // Aletrnatively, prior to C++20: ...(const vector<int>& originalQueries) ...
    vector<int> calculatePointsForAllQueries(span<const int> originalQueries) {
        Position start(0, 0, (*matrix)[0][0]);
        vector<Query> queriesSortedByValueAscending = createQueriesSortedByValueAscending(originalQueries);

        minHeapForValue.push(start);
        visitedForAllQueries.assign(rows, vector<bool>(columns));
        visitedForAllQueries[start.row][start.column] = true;

        int pointsPreviousQuery = 0;
        vector<int> maxPoints(originalQueries.size());

        for (const auto& query : queriesSortedByValueAscending) {
            maxPoints[query.originalIndex] = calculatePointsForCurrentQuery(query.value, pointsPreviousQuery);
            pointsPreviousQuery = maxPoints[query.originalIndex];
        }
        return maxPoints;
    }

    int calculatePointsForCurrentQuery(int currentQueryValue, int pointsPreviousQuery) {
        int pointsCurrentQuery = pointsPreviousQuery;

        while (!minHeapForValue.empty() && minHeapForValue.top().value < currentQueryValue) {

            Position current = minHeapForValue.top();
            minHeapForValue.pop();
            ++pointsCurrentQuery;

            for (const auto& move : moves) {
                size_t nextRow = current.row + move[0];
                size_t nextColumn = current.column + move[1];

                if (isInMatrix(nextRow, nextColumn) && !visitedForAllQueries[nextRow][nextColumn]) {
                    minHeapForValue.emplace(nextRow, nextColumn, (*matrix)[nextRow][nextColumn]);
                    visitedForAllQueries[nextRow][nextColumn] = true;
                }
            }
        }
        return pointsCurrentQuery;
    }

    // Aletrnatively, prior to C++20: ...(const vector<int>& originalQueries) ...
    vector<Query> createQueriesSortedByValueAscending(span<const int> originalQueries) const {
        vector<Query> queriesSortedByValueAscending(originalQueries.size());

        for (size_t i = 0; i < originalQueries.size(); ++i) {
            queriesSortedByValueAscending[i] = Query(originalQueries[i], i);
        }

        auto comparatorAscendingValue = [](const Query& first, const Query & second) {
            return first.value < second.value;
        };

        // Aletrnatively, prior to C++20: 
        // sort(vectorName.begin(), vectorName.end(), comparator);
        ranges::sort(queriesSortedByValueAscending, comparatorAscendingValue);

        return queriesSortedByValueAscending;
    }

    bool isInMatrix(size_t row, size_t column) const {
        // Since 'row' and 'column' are of type size_t, in this particular case, 
        // no need to check for 'row != variant_npos' and 'column != variant_npos'.
        return row < rows && column < columns;
    }
};
