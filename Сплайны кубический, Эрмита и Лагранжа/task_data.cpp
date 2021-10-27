#include "task_data.h"

double L_i(int i, int n, double t){
    double ret = 1.0;
    for(int q = 0; q < n; q++){
        if(q != i){
            ret = ret * ((t - q)/(i - q));
        }
    }

    return ret;
}


point lagrange_point_get(std::vector<point>&pts, double t){
    point ret(0,0,-1);
    int n = pts.size();
    for(int i = 0; i < n; i++){
        ret = ret + L_i(i,n,t)*pts[i];
    }

    return ret;
}

bool task_data::lagrange_spline(){
    int n = vector_of_points.size();
    int curve_points_num = 128*n;

    double step = (double(n-1-0))/(curve_points_num - 1);

    curve.resize(curve_points_num);

    for(int q=0; q < curve.size(); q++){
        curve[q] = lagrange_point_get(vector_of_points, step*q);
    }

     for(int q=0; q < curve.size(); q++){
         std::cout <<curve[q].x << " " << curve[q].y << std::endl;
     }

    return true;
}

/*--------------------------------------------------*/
std::vector <double> new_parametr(std::vector <point>& c_points, int x)
{
    std::vector <double> tmp(c_points.size());
    tmp[0] = 0;
    for (int i = 1; i < c_points.size(); i++)
    {
        if (x == 0)
            tmp[i] = tmp[i - 1] + (c_points[i] - c_points[i - 1]).length();
        else tmp[i] = i;
    }

    return tmp;
}

std::vector <point> GausCub(std::vector <double> &A, std::vector <double> &B, std::vector <double> &C, std::vector <point> &D){
    int i;
    int vectorSize = D.size();
    for (i = 2; i < vectorSize; i++)
    {
        B[i] = B[i] - C[i - 1] * (A[i] / B[i - 1]);
        D[i] = D[i] - D[i - 1] * (A[i] / B[i - 1]);
    }

    std::vector <point> X(vectorSize);
    X[vectorSize - 1] = D[vectorSize - 1] * (1/ B[vectorSize - 1]);
    for (i = vectorSize - 2; i > 0; i--)
    {
        X[i] = (D[i] - C[i] * X[i + 1]) * (1/ B[i]);
    }

    return X;
}

std::vector <point> coefS(std::vector <point>& c_points, std::vector <double> par_t)
{
    int sz = c_points.size();
    int i;
    point null_elem(0,0,-1);
    std::vector <point> tmpSVec(sz), D(sz - 1);
    std::vector <double> A(sz - 1), B(sz - 1), C(sz - 1), X(sz), Y(sz);

    point a, b;

    for (i = 1; i < sz - 1; i++)
    {
        a = 6*((c_points[i + 1] - c_points[i]) *(1/ (par_t[i + 1] - par_t[i])));
        b = 6*((c_points[i] - c_points[i - 1]) *(1/ (par_t[i] - par_t[i - 1])));

        if (i > 1) {
            A[i - 1] = par_t[i] - par_t[i - 1];
        }

        B[i] = 2 * (par_t[i + 1] - par_t[i - 1]);
        C[i] = par_t[i + 1] - par_t[i];
        D[i] = a - b;
    }
    tmpSVec = GausCub(A, B, C, D);

    tmpSVec.resize(sz);
    tmpSVec[sz - 1] = null_elem;
    tmpSVec[0] = null_elem;

    return tmpSVec;
}

point cubic_curve_point_get(std::vector <point>& c_points, double t, std::vector <double> par_t, std::vector <point>& S){
    int i;
    point tmpSVec(0, 0,-1);
    double w;

    for (i = 0; i < par_t.size() - 1; i++)
    {
        if (t >= par_t[i] && t <= par_t[i + 1])
        {
            w = (t - par_t[i]) / (par_t[i + 1] - par_t[i]);
            break;
        }
    }

    point a, b, c, d;
    double e;
    a = (1 - w) * c_points[i];
    b = w * c_points[i + 1];
    c = (-2 * w + 3 * w * w - w * w * w) * S[i];
    d = (-w + w * w *w) * S[i + 1];
    e = ((par_t[i + 1] - par_t[i]) * (par_t[i + 1] - par_t[i])) / 6;

    tmpSVec = a + b + (c + d) * e;

    return tmpSVec;
}

bool task_data::cubic_spline(){
    double t = 0.0;
    std::vector <double> parameter_t;

    int n = vector_of_points.size();
    int curve_points_num = 128*n;

    parameter_t = new_parametr(vector_of_points, 0);
    std::vector <point> S;

    S = coefS(vector_of_points, parameter_t);

    double step = (parameter_t[parameter_t.size() - 1] - 1) / (curve_points_num - 1);
    curve.resize(curve_points_num);


    for(int q=0; q < curve_points_num; q++, t += step)
    {
       curve[q] = cubic_curve_point_get(vector_of_points, t, parameter_t, S);
    }

    return true;
}

/*-------------------------------------------------*/

point hermite_point_get(const std :: vector<point>& pts,
                        const std :: vector<double>& coeff,
                        const std :: vector<point>& tan,
                        double t)
{
  point ret(0, 0,-1);
  int n = pts.size();

  int curve_portion_num = -1;
  if(t >= coeff.back())
    return pts.back();

  for(int q=0; q < coeff.size() - 1; q++)
  {
    if(t >= coeff[q] && t < coeff[q + 1])
      curve_portion_num = q;
  }

  if(curve_portion_num < 0)
     throw 1;

  double w = (t - coeff[curve_portion_num]) / (coeff[curve_portion_num+ 1] - coeff[curve_portion_num]);
  double g0 = 1 - 3.0*pow(w,2) + 2.0*pow(w,3);
  double g1 = w - 2.0*pow(w,2) + pow(w,3);
  double h0 = 3.0*pow(w,2) - 2.0*pow(w,3);
  double h1 = -pow(w,2) + pow(w,3);
  return g0* pts[curve_portion_num] + 1000 * g1*tan[curve_portion_num] +
          h0*pts[curve_portion_num+1] + 1000 * h1*tan[curve_portion_num+1];

}


void compute_hermite_coeff(const std :: vector<point>& pts, std::vector<double> &hermite_coeff)
{
  int num = pts.size();
  if(num < 2)
    return;

  hermite_coeff.resize(num);

  hermite_coeff.resize(num);
  hermite_coeff[0] = 0.0;

  for(int i = 1; i < num; i++) {
    hermite_coeff[i] = (pts[i] - pts[i-1]).length() + hermite_coeff[i-1];
  }
}

bool task_data::hermite_spline(){
    std::vector<double> hermite_coeff;
    compute_hermite_coeff(vector_of_points,hermite_coeff);

    int n = vector_of_points.size();
    int curve_points_num = 100;

    double step = (hermite_coeff.back() - hermite_coeff.front()) / (curve_points_num - 1);

    curve.resize(curve_points_num);

    std::vector<point> tangent_vectors;
    for(size_t q=0; q < curve_points_num; q++){
        curve.push_back(hermite_point_get(vector_of_points, hermite_coeff, tangent_vectors, hermite_coeff[0] +(step * q)));
     }

    return true;
}

