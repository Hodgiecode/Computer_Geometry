#ifndef TASK_DATA_H
#define TASK_DATA_H
#include <math.h>
#include <vector>

struct point {
    double x, y;
    int id;
    point() { }
    point(double _x, double _y, int _id) : x(_x), y(_y),id(_id) { }
    point operator-(const point& p) const {
        return point(x - p.x, y - p.y, -1);
    }
    double cross(const point& p) const {
        return x * p.y - y * p.x;
    }
    double cross(const point& a, const point& b) const {
        return (a - *this).cross(b - *this);
    }
    double dot(const point& p) const {
        return x * p.x + y * p.y;
    }
    double dot(const point& a, const point& b) const {
        return (a - *this).dot(b - *this);
    }
    double sqrLength() const {
        return this->dot(*this);
    }
    bool operator==(const point& p) const {
        return (x == p.x) && (y == p.y);
    }

    double length() { return sqrt(x * x + y * y); }

};


struct edge {
    point origin;
    edge* rot = nullptr;
    edge* onext = nullptr;
    bool used = false;
    edge* rev() const {
        return rot->rot;
    }
    edge* lnext() const {
        return rot->rev()->onext->rot;
    }
    edge* oprev() const {
        return rot->onext->rot;
    }
    point dest() const {
        return rev()->origin;
    }
};


struct triangle {
    point p1;
    point p2;
    point p3;
    point center;
    double radius;
};


struct task_data {
    std::vector<point> vector_of_points;
    std::vector<triangle> triangle_vector;

    task_data(){

    }

    bool run();
};

#endif // TASK_DATA_H
