#include "dsu.h"

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