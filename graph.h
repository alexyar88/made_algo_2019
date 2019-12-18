#ifndef INC_15_GRAPH_H
#define INC_15_GRAPH_H

#include <vector>
#include "point_utils.h"

struct Vertex {
    int v;
    float weight;
};

struct Edge {
    int v;
    int u;
    float weight;
};

class Graph {
public:
    explicit Graph(int size);
    explicit Graph(std::vector<Point> points);

    int Add(int v, int u, float weight);

    float EdgeWeight(int v, int u) const;

    std::vector<Vertex> Siblings(int v) const;

    std::vector<Edge> Edges() const;

    int Size() const;

private:
    std::vector<std::vector<Vertex>> vertexes;
    std::vector<Edge> edges;

};

#endif //INC_15_GRAPH_H
