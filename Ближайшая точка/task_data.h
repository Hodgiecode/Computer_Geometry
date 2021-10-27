#ifndef TASK_DATA_H
#define TASK_DATA_H
#include <math.h>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <algorithm>

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



struct task_data {
    std::vector<point> vector_of_points;
    std::string time_algo_one = "";
    std::string dist_algo_one = "";
    std::vector<int> nearest_point_vector_index;

    std::string time_algo_two = "";
    std::string dist_algo_two = "";

    task_data(){

    }

    bool run_task_one();
    bool run_task_two();
};

#endif // TASK_DATA_H
