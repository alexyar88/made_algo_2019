#include "graph.h"
#include <assert.h>

#include "point_utils.h"

Graph::Graph(int size) : vertexes(size) {}

Graph::Graph(std::vector<Point> points) : vertexes(points.size()) {
    for(int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points.size(); ++j) {
            if (i == j) {
                continue;
            }
            Add(i, j, Distance(points[i], points[j]));
        }
    }
}

int Graph::Add(int v, int u, float weight) {
    vertexes[v].push_back(Vertex{u, weight});
    vertexes[u].push_back(Vertex{v, weight});
    edges.push_back(Edge{v, u, weight});
}

float Graph::EdgeWeight(int v, int u) const {
    for (const auto sibling: vertexes[v]) {
        if (sibling.v == u) {
            return sibling.weight;
        }
    }
    assert(false);
}

std::vector<Vertex> Graph::Siblings(int v) const {
    return vertexes[v];
}

int Graph::Size() const {
    return vertexes.size();
}

std::vector<Edge> Graph::Edges() const {
    return edges;
}


