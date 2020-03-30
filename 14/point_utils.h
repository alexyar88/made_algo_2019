#ifndef INC_15_POINT_UTILS_H
#define INC_15_POINT_UTILS_H

#include <vector>

struct Point {
    float x;
    float y;

    bool operator==(Point &other) {
        return x == other.x && y == other.y;
    }

    bool operator>(Point &other) {
        return x + y < other.x + other.y;
    }
};

std::vector<Point> GeneratePoints(int count);

float Distance(const Point &p1, const Point &p2);

#endif //INC_15_POINT_UTILS_H
