#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
void cleardfs();
int dfs(int currRow, int currCol, const std::string& path, int endRow, int endCol, int** maze, int row, int num, std::string& pathFound);
