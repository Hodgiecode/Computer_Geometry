#define _USE_MATH_DEFINES
#include "task_data.h"

bool PointCmp(const point &a,const point &b,const point &center){
      if (a.x >= 0 && b.x < 0)
         return true;
     if (a.x == 0 && b.x == 0)
         return a.y > b.y;

     int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);

     if (det < 0)
         return true;

     if (det > 0)
         return false;

     int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
     int d2 = (b.x - center.x) * (b.x - center.y) + (b.y - center.y) * (b.y - center.y);
     return d1 > d2;
}
void ClockwiseSortPoints(std::vector<point> &vPoints){
     point center;
     double x = 0,y = 0;
     for (int i = 0;i < vPoints.size();i++){
         x += vPoints[i].x;
         y += vPoints[i].y;
     }

     center.x = (int)x/vPoints.size();
     center.y = (int)y/vPoints.size();

     for(int i = 0;i < vPoints.size() - 1;i++)
     {
         for (int j = 0;j < vPoints.size() - i - 1;j++)
         {
             if (PointCmp(vPoints[j],vPoints[j+1],center))
             {
                 point tmp = vPoints[j];
                 vPoints[j] = vPoints[j + 1];
                 vPoints[j + 1] = tmp;
             }
         }
     }
 }

/*--------------------------------*/

int get2LinesPosition(point &p1, point &p2, point &p3, point &p4, point &pi)
{
    //not intersected = -1
    //collinear = 0
    //parallel = 1
    //intersected = 2

    double eps = 10e-6;

    double ux = p2.x - p1.x;
    double uy = p2.y - p1.y;
    double vx = p4.x - p3.x;
    double vy = p4.y - p3.y;
    double wx = p1.x - p3.x;
    double wy = p1.y - p3.y;

    double k1 = vx * wy - vy * wx;
    double k2 = ux * wy - uy * wx;
    double k3 = ux * vy - uy * vx;

    if ((fabs(k1) < eps) && (fabs(k2) < eps) && (fabs(k3) < eps)){
        return 0;
    }

    if ((fabs(k1) < eps) && (fabs(k2) < eps)){
        return 1;
    }


    double alpha = k1 / k3;
    double beta = k2 / k3;

    if((alpha >=0) && (alpha <= 1) && (beta >=0) && (beta <= 1)){

        double xpi = p1.x + alpha * ux;
        double ypi = p1.y + alpha * uy;

        pi.x = xpi;
        pi.y = ypi;
        pi.alpha = alpha;
        pi.beta = beta;

        return 2;
    }

    //Intersection does not exist
    return -1;
}

void selectEdges(std::vector<point> &pol, int position, std::vector<edge> &edges){
    for(int i = 0; i < pol.size(); i++){
        if (pol[i].position == position){
            edge e (pol[i], pol[(i+1)%pol.size()]);
            edges.push_back(e);
        }
    }
}

void processIntersection(point &pi, double &t, std::vector<point> &polygon, int &i){
    double eps = 1.0e-6;
    if ((t >= eps) && (t <= (1-eps))){
        i = i + 1;
        polygon.insert(polygon.begin() + i, pi);
    }
}

void computePolygonIntersection(std::vector<point> &pa, std::vector<point> &pb){
    for (int i = 0; i < pa.size(); i++){

        std::map<double, point> intersections;

        for (int j = 0; j < pb.size(); j++){
            point pi;

            if (get2LinesPosition(pa[i], pa[(i+1)%pa.size()], pb[j], pb[(j+1)%pb.size()], pi) == 2){

                double alfa = pi.alpha;
                double beta = pi.beta;

                intersections[alfa] = pi;

                processIntersection(pi, beta, pb, j);
            }
        }

        if (intersections.size() > 0){
            for (std::pair<double, point> item:intersections){
                point pi = item.second;
                double alfa = pi.alpha;
                processIntersection(pi, alfa, pa, i);
            }
        }
    }
}


