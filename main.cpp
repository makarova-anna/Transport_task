#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <exception>

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

class Graph
{
    private:
        vector<vector<double>> M;
        
        vector<int> the_cycle;
        int num_of_edge = 0;
        double price = 0;    
        vector<double> horizmin;
        vector<double> verticalmin;

        vector<vector<double>> M_tempo;
        vector<Point> cordinates;
        
        vector<bool> deadline;
        vector<bool> deadcolumn;
        
        
    public:
    

        double return_price()
        {
            return price;
        }
        
        
        bool done()
        {
            if (cordinates.size() == 2)
            {
                the_cycle = { cordinates[0].N, cordinates[1].N};
                price = distance(cordinates[0], cordinates[1]) * 2;
            }
            if (num_of_edge == M.size())
                return true;
            else
                return false;
        }

        Graph(vector <Point>& coords)
        { 
            cordinates = coords;
            M.resize(coords.size(), vector<double>(coords.size(), -1000));
            for (int i = 0; i < coords.size(); i++)
            {
                for (int j = 0; j < coords.size(); j++)
                {
                    if (i != j)
                    {
                        M[j][i] = distance(coords[j], coords[i]);
                    }
                }
            }
            
            verticalmin.resize(coords.size(), -1000);
            horizmin.resize(coords.size(), -1000);
            deadcolumn.resize(coords.size(), false);
            deadline.resize(coords.size(), false);
            M_tempo = M;
        }
        
        vector<int> return_cycle()
        {
            return the_cycle;
        }

    
            void Exterminate_the_column()
        {
            double c;
            for (int i = 0; i < M.size(); i++)
            {
                if (deadcolumn[i])
                {   
                    continue;
                }
                c = -1;
                for (int j = 0; j < M.size(); j++)
                {
                    if ((i != j) && (M[j][i] >= 0) && ((c == -1) || (M[j][i] < c)))
                    {
                        c = M[j][i];
                    }
                verticalmin[i] = c;
                for (int j = 0; j < M.size(); j++)
                {
                    if ((i != j) && (M[j][i] > 0))
                    {
                        M[j][i] -= c;
                    }
                }
            }
        }

        void Exterminate_the_line()
        {
            double c;
            for (int i = 0; i < M.size(); i++)
            {
                if (deadline[i])
                {
                    continue;
                }
                c = -1;
                for (int j = 0; j < M.size(); j++)
                {
                    if ((i != j) && (M[i][j] >= 0) && ((c == -1) || (M[i][j] < c)))
                    {
                        c = M[i][j];
                    }
                }
                horizmin[i] = c;
                for (int j = 0; j < M.size(); j++)
                {
                    if ((i != j) && (M[i][j] > 0))
                    {
                        M[i][j] -= c;
                    }
                }
            }
        }
    
