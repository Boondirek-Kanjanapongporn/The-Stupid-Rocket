#include "bfs.h"
std::unordered_set<std::string> trackerBFS;
std::queue<int> Rowqueue;
std::queue<int> Colqueue;
std::map<std::string, std::string> parent;
std::vector <std::string> successfulPathDFS;

void clearbfs() {
    trackerBFS.clear();
    Rowqueue = std::queue <int>();
    Colqueue = std::queue <int>();
    parent.clear();
    successfulPathDFS.clear();
}

std::string reconstructPath(int sRow, int sCol, int eRow, int eCol) {
    std::string copyPath;
    std::string start = "[" + std::to_string(sRow) + "," + std::to_string(sCol) + "] ";
    std::string end = "[" + std::to_string(eRow) + "," + std::to_string(eCol) + "] ";
    for (auto curr = end; curr != start; curr = parent[curr]) {
        successfulPathDFS.push_back(curr);
    }
    successfulPathDFS.push_back(start);
    reverse(successfulPathDFS.begin(), successfulPathDFS.end());
    for (const auto& i : successfulPathDFS)
        copyPath += i;
    return copyPath;
}

int explore(int exRow, int exCol, const std::string& exPosition, int** maze, int row, int col) {
    int i, nRow, nCol;
    char Row_direction[] = { -1,-1,1,1,-1,1,0,0 };
    char Col_direction[] = { -1,1,1,-1,0,0,-1,1 };
    for (i = 0; i < 8; i++) {
        nRow = exRow + Row_direction[i];
        nCol = exCol + Col_direction[i];
        std::string nposition = "[" + std::to_string(nRow) + "," + std::to_string(nCol) + "] ";
        if (trackerBFS.find(nposition) != trackerBFS.end())
            continue;
        if (nRow > -1 && nRow < row && nCol > -1 && nCol < col && maze[nRow][nCol] != 1) {
            Rowqueue.push(nRow);
            Colqueue.push(nCol);
            trackerBFS.insert(nposition);
            parent[nposition] = exPosition;
        }
    }
    return 0;
}

int bfs(int startRow, int startCol, int endRow, int endCol, int** maze, int row, int col, std::string& pathFound) {
    int currRow, currCol;
    Rowqueue.push(startRow);
    Colqueue.push(startCol);
    while (!Rowqueue.empty()) {
        currRow = Rowqueue.front();
        currCol = Colqueue.front();
        std::string position = "[" + std::to_string(currRow) + "," + std::to_string(currCol) + "] ";
        Rowqueue.pop();
        Colqueue.pop();
        if (currRow == endRow && currCol == endCol) {
            std::cout << "I have completed the maze, Yeah!" << std::endl;
            pathFound = reconstructPath(startRow, startCol, endRow, endCol);
            return 1;
        }
        explore(currRow, currCol, position, maze, row, col);
    }
    return 0;
}