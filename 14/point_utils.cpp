#include <cmath>
#include <random>

const int PI = 3.14159265;

#include "point_utils.h"

std::vector<Point> GeneratePoints(int count) {
    std::vector<Point> points;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> uniform_dist(0, 1);

    for (int i = 0; i < count; ++i) {
        float r = uniform_dist(gen);
        float phi = uniform_dist(gen);
        float x = std::cos(2 * PI * phi) * std::sqrt(-2 * std::log(r));
        float y = std::sin(2 * PI * phi) * std::sqrt(-2 * std::log(r));
        points.push_back(Point{x, y});
    }
    return points;
}

float Distance(const Point &p1, const Point &p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}
