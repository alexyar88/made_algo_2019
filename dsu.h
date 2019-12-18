//
// Created by Алексей Ярошенко on 17.12.2019.
//

#ifndef INC_15_DSU_H
#define INC_15_DSU_H

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
#endif //INC_15_DSU_H
