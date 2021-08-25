#include "dfs.h"
std::unordered_set<std::string> trackerDFS;

void cleardfs() {
    trackerDFS.clear();
}

int dfs(int currRow, int currCol, const std::string& path, int endRow, int endCol, int** maze, int row, int col, std::string &pathFound) {
    std::string position = "[" + std::to_string(currRow) + "," + std::to_string(currCol) + "] ";
    std::string currPath = path + position;

    if (currRow == endRow && currCol == endCol) {
        if (pathFound.empty()) {
            pathFound = currPath;
        }
        return 1;
    }
    if (trackerDFS.find(position) != trackerDFS.end()) {
        return 0;
    }
    if (currRow > -1 && currRow < row && currCol > -1 && currCol < col && maze[currRow][currCol] != 1) {
        trackerDFS.insert(position);
        return dfs(currRow - 1, currCol - 1, currPath, endRow, endCol, maze, row, col, pathFound) // up left
            + dfs(currRow - 1, currCol + 1, currPath, endRow, endCol, maze, row, col, pathFound) // up right
            + dfs(currRow + 1, currCol + 1, currPath, endRow, endCol, maze, row, col, pathFound) // down right
            + dfs(currRow + 1, currCol - 1, currPath, endRow, endCol, maze, row, col, pathFound) // down left
            + dfs(currRow - 1, currCol, currPath, endRow, endCol, maze, row, col, pathFound) // up
            + dfs(currRow + 1, currCol, currPath, endRow, endCol, maze, row, col, pathFound) // down
            + dfs(currRow, currCol - 1, currPath, endRow, endCol, maze, row, col, pathFound) // left
            + dfs(currRow, currCol + 1, currPath, endRow, endCol, maze, row, col, pathFound); // right
    }
    return 0;
}
