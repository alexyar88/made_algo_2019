#include "min_path_find.h"


float FindMinPath(std::vector<Point> points) {
    float min_path = 99999999;
    do {
        float current_distance = Distance(points[0], points[points.size() - 1]);
        for (int i = 0; i < points.size() - 1; ++i) {
            current_distance += Distance(points[i], points[i + 1]);
        }

        min_path = std::min(current_distance, min_path);
    } while (std::next_permutation(points.begin(), points.end(), [](const auto &p1, const auto &p2) {
        return p1.x + p1.y < p2.x + p2.y;
    }));
    return min_path;
}