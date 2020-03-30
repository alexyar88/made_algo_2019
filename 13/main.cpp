/*
14. «MST».
Дан неориентированный связный граф.
Требуется найти вес минимального остовного дерева в этом графе.
Вариант 2. С помощью алгоритма Крускала.
 * */

#include <iostream>
#include <algorithm>
#include <vector>
#include "assert.h"

class DSU {
public:
    explicit DSU(size_t size);

    int Find(int element);

    void Merge(int left, int right);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(size_t size) : parent(size, -1), rank(size, 0) {}

int DSU::Find(int element) {
    if (parent[element] == -1) {
        return element;
    } else {
        return parent[element] = Find(parent[element]);
    }
}

void DSU::Merge(int left, int right) {
    const int deputat_left = Find(left);
    const int deputat_right = Find(right);
    assert(deputat_left != deputat_right);

    if (rank[deputat_left] == rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
        ++rank[deputat_left];
    } else if (rank[deputat_left] > rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
    } else {
        parent[deputat_left] = deputat_right;
    }
}

struct Edge {
    int v1;
    int v2;
    int weight;
};

int MSTWeightKruskal(std::vector<Edge> &edges, int vertex_count) {

    // Сертируем ребра, чтобы начать перебор с минимального
    std::sort(edges.begin(), edges.end(), [](const Edge &first, const Edge &second) {
        return first.weight < second.weight;
    });

    int weight = 0;

    DSU dsu(vertex_count);

    for (Edge edge : edges) {
        int set1 = dsu.Find(edge.v1);
        int set2 = dsu.Find(edge.v2);

        // Если вершины в разных множествах, соединяем их и добавляем вес ребра
        if (set1 != set2) {
            weight += edge.weight;
            dsu.Merge(set1, set2);
        }
    }
    return weight;
}


int main() {
    int vertex_count = 0;
    int edges_count = 0;

    std::cin >> vertex_count >> edges_count;
    std::vector<Edge> edges;

    int v1 = 0;
    int v2 = 0;
    int weight = 0;

    for (int i = 0; i < edges_count; ++i) {
        std::cin >> v1 >> v2 >> weight;
        edges.push_back(Edge{v1 - 1, v2 - 1, weight});
    }

    std::cout << MSTWeightKruskal(edges, vertex_count);

    return 0;
}
