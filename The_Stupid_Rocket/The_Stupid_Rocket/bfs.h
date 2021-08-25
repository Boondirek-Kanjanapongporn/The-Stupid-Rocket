#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <queue>
#include <map>
void clearbfs();
int bfs(int startRow, int startCol, int endRow, int endCol, int** maze, int row, int col, std::string& pathFound);
int explore(int exRow, int exCol, const std::string& exPosition, int** maze, int row, int col);
std::string reconstructPath(int sRow, int sCol, int eRow, int eCol);
