#include "kruskal_approximation.h"
#include "dsu.h"

Graph MakeMST(const Graph &graph) {
    Graph mst(graph.Size());
    DSU dsu(graph.Size());

    std::vector<Edge> edges = graph.Edges();

    std::sort(edges.begin(), edges.end(), [](const Edge &first, const Edge &second) {
        return first.weight < second.weight;
    });

    for (Edge edge : edges) {
        int set1 = dsu.Find(edge.v);
        int set2 = dsu.Find(edge.u);

        if (set1 != set2) {
            mst.Add(edge.v, edge.u, edge.weight);
            dsu.Merge(set1, set2);
        }
    }
    return mst;
}

void DFS(const Graph &mst, int v, std::vector<bool> &visited, std::vector<int> &path) {

    path.push_back(v);
    visited[v] = true;

    for (const Vertex sibling : mst.Siblings(v)) {
        if (visited[sibling.v]) {
            continue;
        }
        DFS(mst, sibling.v, visited, path);
    }
}

float ApproximatedPath(std::vector<Point> &points) {
    Graph graph(points);
    Graph mst = MakeMST(graph);

    std::vector<bool> visited(mst.Size(), false);
    std::vector<int> path;

    DFS(mst, 0, visited, path);

    path.push_back(0);

    float approximated_path = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        approximated_path += graph.EdgeWeight(path[i], path[i + 1]);
    }

    return approximated_path;
}