int getPointLinePosition(point &q, point &p1, point &p2){
    //left 1; right -1; colinear 0

    double ux = p2.x - p1.x;
    double uy = p2.y - p1.y;
    double vx = q.x - p1.x;
    double vy = q.y - p1.y;

    double t = ux * vy - uy * vx;

    if (t > 0){
        return 1;
    }

    if (t < 0){
        return -1;
    }

    return 0;
}

double getDistance(point &p1, point &p2){
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dist = sqrt(dx * dx + dy * dy);

    return dist;
}


double getAngle2Vectors(point &p1, point &p2,point &p3, point &p4){
    double ux = p2.x - p1.x;
    double uy = p2.y - p1.y;

    double vx = p4.x - p3.x;
    double vy = p4.y - p3.y;

    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    double scalar = ux * vx + uy * vy;

    double angle = fabs(acos(scalar/(nu*nv)));
    return angle;
}


int positionPointPolygonWinding(point &q, std::vector<point> &pol){
    //inner 1; outer -1; 0 on
    double wn = 0.0;
    double eps = 1.0e-6;
    int n = pol.size();

    for (int i = 0; i < n; i++){
        double omega = getAngle2Vectors(pol[i], q, pol[(i+1)%n], q);

        if (fabs(fabs(omega) - M_PI) <= eps){
            return 0;
        }

        int orient = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        if (orient == 1){
            wn += omega;
        }

        else {
            wn -= omega;
        }
    }

    if (fabs(fabs(wn) - 2 * M_PI) <= eps){
        return 1;
    }

    return -1;
}

void setPositions(std::vector<point> &pa, std::vector<point> &pb){
    int n = pa.size();
    for(int i = 0; i < n; i++){
        double mx = (pa[i].x + pa[(i + 1)%n].x) / 2;
        double my = (pa[i].y + pa[(i + 1)%n].y) / 2;

        point m(mx, my, 0);
        int position = positionPointPolygonWinding(m, pb);

        pa[i].position = position;
    }
}

void setPositionsAB(std::vector<point> &pa, std::vector<point> &pb){
    setPositions(pa, pb);
    setPositions(pb, pa);
}


std::vector<edge> booleanOperations(std::vector<point> &polygonA, std::vector<point> &polygonB, std::string operation)
{
    std::vector<edge> result;

    computePolygonIntersection(polygonA, polygonB);

    setPositionsAB(polygonA, polygonB);

    //Union
    if (operation == "union"){
        selectEdges(polygonA, -1, result);
        selectEdges(polygonB, -1, result);
    }

    //Intersect
    else if(operation == "intersection"){
        selectEdges(polygonA, 1, result);
        selectEdges(polygonB, 1, result);
    }

    //Difference A - B
    else if(operation == "differenceAB"){
        selectEdges(polygonA, -1, result);
        selectEdges(polygonB, 1, result);
        selectEdges(polygonA, 0, result);
    }

    //Difference B - A
    else if (operation == "differenceBA")
    {
        selectEdges(polygonA, 1, result);
        selectEdges(polygonB, -1, result);
        selectEdges(polygonB, 0, result);
    }

    return result;
}

bool task_data::intersection(){
    std::vector<point> polygonA;
    std::vector<point> polygonB;

    polygonA = polygons[0];
    polygonB = polygons[1];

    result = booleanOperations(polygonA,polygonB,"intersection");
    return true;
}

bool task_data::union_(){
    std::vector<point> polygonA;
    std::vector<point> polygonB;

    polygonA = polygons[0];
    polygonB = polygons[1];

    result = booleanOperations(polygonA,polygonB,"union");
    return true;
}

bool task_data::difference1(){
    std::vector<point> polygonA;
    std::vector<point> polygonB;

    polygonA = polygons[0];
    polygonB = polygons[1];

    result = booleanOperations(polygonA,polygonB,"differenceAB");
    return true;
}


bool task_data::difference2(){
    std::vector<point> polygonA;
    std::vector<point> polygonB;

    polygonA = polygons[0];
    polygonB = polygons[1];

    result = booleanOperations(polygonA,polygonB,"differenceBA");
    return true;
}
