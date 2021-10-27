#include "task_data.h"

double distance(point &p1, point &p2){
    return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}

bool task_data::run_task_one(){
    double min_dist = 1e10;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i=0;i<vector_of_points.size();i++){
            for (int j=0;j<vector_of_points.size();j++){
                if (i!=j){
                    double dist = distance(vector_of_points[i],vector_of_points[j]);
                    if (dist < min_dist){
                        min_dist = dist;
                        nearest_point_vector_index.clear();
                        nearest_point_vector_index.push_back(i);
                        nearest_point_vector_index.push_back(j);
                    }

                }
            }
     }

     std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
     double elapsed_secs = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

     time_algo_one = std::to_string(elapsed_secs);
     dist_algo_one = std::to_string(min_dist);

     return true;
}


/*--------------------*/

struct cmp_x {
    bool operator()(const point &p1, const point &p2) const {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    }
};

struct cmp_y {
    bool operator()(const point &p1, const point &p2) const {
        return p1.y < p2.y;
    }
};

double min_distance;
std::vector<point> buffer;

void update(const point &p1, const point&p2, std::vector<int> &best_pair) {
    double dist = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    if (dist <  min_distance) {
        min_distance = dist;
        best_pair.clear();
        best_pair.push_back(p1.id);
        best_pair.push_back(p2.id);
    }
}


void recursive(int l, int r, std::vector<point> &v1, std::vector<int> &best_pair) {
    if (r - l <= 3) {
        for (int i = l; i < r; ++i) {
            for (int j = i + 1; j < r; ++j) {
                update(v1[i], v1[j], best_pair);
            }
        }

        sort(v1.begin() + l, v1.begin() + r, cmp_y());
        return;
    }

    int m = (l + r) >> 1;
    int midx = v1[m].x;

    recursive(l, m, v1, best_pair);
    recursive(m, r, v1, best_pair);

    merge(v1.begin() + l, v1.begin() + m, v1.begin() + m, v1.begin() + r, buffer.begin(), cmp_y());
    copy(buffer.begin(), buffer.begin() + r - l, v1.begin() + l);

    int tsz = 0;
    for (int i = l; i < r; ++i) {
        if (abs(v1[i].x - midx) < min_distance) {
            for (int j = tsz - 1; j >= 0 && v1[i].y - buffer[j].y < min_distance; --j){
                update(v1[i], buffer[j], best_pair);
            }

            buffer[tsz++] = v1[i];
        }
    }
}

bool task_data::run_task_two(){
    int n = vector_of_points.size();
    std::vector<point> v3;

    for(int i=0;i<n;i++){
        v3.push_back(vector_of_points[i]);
    }


    buffer.resize(n);

    sort(v3.begin(), v3.end(), cmp_x());


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    min_distance = 1e20;
    recursive(0, n, v3, nearest_point_vector_index);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double elapsed_secs = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    std::cout << nearest_point_vector_index[0] << " " << nearest_point_vector_index[1] << std::endl;

    time_algo_two = std::to_string(elapsed_secs);
    dist_algo_two = std::to_string(min_distance);

    return true;
}
