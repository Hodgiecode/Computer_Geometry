#ifndef TASK_DATA_H
#define TASK_DATA_H
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>

struct point {
    double x, y;
    double alpha,beta;
    int id;
    int position; //1 inner -1 outer 0 on

    point() { }
    point(double _x, double _y, int _id) : x(_x), y(_y), id(_id) { }
    point operator-(const point& p) const {
        return point(x - p.x, y - p.y, -1);
    }

    bool operator==(const point& p) const {
           return (x == p.x) && (y == p.y);
    }

    double length() { return sqrt(x * x + y * y); }
};

struct edge {
    point s,e;
    edge():s(0,0,0),e(0,0,1){}
    edge(point &start, point &end): s(start), e(end){}

    void setStart(point &s){ this->s = s; }
    void setEnd(point &e){ this->e = e; }
};


struct task_data {
    std::vector<std::vector<point>> polygons;
    std::vector<edge> result;

    task_data(){

    }

    bool difference1();
    bool difference2();
    bool union_();
    bool intersection();
};

#endif // TASK_DATA_H

