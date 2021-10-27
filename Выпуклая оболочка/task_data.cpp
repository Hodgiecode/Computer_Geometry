#include "task_data.h"
#include <iostream>

int orientation(point p1, point p2, point p3){
    double eps = 1e-9;
    double val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);

    if (fabs(val) < eps){
        return 0; //колинеарны
    }

    if (val > 0){
        return 1; //по часовой
    }

    if (val < 0){
        return 2; //против часовой стрелки
    }
}

bool task_data :: run()
{
    //через алгоритм Джарвиса

   int most_left_point = 0;
   int n = vector_of_points.size();

   if (n < 3){
       return false;
   }

   for (int i = 1; i < n; i++){
       if (vector_of_points[i].x < vector_of_points[most_left_point].x){
           most_left_point = i;
       }
   }

   int p = most_left_point;
   int q = 0;

   while (true){
       result_vector.push_back(vector_of_points[p]);
       q = (p+1) % n;
       for (int i=0; i<n;i++){
           if (orientation(vector_of_points[p],
                           vector_of_points[i],
                           vector_of_points[q]) == 2){
               q = i;
           }
       }

       if (q == most_left_point){
            break;
       }

       p = q;
   }


   //for (int i = 0; i < result_vector.size(); i++)
           //std::cout << "(" << result_vector[i].x << ", "
             //    << result_vector[i].y << ")\n";

    return true ;
}
