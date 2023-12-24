#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

typedef pair<int, int> Point;

bool isValid(int x, int y, const vector<vector<char>> &maze, const vector<vector<bool>> &visited) {
    int rows = maze.size();
    int cols = maze[0].size();
    return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == '.' && !visited[x][y];
}

vector<Point> bfs(const vector<vector<char>> &maze, Point start, Point goal) {
    int rows = maze.size();
    int cols = maze[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<Point> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    queue<pair<Point, vector<Point>>> q;
    q.push({start, {}});

    while (!q.empty()) {
        Point current = q.front().first;
        vector<Point> path = q.front().second;
        q.pop();

        int x = current.first;
        int y = current.second;

        if (current == goal) {
            return path;
        }

        if (isValid(x, y, maze, visited)) {
            visited[x][y] = true;
            for (const auto &dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;
                if (isValid(nx, ny, maze, visited)) {
                    q.push({{nx, ny}, path});
                }
            }
        }
    }

    return {};  // No path found
}

int main() {
    vector<vector<char>> maze = {
        {'S', '.', '.', '#', '.', '.', '.'},
        {'.', '.', '.', '#', '.', '.', '.'},
        {'.', '.', '.', '#', '.', '.', '.'},
        {'.', '.', '.', '#', '.', '.', '.'},
        {'.', '.', '.', '#', '.', '.', '.'},
        {'.', '.', '.', '#', '.', '.', 'G'}
    };

    Point start = {0, 0};
    Point goal = {5, 6};

    vector<Point> result = bfs(maze, start, goal);

    if (!result.empty()) {
        cout << "Shortest path:";
        for (const auto &point : result) {
            cout << " (" << point.first << "," << point.second << ")";
        }
        cout << endl;
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
