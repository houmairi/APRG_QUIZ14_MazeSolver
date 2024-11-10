#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;

const int SIZE = 41;
const int START_X = 1;
const int START_Y = 0;
const int END_X = 39;
const int END_Y = 39;

struct Coord {
    int x, y;
    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator<(const Coord& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

struct HashCoord {
    size_t operator()(const Coord& c) const {
        return c.x * 31 + c.y;
    }
};

int heuristic(int x, int y, int end_x, int end_y) {
    return abs(end_x - x) + abs(end_y - y);
}

struct Node {
    Coord pos;
    int f, g;
    
    Node(Coord p, int g_, int h) : pos(p), g(g_) {
        f = g + h;
    }
    
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

string replaceOIfNeeded(const string& input) {
    if (input.size() != 5 || input[2] != '\n') {
        return "Invalid Input!";
    }
    char grid[2][2] = {{input[0], input[1]}, {input[3], input[4]}};
    int o_count = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (grid[i][j] == 'o') ++o_count;
        }
    }
    if (o_count == 3) {
        if (grid[0][0] != 'o')
            grid[1][1] = '.';
        else if (grid[0][1] != 'o')
            grid[1][0] = '.';
        else if (grid[1][1] != 'o')
            grid[0][0] = '.';
        else
            grid[0][1] = '.';
    }
    
    string result;
    result += grid[0][0];
    result += grid[0][1];
    result += '\n';
    result += grid[1][0];
    result += grid[1][1];
    return result;
}

vector<string> processSolution(vector<string>& solution) {
    // Skip processing the first row by starting from y = 1
    for (size_t y = 1; y < solution.size() - 1; ++y) {
        for (size_t x = 0; x < solution[y].size() - 1; ++x) {
            string gridSegment;
            gridSegment += solution[y][x];
            gridSegment += solution[y][x + 1];
            gridSegment += '\n';
            gridSegment += solution[y + 1][x];
            gridSegment += solution[y + 1][x + 1];
            
            string modifiedSegment = replaceOIfNeeded(gridSegment);
            if (modifiedSegment != gridSegment) {
                solution[y][x] = modifiedSegment[0];
                solution[y][x + 1] = modifiedSegment[1];
                solution[y + 1][x] = modifiedSegment[3];
                solution[y + 1][x + 1] = modifiedSegment[4];
            }
        }
    }
    return solution;
}

void solveMaze() {
    vector<string> maze(SIZE);
    for (int i = 0; i < SIZE; i++) {
        getline(cin, maze[i]);
        if (!maze[i].empty() && maze[i].back() == '\r') {
            maze[i].pop_back();
        }
    }
    
    priority_queue<Node, vector<Node>, greater<Node>> open;
    unordered_map<Coord, int, HashCoord> g_cost;
    unordered_map<Coord, Coord, HashCoord> came_from;
    
    Coord start{START_X, START_Y};
    Coord goal{END_X, END_Y};
    
    open.push(Node(start, 0, heuristic(start.x, start.y, goal.x, goal.y)));
    g_cost[start] = 0;
    
    vector<Coord> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    while (!open.empty()) {
        Node current = open.top();
        open.pop();
        
        if (current.pos.x == goal.x && current.pos.y == goal.y) {
            Coord path = goal;
            while (!(path.x == start.x && path.y == start.y)) {
                // Don't modify the first row when marking the path
                if (path.y > 0) {
                    maze[path.y][path.x] = 'o';
                }
                path = came_from[path];
            }
            // Special handling for start position if it's in first row
            if (start.y > 0) {
                maze[start.y][start.x] = 'o';
            }
            
            vector<string> processed = processSolution(maze);
            for (const auto& row : processed) {
                cout << row << '\n';
            }
            return;
        }
        
        for (const auto& dir : directions) {
            Coord next{current.pos.x + dir.x, current.pos.y + dir.y};
            
            if (next.x >= 0 && next.x < SIZE && next.y >= 0 && next.y < SIZE && 
                maze[next.y][next.x] != '#') {
                
                int new_cost = g_cost[current.pos] + 1;
                if (g_cost.find(next) == g_cost.end() || new_cost < g_cost[next]) {
                    g_cost[next] = new_cost;
                    open.push(Node(next, new_cost, heuristic(next.x, next.y, goal.x, goal.y)));
                    came_from[next] = current.pos;
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solveMaze();
    return 0;
}