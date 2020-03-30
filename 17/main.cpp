/*
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 2. С помощью алгоритма Джарвиса.
*/

#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    double x;
    double y;
};

const double ZERO = 0.0000000001;

// Векторное произведение
double Dot(const Point &p1, const Point &p2, const Point &zero) {
    return (p1.x - zero.x) * (p2.y - zero.y) - (p1.y - zero.y) * (p2.x - zero.x);
}

double Distance(const Point &p1, const Point &p2) {
    return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
}

double JarvisPerimeter(std::vector<Point> &points) {
    int start = 0;
    // Берем нижнюю левую точку, потому что точки могут быть на одной прямой и
    // взять просто нижнюю - недостаточно, она может не входить в выпуклую оболочку.
    for (int i = 0; i < points.size(); ++i) {
        if (points[start].y > points[i].y) {
            start = i;
        } else if (points[start].y == points[i].y && points[start].x > points[i].x) {
            start = i;
        }
    }

    int current = start;
    int next = 0;

    double perimeter = 0;

    while (true) {
        for (int i = 0; i < points.size(); ++i) {
            double dot_result = Dot(points[i], points[next], points[current]);
            if (dot_result > ZERO) { // Если точки не на одной прямой, берем ту, что правее
                next = i;
            } else if (dot_result > -ZERO) { // Если точки на одной прямой, берем дальнюю
                if (Distance(points[i], points[current]) > Distance(points[next], points[current])) {
                    next = i;
                }
            }
        }

        perimeter += Distance(points[current], points[next]);
        current = next;

        if (start == next) {
            break;
        }
    }

    return perimeter;
}

int main() {
    int points_count = 0;
    double x;
    double y;

    std::vector<Point> points;
    std::cin >> points_count;
    for (int i = 0; i < points_count; ++i) {
        std::cin >> x >> y;
        points.push_back(Point{x, y});
    }
    std::cout.precision(9);
    std::cout << JarvisPerimeter(points) << std::endl;
    return 0;
}
