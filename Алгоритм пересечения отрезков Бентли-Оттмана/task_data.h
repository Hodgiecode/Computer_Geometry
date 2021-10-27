#ifndef TASK_DATA_H
#define TASK_DATA_H
#include <math.h>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>


struct point {
    double x;
    double y;
    int id;

    point() {} //конструктор без параметров
    point(double x_, double y_, int id_): x(x_), y(y_), id(id_) {} //конструктор с параметрами

    double length() { return sqrt(x * x + y * y); }

    point operator+(const point& op) const
    {
        return point(x + op.x, y + op.y, -1);
    }

    point operator-(const point& op) const
    {
        return  point(x - op.x, y - op.y, -1);
    }
};

inline point operator*(const double& t, const point& v)
{
  return point(t * v.x, t * v.y, -1);
}


struct event {
    double x;
    int tp, id;

    event() {}
    event(double x, int tp, int id) : x(x), tp(tp), id(id) {}

    bool operator<(const event& e) const {
        if (abs(x - e.x) > 1e-9){
            return x < e.x;
        }
        return tp > e.tp;
    }
};

struct segment {
    point p;
    point q;
    int id;

    double get_y(double x) const {
          if (abs(p.x - q.x) < 1e-9){
                return p.y;
          }

          return p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
     }
};

struct result{
    int a;
    int b;
    point p;
};

struct task_data {
    std::vector<point> vector_of_points;
    std::vector<result> result_vector;

    task_data(){

    }

    bool run();
};

#endif // TASK_DATA_H
