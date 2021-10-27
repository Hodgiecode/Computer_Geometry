#include "task_data.h"

enum pointType {vertex, cross};

class Node{
    public:
    Node(point* pt, pointType ptT, bool isTraverse = false, Node* next = NULL){
        this->pt = pt;
        this->ptType = ptT;
        this->isTraverse = isTraverse;
        this->next = next;
    }

    point* pt;
    pointType ptType;
    bool isTraverse;
    Node* next;
};


std::vector<std::vector<point>> polygon_intersection(std::vector<point> poly1,std::vector<point> poly2);

std::vector<std::vector<std::vector<std::vector<point>>>> polygon_split(std::vector<point> bigPolygon, double side);
int clock_wise(const std::vector<point>& p);
std::vector<std::vector<point>> PolygonIntersection(std::vector<point> poly1, std::vector<point> poly2);
bool is_point_on_polygon_edge(const point& pt, const std::vector<point>& poly);

bool is_point_on_segment(const point& p, const point& p1, const point& p2);
bool detect_intersect(const point& p1, const point& p2, const point& p3, const point& p4, double& line_x, double& line_y);
bool point_cmp(point* poly1, point* poly2);
