#ifndef TASK_DATA_H
#define TASK_DATA_H
#include <math.h>
#include <vector>
#include <iostream>

struct point {
    double x;
    double y;
    int id;

    point() {} //конструктор без параметров
    point(double x_, double y_, int id_): x(x_), y(y_), id(id_) {} //конструктор с параметрами

    double length() { return sqrt(x * x + y * y); }

    point operator+(const point& src) const { return point(x + src.x, y + src.y,-1); }
    point operator-(const point& src) const { return point(x - src.x, y - src.y,-1); }
    double operator*(const point& src) const { return (x  * src.x +  y * src.y); }
    point operator*(double d) const { return point(d * x, d * y,-1); }
};

inline point operator*(const double& t, const point& v){
  return point(t * v.x, t * v.y, -1);
}


struct task_data {
    std::vector<point> vector_of_points;
    std::vector<point> curve;

    task_data(){

    }

    bool lagrange_spline();
    bool cubic_spline();
    bool hermite_spline();
};

#endif // TASK_DATA_H
