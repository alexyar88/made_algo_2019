/*
11_1. Цикл минимальной длины
Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
 * */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

class Graph {
public:

    Graph(int _size) : size(_size), graph_array(_size) {}

    void Add(int from, int to);

    const int size;

    std::unordered_set<int> GetChildPoints(int point);

private:
    std::vector<std::unordered_set<int>> graph_array;

};

void Graph::Add(int from, int to) {
    graph_array[from].insert(to);
    graph_array[to].insert(from);
}


std::unordered_set<int> Graph::GetChildPoints(int point) {
    return graph_array[point];
}

// Делаем BFS, находим цикл в узле
int BFS(Graph &graph, int start) {
    std::queue<int> points_queue;
    points_queue.push(start);

    std::vector<bool> visited(graph.size, false); // помечаем, какие узлы прошли
    std::vector<int> parent(graph.size, -1); // чтобы проверять, не вернулись ли в родителя
    std::vector<int> path_len(graph.size, 0); // собираем длину пути сюда
    visited[start] = true;

    while (!points_queue.empty()) {
        int point = points_queue.front();
        points_queue.pop();

        auto children = graph.GetChildPoints(point);
        for (int child : children) {
            if (!visited[child]) {
                parent[child] = point;
                visited[child] = true;
                // Т.к. мы от point на расстоянии 1 ребра, путь к текущему узлу = путь к point + 1
                path_len[child] = path_len[point] + 1;
                points_queue.push(child);
            } else if (parent[point] != child) { // Проверяем, не вернулись ли мы назад
                // Нашли цикл! Длина цикла = путь до point + путь до child + 1 ребро, чтобы замкнуть цикл
                return path_len[point] + path_len[child] + 1;
            }
        }
    }
    return graph.size + 1; // Ничего не нашли
}

// Делаем BFS от каждого узла, находим минимальный цикл
int MinCycleSize(Graph &graph) {
    // длина цикла не больше числа узлов, поэтому за максимум берем число узлов + 1
    int min_cycle_size = graph.size + 1;
    for (int i = 0; i < graph.size; ++i) {
        int point_cycle_size = BFS(graph, i);
        if (min_cycle_size > point_cycle_size) {
            min_cycle_size = point_cycle_size;
        }
    }
    // Ничего не нашли
    if (min_cycle_size == graph.size + 1) {
        return -1;
    }
    return min_cycle_size;
}


int main() {
    int points_count = 0;
    int edges_count = 0;
    std::cin >> points_count >> edges_count;
    Graph graph(points_count);
    int from = 0;
    int to = 0;
    for (int i = 0; i < edges_count; ++i) {
        std::cin >> from >> to;
        graph.Add(from, to);
    }
    std::cout << MinCycleSize(graph);
    return 0;
}

