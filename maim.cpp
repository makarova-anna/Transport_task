#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cmath>


using namespace std;


class Point
{
    public:
        int N;
        double x, y, cost;
        Point(int n, double c, double ksi, double eta) : N(n), cost(c), x(ksi), y(eta)
        {}
        friend double distance(Point A, Point B);
};

double distance(Point A, Point B)
{
    double d = sqrt((B.x - A.x)*(B.x - A.x)  + (B.y - A.y)*(B.y - A.y)); 
    return d;
}
