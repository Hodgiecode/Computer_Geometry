#include "intersection.h"
#include <algorithm>

std::vector<std::vector<std::vector<std::vector<point>>>> polygon_split(std::vector<point> bigPolygon, double side){
    std::vector<std::vector<std::vector<std::vector<point>>>> polySplit(4);

    double left = (std::numeric_limits<double>::max)(),
            down = (std::numeric_limits<double>::max)(),
            right = -(std::numeric_limits<double>::max)(),
            up = -(std::numeric_limits<double>::max)();

    for (int i = 0; i < bigPolygon.size(); ++i){
        if (bigPolygon[i].x < left){
            left = bigPolygon[i].x;
        }

        if (bigPolygon[i].x > right){
            right = bigPolygon[i].x;
        }

        if (bigPolygon[i].y < down){
            down = bigPolygon[i].y;
        }

        if (bigPolygon[i].y > up){
            up = bigPolygon[i].y;
        }
    }

    left--;
    right++;
    down--;
    up++;

    int m = ceil((up-down)/side);
    int n = ceil((right-left)/side);

    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j){
            std::vector<point> square;
            square.push_back(point(left+j*side, up-i*side,-1));
            square.push_back(point(left+(j+1)*side, up-i*side,-1));
            square.push_back(point(left+(j+1)*side, up-(i+1)*side,-1));
            square.push_back(point(left+j*side, up-(i+1)*side,-1));

            std::vector<std::vector<point>> intersect = polygon_intersection(square, bigPolygon);
            if (!intersect.empty()){
                int race = i%2*2+j%2;
                polySplit[race].push_back(intersect);
            }
        }
    }

    return polySplit;
}

bool between(double a, double x0, double x1){
    double temp1= a-x0;
    double temp2= a-x1;

    if ((temp1<1e-6 && temp2>1e-6) || (temp2<-1e-6 && temp1>-1e-6)){
        return true;
    }

    else {
        return false;
    }
}

