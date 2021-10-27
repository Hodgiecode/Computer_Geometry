#include "task_data.h"

bool intersect1d(double l1, double r1, double l2, double r2) {
    if (l1 > r1){
        std::swap(l1, r1);
    }

    if (l2 > r2){
        std::swap(l2, r2);
    }

    return std::max(l1, l2) <= std::min(r1, r2) + 1e-9;
}


int vec(const point& a, const point& b, const point& c) {
    double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return abs(s) < 1e-9 ? 0 : s > 0 ? +1 : -1;
}

bool intersect(const segment& a, const segment& b)
{
    return intersect1d(a.p.x, a.q.x, b.p.x, b.q.x) &&
           intersect1d(a.p.y, a.q.y, b.p.y, b.q.y) &&
           vec(a.p, a.q, b.p) * vec(a.p, a.q, b.q) <= 0 &&
           vec(b.p, b.q, a.p) * vec(b.p, b.q, a.q) <= 0;
}

bool operator<(const segment& a, const segment& b)
{
    double x = std::max(std::min(a.p.x, a.q.x), std::min(b.p.x, b.q.x));
    return a.get_y(x) < b.get_y(x) - 1e-9;
}

std::set<segment> s;
std::vector<std::set<segment>::iterator> where;

std::set<segment>::iterator prev(std::set<segment>::iterator it) {
    return it == s.begin() ? s.end() : --it;
}

std::set<segment>::iterator next(std::set<segment>::iterator it) {
    return ++it;
}

int intersection_point(segment &seg1, segment &seg2, point &p){
   double val = 0;

   double x1 = seg1.p.x;
   double y1 = seg1.p.y;

   double x2 = seg1.q.x;
   double y2 = seg1.q.y;

   double x3 = seg2.p.x;
   double y3 = seg2.p.y;

   double x4 = seg2.q.x;
   double y4 = seg2.q.y;
   double n = 0;

   if (y2 - y1 > 1e-9 || y2 - y1 < 1e-9){
       double q = (x2 - x1)/(y1 - y2);
       double sn = (x3 - x4) + (y3 - y4)*q;
       if (!sn){
           return 0;
       }

       double fn = (x3 - x1) + (y3 - y1) * q;
       n = fn/sn;
   }

   else {
       if (!(y3 - y4)){
           return 0;
       }

       n = (y3 - y1)/(y3 - y4);
   }

   p.x = x3 + (x4 - x3) * n;
   p.y = y3 + (y4 - y3) * n;
   return 1;
}



void solve(std::vector<segment> &segment_vector, std::vector<result> &result_vector){
    int n = segment_vector.size();
    std::vector<event> e;

    for (int i = 0; i < n; ++i) {
       e.push_back(event(std::min(segment_vector[i].p.x, segment_vector[i].q.x), +1, i));
       e.push_back(event(std::max(segment_vector[i].p.x, segment_vector[i].q.x), -1, i));
    }

    std::sort(e.begin(), e.end());

    s.clear();
    where.resize(segment_vector.size());
    for (size_t i = 0; i < e.size(); ++i) {
        int id = e[i].id;
        if (e[i].tp == +1) {
            std::set<segment>::iterator nxt = s.lower_bound(segment_vector[id]),
            prv = prev(nxt);

            if (nxt != s.end() && intersect(*nxt, segment_vector[id])){
                result res;
                res.a = nxt->id;
                res.b = id;

                point p;
                if (intersection_point(segment_vector[nxt->id], segment_vector[id],p) == 1){
                    res.p = p;
                    result_vector.push_back(res);
                }
            }

            if (prv != s.end() && intersect(*prv, segment_vector[id])){
                result res;
                res.a = prv->id;
                res.b = id;

                point p;
                if (intersection_point(segment_vector[prv->id], segment_vector[id],p) == 1){
                    res.p = p;
                    result_vector.push_back(res);
                }
            }
                where[id] = s.insert(nxt, segment_vector[id]);
            } else {
                std::set<segment>::iterator nxt = next(where[id]), prv = prev(where[id]);
                if (nxt != s.end() && prv != s.end() && intersect(*nxt, *prv)){
                    result res;
                    res.a = prv->id;
                    res.b = nxt->id;

                    point p;
                    if (intersection_point(segment_vector[prv->id], segment_vector[nxt->id],p) == 1){
                        res.p = p;
                        result_vector.push_back(res);
                    }
                }

                s.erase(where[id]);
            }
    }
    return;
}




bool task_data :: run()
{
    std::vector<segment> segment_vector;
    int cnt = 0;
    for (int i=0;i<vector_of_points.size()-1;i=i+2){
        segment seg;
        seg.p = vector_of_points[i];
        seg.q = vector_of_points[i+1];
        seg.id = cnt;
        cnt = cnt + 1;
        segment_vector.push_back(seg);
    }

    solve(segment_vector, result_vector);

    for (int i=0;i<result_vector.size();i=i+2){
        std::cout << result_vector[i].a << " " << result_vector[i].b << " ("
                  << result_vector[i].p.x << "," << result_vector[i].p.y << ")" << std::endl;
    }

    return true ;
}
