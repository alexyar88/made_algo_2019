/*
Задача 12. Мосты
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа
увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stack>

class Graph {
public:

    Graph(int _size) : size(_size), graph_array(_size), graph_edges_num_array(_size) {}

    void Add(int from, int to, int num);

    const int size;

    std::vector<int> GetChildPoints(int point);

    int GetEdgeNum(int from, int to) const;

private:
    std::vector<std::vector<int>> graph_array;
    // Храним еще и номера ребер
    std::vector<std::vector<int>> graph_edges_num_array;

};

void Graph::Add(int from, int to, int num) {
    graph_array[from].push_back(to);
    graph_array[to].push_back(from);

    graph_edges_num_array[from].push_back(num);
    graph_edges_num_array[to].push_back(num);
}

std::vector<int> Graph::GetChildPoints(int from) {
    return graph_array[from];
}

int Graph::GetEdgeNum(int from, int to) const {
    return graph_edges_num_array[from][to];
}


void DFS_iter(Graph &graph, int v, std::vector<char> &visited, std::vector<int> &tin,
              std::vector<int> &fup, std::vector<int> &bridges) {

    struct StackParams {
        int v;
        int p;
        int edge_num;
        int timer;
        int iter;
    };

    // Эмулируем рекурсию стеком
    std::stack<StackParams> stack;
    stack.push(StackParams{v, -1, -1, 0, 0});
    int parent_iter = 0;
    int p;
    int edge_num;
    int timer;
    while (!stack.empty()) {
        StackParams sp = stack.top();
        stack.pop();

        v = sp.v;
        p = sp.p;
        edge_num = sp.edge_num;
        timer = sp.timer;

        // Когда возвращаемся назад по стеку, проверяем, не мост ли элемент
        if (sp.iter < parent_iter) {
            fup[p] = std::min(fup[p], fup[v]);
            if (fup[v] > tin[p]) {
                bridges.push_back(edge_num);
            }
            parent_iter = sp.iter;
            continue;
        } else {
            parent_iter = sp.iter;
        }

        if (visited[v]) {
            fup[p] = std::min(fup[p], tin[v]);
            continue;
        }

        visited[v] = true;
        tin[v] = timer;
        fup[v] = timer;
        timer++;

        auto children = graph.GetChildPoints(v);

        if (children.size() == 1) {
            // Если нет исходящих ребер, кроме того, по которому пришли (оно есть всегда), заранее
            // проверяем на мосты, ибо это - последняя итерация с этой вершиной
            fup[p] = std::min(fup[p], fup[v]);
            if (fup[v] > tin[p]) {
                bridges.push_back(edge_num);
            }
        } else {
            // Если есть, отправляем ту же вершину, уже серую, в стек опять
            stack.push(StackParams{v, p, edge_num, timer, sp.iter});
        }

        for (int i = 0; i < children.size(); ++i) {
            int edge_num_child = graph.GetEdgeNum(v, i);
            int to = children[i];
            // Проверяем, не пытаемся ли мы вернуться в себя по тому же ребру (не кратному)
            if (p != to || edge_num_child != edge_num) {
                // Эмулируем вызов рекурсии с новыми параметрами
                stack.push(StackParams{to, v, edge_num_child, timer, sp.iter + 1});
            }
        }
    }
}

std::vector<int> FindBridges(Graph &graph) {
    std::vector<char> visited(graph.size, 0);
    std::vector<int> bridges;
    std::vector<int> tin(graph.size, 0);
    std::vector<int> fout(graph.size, 0);
    int timer = 0;
    for (int start = 0; start < graph.size; start++) {
        if (!visited[start]) {
            DFS_iter(graph, start, visited, tin, fout, bridges);
        }
    }
    return std::move(bridges);
}

int main() {

//    std::ifstream in_stream("bridges.in");
//    std::ofstream out_stream("bridges.out");
//
//    int points_count = 0;
//    int edges_count = 0;
//    in_stream >> points_count >> edges_count;
//    Graph graph(points_count);
//    int from = 0;
//    int to = 0;
//    for (int i = 0; i < edges_count; ++i) {
//        in_stream >> from >> to;
//        // -1, потому что концы ребра начинаются с 1, а не с 0
//        // Передаем номер ребра, начиная с 1
//        graph.Add(from - 1, to - 1, i + 1);
//    }
//    std::vector<int> bridges = FindBridges(graph);
//    std::sort(bridges.begin(), bridges.end());
//    out_stream << bridges.size() << std::endl;
//    for (int bridge : bridges) {
//        out_stream << bridge << std::endl;
//    }


    std::ifstream in_stream("/Users/yaroshenko/CLionProjects/test/made/12/bridges.in");
    std::ofstream out_stream("bridges.out");

    int points_count = 0;
    int edges_count = 0;
    in_stream >> points_count >> edges_count;
    Graph graph(points_count);
    int from = 0;
    int to = 0;
    for (int i = 0; i < edges_count; ++i) {
        in_stream >> from >> to;
        graph.Add(from - 1, to - 1, i + 1);
    }
    std::vector<int> bridges = FindBridges(graph);
    std::sort(bridges.begin(), bridges.end());
    std::cout << bridges.size() << std::endl;
    for (int bridge : bridges) {
        std::cout << bridge << std::endl;
    }

    return 0;
}
