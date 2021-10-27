#include "task_data.h"
#include <iostream>
#include <vector>
#include <algorithm>

/*Триангуляция Делоне */
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
            tr.id_p1 = p[i].id;
            tr.id_p2 = p[i+1].id;
            tr.id_p3 = p[i+2].id;
            ans.push_back(tr);
        }
    }

    return ans;
}

/* Краскал */
double distance(point &p1, point &p2){
    return sqrt((p2.x-p1.x)*(p2.x-p1.x) + (p2.y - p1.y)*(p2.y-p1.y));
}

void sort_edge(std::vector<edge_tree> &A){
    int i,j;
    edge_tree temp;
    int n = A.size();

    for(i=0;i<n;i++){
        for(j=0;j<n-1;j++){
            if(A[j].weight > A[j+1].weight){
                temp = A[j];
                A[j] = A[j+1];
                A[j+1] = temp;
            }
        }
    }
}

int search(std::vector<int> &set_vertex, int i, int j){
    if(set_vertex[i]!=set_vertex[j]){
        return 1;
    }
    else {
       return 0;
    }
}


void union_(std::vector<int> &set_vertex, int a, int b, int n){
    for(int i=0;i<n;i++){
        if(set_vertex[i]==b){
            set_vertex[i] = a;
        }
    }
}

bool task_data :: run()
{
    int n = vector_of_points.size();
    std::vector<int> set;
    std::vector<edge_tree> edge_vector;
    triangle_vector = delaunay(vector_of_points);


    for (size_t i = 0; i<triangle_vector.size(); i++){
        edge_tree e1;
        e1.i = triangle_vector[i].id_p1;
        e1.j = triangle_vector[i].id_p2;
        e1.weight = distance(vector_of_points[triangle_vector[i].id_p1],vector_of_points[triangle_vector[i].id_p2]);

        edge_tree e2;
        e2.i = triangle_vector[i].id_p1;
        e2.j = triangle_vector[i].id_p3;
        e2.weight = distance(vector_of_points[triangle_vector[i].id_p1],vector_of_points[triangle_vector[i].id_p3]);

        edge_tree e3;
        e3.i = triangle_vector[i].id_p2;
        e3.j = triangle_vector[i].id_p3;
        e3.weight = distance(vector_of_points[triangle_vector[i].id_p2],vector_of_points[triangle_vector[i].id_p3]);

        edge_vector.push_back(e1);
        edge_vector.push_back(e2);
        edge_vector.push_back(e3);
    }

    for (int i=0;i<n;i++){
        set.push_back(i);
    }

    //через полный граф
    /*for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (i<=j){
                edge e;
                e.i = i;
                e.j = j;
                e.weight = distance(vector_of_points[i],vector_of_points[j]);
                edge_vector.push_back(e);
            }
        }

        set.push_back(i);
    }*/


    sort_edge(edge_vector);

    double mincost = 0;
    for(int k=0; k<edge_vector.size();k++){
        if (search(set, edge_vector[k].i, edge_vector[k].j)){
                int a = set[edge_vector[k].i];
                int b = set[edge_vector[k].j];
                union_(set,a,b,n);

                mincost = mincost + edge_vector[k].j;
                std::cout << edge_vector[k].i << " " << edge_vector[k].j << " " << edge_vector[k].weight << std::endl;
                result_vector.push_back(edge_vector[k]);
         }
    }

    std::cout << mincost << std::endl;

    return true;
}
