#include "task_data.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

double calc_radius(point p1, point p2, point p3){
    double a = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
    double b = sqrt((p3.x - p2.x)*(p3.x - p2.x) + (p3.y - p2.y)*(p3.y - p2.y));
    double c = sqrt((p3.x - p1.x)*(p3.x - p1.x) + (p3.y - p1.y)*(p3.y - p1.y));
    double p = (a+b+c)/2;
    double R = (a*b*c)/(4*sqrt(p*(p-a)*(p-b)*(p-c)));
    return R;
}

point calc_center(point p1, point p2, point p3){
    double a2 = ((p2.x - p3.x)*(p2.x - p3.x) + (p2.y - p3.y)*(p2.y - p3.y));
    double b2 = ((p1.x - p3.x)*(p1.x - p3.x) + (p1.y - p3.y)*(p1.y - p3.y));
    double c2 = ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));

    double a = sqrt(a2);
    double b = sqrt(b2);
    double c = sqrt(c2);

    double alpha = acos((b2 + c2 - a2)/(2*b*c));
    double beta = acos((a2 + c2 - b2)/(2*a*c));
    double gamma = acos((a2 + b2 - c2)/(2*a*b));

    double x =(p1.x*sin(2*alpha)+p2.x*sin(2*beta)+p3.x*sin(2*gamma))/(sin(2*alpha)+sin(2*beta)+sin(2*gamma));
    double y =(p1.y*sin(2*alpha)+p2.y*sin(2*beta)+p3.y*sin(2*gamma))/(sin(2*alpha)+sin(2*beta)+sin(2*gamma));

    point p;
    p.x = x;
    p.y = y;

    return p;
}


int sgn(const double& a) {
    if (a >= 0){
        return 1;
    }

    if (a == 0){
        return 0;
    }

    if (a < 0){
        return -1;
    }
}


edge* make_edge(point from, point to) {
    edge* e1 = new edge;
    edge* e2 = new edge;
    edge* e3 = new edge;
    edge* e4 = new edge;
    e1->origin = from;
    e2->origin = to;
    e3->origin = e4->origin = point(1e18,1e18,-1);
    e1->rot = e3;
    e2->rot = e4;
    e3->rot = e2;
    e4->rot = e1;
    e1->onext = e1;
    e2->onext = e2;
    e3->onext = e4;
    e4->onext = e3;
    return e1;
}

void splice(edge* a, edge* b) {
    std::swap(a->onext->rot->onext, b->onext->rot->onext);
    std::swap(a->onext, b->onext);
}

void delete_edge(edge* e) {
    splice(e, e->oprev());
    splice(e->rev(), e->rev()->oprev());
    delete e->rev()->rot;
    delete e->rev();
    delete e->rot;
    delete e;
}

edge* connect(edge* a, edge* b) {
    edge* e = make_edge(a->dest(), b->origin);
    splice(e, a->lnext());
    splice(e->rev(), b);
    return e;
}

bool left_of(point p, edge* e) {
    return (p.cross(e->origin, e->dest())> 0);
}

bool right_of(point p, edge* e) {
    return (p.cross(e->origin, e->dest()) < 0);
}


double det3(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3) {
    return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) +
           a3 * (b1 * c2 - c1 * b2);
}

bool in_circle(point a, point b, point c, point d) {
    double det = -det3(b.x, b.y, b.sqrLength(), c.x, c.y,
                                   c.sqrLength(), d.x, d.y, d.sqrLength());
    det += det3(a.x, a.y, a.sqrLength(), c.x, c.y, c.sqrLength(), d.x,
                          d.y, d.sqrLength());
    det -= det3(a.x, a.y, a.sqrLength(), b.x, b.y, b.sqrLength(), d.x,
                          d.y, d.sqrLength());
    det += det3(a.x, a.y, a.sqrLength(), b.x, b.y, b.sqrLength(), c.x,
                          c.y, c.sqrLength());
    return det > 0;
}