bool detect_intersect(const point& p1, const point& p2, const point& p3, const point& p4, double& line_x, double& line_y){
    if ( (fabs(p1.x-p2.x)<1e-6) && (fabs(p3.x-p4.x)<1e-6)){
        return false;
    }

    else if ( (fabs(p1.x-p2.x)<1e-6)){
        if (between(p1.x,p3.x,p4.x)){
            double k = (p4.y-p3.y)/(p4.x-p3.x);
            line_x = p1.x;
            line_y = k*(line_x-p3.x)+p3.y;

            if (between(line_y,p1.y,p2.y))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        else{
            return false;
        }
    }

    else if ( (fabs(p3.x-p4.x)<1e-6) ){
        if (between(p3.x,p1.x,p2.x)){
            double k = (p2.y-p1.y)/(p2.x-p1.x);
            line_x = p3.x;
            line_y = k*(line_x-p2.x)+p2.y;

            if (between(line_y,p3.y,p4.y))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else {
            return false;
        }
    }
    else{
        double k1 = (p2.y-p1.y)/(p2.x-p1.x);
        double k2 = (p4.y-p3.y)/(p4.x-p3.x);

        if (fabs(k1-k2)<1e-6)
        {
            return false;
        }
        else
        {
            line_x = ((p3.y - p1.y) - (k2*p3.x - k1*p1.x)) / (k1-k2);
            line_y = k1*(line_x-p1.x)+p1.y;
        }

        if (between(line_x,p1.x,p2.x)&&between(line_x,p3.x,p4.x))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool is_point_in_polygon(const point& pt, std::vector<point>& poly){
    if (is_point_on_polygon_edge(pt, poly)){
        return true;
    }

    int i,j;
    bool c = false;
    for (i = 0,j = poly.size() - 1;i < poly.size();j = i++){
        if ((((poly[i].y <= pt.y) && (pt.y < poly[j].y)) ||
             ((poly[j].y <= pt.y) && (pt.y < poly[i].y)))
            && (pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y)/(poly[j].y - poly[i].y) + poly[i].x))
        {
            c = !c;
        }
    }
    return c;
}


bool point_cmp(point* poly1, point* poly2){
    if (poly1->x > poly2->x){
        return true;
    }

    if (poly1->x == poly2->x){
        return poly1->y > poly2->y;
    }

    return false;
}


Node* crossWithPoly(const std::vector<std::vector<point*>>& crossPoints,
                    std::vector<point>& poly1, int poly2Size, bool transpose){

    Node* crossWithPoly1 = NULL;
    Node* pNode = crossWithPoly1;

    for (int i = 0; i < poly1.size(); ++i){
        std::vector<point*> ptInLine;
        for (int j = 0; j < poly2Size; ++j){
            point* curCross= transpose ? crossPoints[j][i] : crossPoints[i][j];
            if (curCross != NULL){
                ptInLine.push_back(curCross);
            }
        }

        std::sort(ptInLine.begin(), ptInLine.end(), point_cmp);
        if (!point_cmp(&poly1[i], &poly1[(i+1)%poly1.size()])){
            reverse(ptInLine.begin(), ptInLine.end	());
        }

        if (crossWithPoly1 == NULL){

            crossWithPoly1 = new Node(&poly1[i], vertex, false, NULL);
            pNode = crossWithPoly1;
        }

        else {
            pNode->next = new Node(&poly1[i], vertex, false, NULL);
            pNode = pNode->next;
        }

        for (int j = 0; j < ptInLine.size(); ++j){
            pNode->next = new Node(ptInLine[j], cross, false, NULL);
            pNode = pNode->next;
        }
    }

    pNode->next = crossWithPoly1;
    return crossWithPoly1;
}


std::vector<std::vector<point>> polygon_intersection(std::vector<point>poly1, std::vector<point> poly2){
    std::vector<std::vector<point>> polyInter;

    if (clock_wise(poly1) < 0){
        reverse(poly1.begin(), poly1.end());
    }

    if (clock_wise(poly2) < 0){
        reverse(poly2.begin(), poly2.end());
    }

    std::vector<std::vector<point*>> crossPoints(poly1.size(), std::vector<point*>(poly2.size(), NULL));

    int crossNum = 0;
    for (int i = 0; i < poly1.size(); ++i){
        for (int j = 0; j < poly2.size(); ++j){
            double x, y;
            if (detect_intersect(poly1[i], poly1[(i+1)%poly1.size()], poly2[j], poly2[(j+1)%poly2.size()], x, y)){
                point p;
                p.x = x;
                p.y = y;
                p.id = -1;

                crossPoints[i][j] = &p;
                crossNum++;
            }
        }
    }

    if (crossNum == 0){
        int inPolyNum = 0;
        for (int i = 0; i < poly1.size(); ++i){
            if (is_point_in_polygon(poly1[i], poly2)){
                inPolyNum++;
            }
        }

        if (inPolyNum > 0){
            if (inPolyNum != poly1.size()){
                return polyInter;
            }

            polyInter.push_back(poly1);
            return polyInter;
        }

        inPolyNum = 0;
        for (int i = 0; i < poly2.size(); ++i){
            if (is_point_in_polygon(poly2[i], poly1)){
                inPolyNum++;
            }
        }

        if (inPolyNum > 0){
            if (inPolyNum != poly2.size()){
                return polyInter;
            }
            polyInter.push_back(poly2);
            return polyInter;
        }
        return polyInter;
    }

    Node* crossWithPoly1 = crossWithPoly(crossPoints, poly1, poly2.size(), false);
    Node* crossWithPoly2 = crossWithPoly(crossPoints, poly2, poly1.size(), true);

    int traver1 = 0;

    while (traver1 < poly1.size()+crossNum){
        Node* poly1First = crossWithPoly1;
        while(true){
            if (poly1First->isTraverse == false){
                poly1First->isTraverse = true;
                traver1++;
                if (poly1First->ptType == cross ||
                    (poly1First->ptType == vertex && is_point_in_polygon(*poly1First->pt, poly2)))
                {
                    break;
                }
            }
            poly1First = poly1First->next;

            if (traver1 >= poly1.size() + crossNum){
                return polyInter;
            }
        }

        Node* cur = poly1First;
        std::vector<point> intersection;
        bool traverseSwitch = true;
        do{
            intersection.push_back(*(cur->pt));
            cur->isTraverse = true;
            if (traverseSwitch){
                traver1++;
            }

            if (cur->ptType == cross && cur != poly1First){
                Node* otherLinkStart = (traverseSwitch?crossWithPoly2:crossWithPoly1);
                while(otherLinkStart->pt != cur->pt){
                    otherLinkStart = otherLinkStart->next;
                }

                cur = otherLinkStart;
                traverseSwitch = !traverseSwitch;
                cur->isTraverse = true;
                if (traverseSwitch){
                    traver1++;
                }
            }
            cur = cur->next;
        }while (cur->pt != poly1First->pt);

        polyInter.push_back(intersection);
    }

    return polyInter;
}



int clock_wise(const std::vector<point>& p){
    int n = p.size();
    const int colckWise = 1;
    const int counterClockWise = -1;
    int i,j,k;
    int count = 0;
    double z;

    if (n < 3)
        return(0);

    for (i=0;i<n;i++) {
        j = (i + 1) % n;
        k = (i + 2) % n;
        z  = (p[j].x - p[i].x) * (p[k].y - p[j].y);
        z -= (p[j].y - p[i].y) * (p[k].x - p[j].x);
        if (z < 0)
            count--;
        else if (z > 0)
            count++;
    }
    if (count > 0)
        return(counterClockWise);
    else if (count < 0)
        return(colckWise);
    else
        return(0);
}


bool is_point_on_segment(const point& p, const point& p1, const point& p2){
    if ((p1.x-p.x)*(p2.y-p.y)-(p2.x-p.x)*(p1.y-p.y)!=0){
        return false;
    }

    if ((p.x>p1.x && p.x>p2.x) || (p.x<p1.x & p.x<p2.x)){
        return false;
    }

    if ((p.y>p1.y && p.y>p2.y) || (p.y<p1.y & p.y<p2.y)){
        return false;
    }

    return true;
}

bool is_point_on_polygon_edge(const point& pt, const std::vector<point>& poly){
    for (int i = 0; i < poly.size(); ++i){
        if (is_point_on_segment(pt, poly[i], poly[(i+1)%poly.size()])){
            return true;
        }
    }

    return false;
}
