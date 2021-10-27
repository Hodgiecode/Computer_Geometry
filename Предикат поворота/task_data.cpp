#include "task_data.h"
#include <iostream>



bool task_data :: run()
{
    //на вход поступает вектор точек

    point A = vector_of_points[0];
    point B = vector_of_points[1];
    point C = vector_of_points[2];

    double l = (B.x - A.x) * (C.y - A.y);
    double r = (C.x - A.x) * (B.y - A.y);
    double e = (abs(l) + abs(r)) * std::numeric_limits<double>::epsilon() * 4;
    double det = l - r;
    if (det > e){
        result = "left";
        std::cout << result << std::endl;
        return true;
    }

    if (det < -e){
        result = "right";
        std::cout << result << std::endl;
        return true;
    }

    result = "collinear";
    std::cout << result << std::endl;
    return true ;
}