std::vector<edge*> build_triangle(int l, int r, std::vector<point>& p) {
    std::vector<edge *> tr_vec;

    if (r - l + 1 == 2) {
        edge* res = make_edge(p[l], p[r]);
        tr_vec.push_back(res);
        tr_vec.push_back(res->rev());
        return tr_vec;
    }
    if (r - l + 1 == 3) {
        edge *a = make_edge(p[l], p[l + 1]);
        edge *b = make_edge(p[l + 1], p[r]);
        splice(a->rev(), b);
        int sg = sgn(p[l].cross(p[l + 1], p[r]));

        if (sg == 0){
            tr_vec.push_back(a);
            tr_vec.push_back(b->rev());
            return tr_vec;
        }

        edge* c = connect(b, a);
        if (sg == 1){
            tr_vec.push_back(a);
            tr_vec.push_back(b->rev());
            return tr_vec;
        }
        else {
            tr_vec.push_back(c->rev());
            tr_vec.push_back(c);
            return tr_vec;
        }
    }

    int mid = (l + r) / 2;
    edge *ldo, *ldi, *rdo, *rdi;

    std::vector<edge*> tmp;
    tmp = build_triangle(l, mid, p);
    ldo = tmp[0];
    ldi = tmp[1];

    tmp = build_triangle(mid + 1, r, p);
    rdi = tmp[0];
    rdo = tmp[1];

    while (true) {
        if (left_of(rdi->origin, ldi)) {
            ldi = ldi->lnext();
            continue;
        }
        if (right_of(ldi->origin, rdi)) {
            rdi = rdi->rev()->onext;
            continue;
        }
        break;
    }

    edge* basel = connect(rdi->rev(), ldi);
    auto valid = [&basel](edge* e) { return right_of(e->dest(), basel); };

    if (ldi->origin == ldo->origin){
        ldo = basel->rev();
    }

    if (rdi->origin == rdo->origin){
        rdo = basel;
    }

    while (true) {
        edge* lcand = basel->rev()->onext;
        if (valid(lcand)) {
            while (in_circle(basel->dest(), basel->origin, lcand->dest(),
                             lcand->onext->dest())) {
                edge* t = lcand->onext;
                delete_edge(lcand);
                lcand = t;
            }
        }
        edge* rcand = basel->oprev();
        if (valid(rcand)) {
            while (in_circle(basel->dest(), basel->origin, rcand->dest(),
                             rcand->oprev()->dest())) {
                edge* t = rcand->oprev();
                delete_edge(rcand);
                rcand = t;
            }
        }

        if (!valid(lcand) && !valid(rcand)){
            break;
        }

        if (!valid(lcand) || (valid(rcand) && in_circle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest()))){
            basel = connect(rcand, basel->rev());
        }

        else {
            basel = connect(basel->rev(), lcand->rev());
        }
    }

    tr_vec.push_back(ldo);
    tr_vec.push_back(rdo);
    return tr_vec;
}

std::vector<triangle> delaunay(std::vector<point> p) {
    std::sort(p.begin(), p.end(), [](const point& a, const point& b) {
        return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
    });

    std::vector<edge*> res = build_triangle(0, (int)p.size() - 1, p);
    edge* e = res[0];

    std::vector<edge*> edges = {e};
    while ((e->onext->dest().cross(e->dest(), e->origin) < 0)){
        e = e->onext;
    }

    auto add = [&p, &e, &edges]() {
        edge* curr = e;
        do {
            curr->used = true;
            p.push_back(curr->origin);
            edges.push_back(curr->rev());
            curr = curr->lnext();
        } while (curr != e);
    };

    add();
    p.clear();

    int kek = 0;
    while (kek < (int)edges.size()) {
        if (!(e = edges[kek++])->used){
            add();
        }
    }

    std::vector<triangle> ans;

    for (int i = 0; i < p.size(); i += 3) {
        if (i<p.size()-2){
            triangle tr;
            tr.p1 = p[i];
            tr.p2 = p[i+1];
            tr.p3 = p[i+2];

            tr.radius = calc_radius(p[i],p[i+1],p[i+2]);
            tr.center = calc_center(p[i],p[i+1],p[i+2]);
            ans.push_back(tr);
        }
    }

    return ans;
}

bool task_data::run(){
    triangle_vector = delaunay(vector_of_points);
    //std::cout << triangle_vector.size() <<std::endl;
    return true;
}
