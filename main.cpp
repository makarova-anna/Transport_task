#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <fstream>
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



bool In(vector<bool> IN)
{
    for(bool i : IN)
        if (!i)
            return true;
    return false;
}
vector <int> Algorithm(double** M, int n)
{
    int L = 0;
    vector <int> Ans;
    vector <bool> IN (n);
    for (int i = 0; i < n; ++i)
    {
        if (M[L][n] > M[i][n])
            L = i;
        IN[i] = false;
    }
    Ans.push_back(L);
    IN[L] = true;
    while(In(IN))
    {
        IN[L] = true;
        Ans.push_back(L);
    }
    return Ans;
}



//file reading
vector <string> List(string dir)
{
    vector <string> output;
    for (const auto& enter : filesystem::directory_iterator(dir))
    {
        output.push_back(enter.path().string().substr(5));
    }
    return output;
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
        
        
        double return_price()
        {
            return price;
        }
        
        vector<int> return_cycle()
        {
            return the_cycle;
        }
    
        bool done()
        {
            if (cordinates.size() == 2)
            {
                int p = 0;
                vector<vector<bool>> M_true;
                price = distance(cordinates[0], cordinates[1]) * 2;
                the_cycle = { cordinates[0].N, cordinates[1].N};
                
            }
            if (num_of_edge == M.size())
                return true;
            else
                return false;
        }




        void Exterminate_the_line()
        {
            double c, m;
            for (int i = 0; i < M.size(); i++)
            {
                if (deadline[i])
                {
                    continue;
                }
                c = -1;
                m = 1;
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
                        M[i][j] = M[i][j] - c;
                    }
                }
            }
        }
    
        void Exterminate_the_column()
        {
            double c;
            vector<double> hr;
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
                }
                        
                verticalmin[i] = c;
                
                for (int j = 0; j < M.size(); j++)
                {
                    if ((i != j) and (M[j][i] > 0))
                    {
                        M[j][i] = M[j][i] - c;
                    }
                }
            }
        }
    
        double Minimum(int x, int y)
        {
            int i = 0;
            double output = 0;
            double loc_m = -1;
            
            for (i = 0; i < M.size(); i++)
            {
                if (((loc_m == -1) || (loc_m > M[x][i])) && (M[x][i] >= 0) && (i != y))
                {
                    loc_m = M[x][i];
                }
            }
            output = output + loc_m;
            
            //
            loc_m = -1;
            for (i = 0; i < M.size(); i++)
            {
                if (((loc_m == -1) || (loc_m > M[i][y])) && (M[i][y] >= 0) && (i != x))
                {
                    loc_m = M[x][i];
                }
            }
            output = output + loc_m;
            
            return output;
        }
    
        void Edge_price()
        {
            double c = 0, e_p = 0;//c for counter
            int i, j;
            pair <int, int> temp;
            for (i = 0; i < M.size(); i++)
                for (j = 0; j < M.size(); j++)
                    if (M[i][j] == 0)
                        if (Minimum(i, j) > c)
                        {
                            temp = make_pair(i, j);
                            e_p = M_tempo[i][j]*2.4;
                            
                            if (find(the_cycle.begin(), the_cycle.end(), cordinates[i].N) == the_cycle.end())
                            {
                                the_cycle.push_back(cordinates[i].N);
                            }
                            
                            if (find(the_cycle.begin(), the_cycle.end(), cordinates[j].N) == the_cycle.end())
                            {
                                the_cycle.push_back(cordinates[j].N);
                            }
                        }
            num_of_edge++;
            
            M[temp.second][temp.first] = -1;
            M[temp.first][temp.second] = -1;
            
            price = price + e_p;
        }
    
        void Exterminate_the_matrix()
        {
            int a = -1, c;
            int i, j;
            for (i = 0; i < M.size(); i++)
            {
                c = 0;
                for (j = 0; j < M.size(); j++)
                {
                    if (M[i][j] == -1) 
                    {
                        c++;
                    }
                    if (c >= 2)
                    {
                        deadline[i] = true;
                        for (j = 0; j < M.size(); j++)
                        {
                            M[i][j] = -2;
                        }
                        break;
                    }
                }
            }
    
            for (int i = 0; i < M.size(); i++)
            {
                c = 0;
                for (j = 0; j < M.size(); j++)
                {
                    if (M[j][i] == -1) 
                    {
                         c++;
                    }
                    
                    if (c >= 2)
                    {
                        deadcolumn[i] = true;
                        for (j = 0; j < M.size(); j++)
                        {
                            M[j][i] = -2;
                        }
                        
                        break;
                    }
                }
            }
    
        }
    
};
/////////////class end


int Sol(int N, vector<int> weights, int W, vector<int> values)
{
    vector<int> first(W+1, 0);
    vector<int> second(W+1, 0);
    int output = 0, i;
    
    for (i = 1; i <= N; i++) 
    {
        vector<int> tempo = first;
        first = second;
        second = tempo;
        for (int j = 0; j <= W; j++)
        {
            second[j] = first[j];
            if (j - weights[i - 1] >= 0) 
            {
                second[j] = max(second[j], first[j - weights[i - 1]] + values[i - 1]);
            }
        }
    }
    for (i = W; i >= 0; i--) 
    {
        if(second[i] != 0)
        {
            output = second[i];
            break;
        }
    }
    return output;
}



pair<double, vector<int>> fonetsp(vector<Point>& Coordinates)
{
    int c = 0;
    Graph test(Coordinates);
    
    while (!test.done())
    {
        test.Exterminate_the_line();
        test.Exterminate_the_column();
        test.Edge_price();
        test.Exterminate_the_matrix();
        c++;
    }
    
    return make_pair(test.return_price(), test.return_cycle());
}


Point Divide(string& DATA, int N, string sym = "")
{
    int tr_it;
    if (DATA.find("  ") != string::npos) 
    {
        tr_it = 2;
    }
    else tr_it = 1;
    
    auto position = DATA.find(" ");
    string shift = DATA.substr(position + tr_it);
    double c = stod(DATA.substr(0, position));//stod: string -> double
    position = shift.find(" ");
    if (shift.find("  ") != string::npos)
    {
        tr_it = 2;
    }
    else tr_it = 1;
    
    return Point(N, c, stod(shift.substr(0, position)), stod(shift.substr(position + tr_it)));
}




int main()
{
    vector <string> DATA = List("data");
    int N, i;
    string s, ksi;
    double f = -1000;
    ofstream fout;
    fout.open("RES.txt");
    
    
    for (i = 0; i < DATA.size(); i++)
    {
        N = 0;
        ksi = DATA[i];

        cout << "beginning " << ksi << endl << flush;
        vector <Point> Coordinates;
        ifstream file("data/" + ksi);
        getline(file, s);
        
        Point entering = Divide(s, 0);
        while (getline(file, s))
        {
            if ((s == " ")||(s == ""))
            {
                continue;
            }
            
            Coordinates.push_back(Divide(s, N));
            N++;
        }
        
        file.close();
        
        vector<double> vehicles(entering.x, 0);
        double end_ = entering.y, output = 0;
        int it = 0, m = 1;



        while (m < Coordinates.size())
        {
            auto beg_ = Coordinates[m];
            vector<Point> iter;
            iter.push_back(Coordinates[0]);
            
            sort(Coordinates.begin() + m, Coordinates.end(), [&](Point a, Point b)
            {return distance(a, beg_) < distance(b, beg_);});
            
            while ((m < Coordinates.size()) && (vehicles[it] + Coordinates[m].cost < end_) && (it < vehicles.size() - 1))
            {
                vehicles[it] = vehicles[it] + Coordinates[m].cost;
                iter.push_back(Coordinates[m]);
                m++;
            }
            
            it++;
            
            if (it == vehicles.size() - 1)
            {
                while (m < Coordinates.size())
                {
                    vehicles[it] += Coordinates[m].cost;
                    iter.push_back(Coordinates[m]);
                    m++;
                }
            }
            auto tempres = fonetsp(iter);
            output = output + tempres.first;
            if (true)
            {
                fout << it << " : ";
                for (auto k : tempres.second)
                fout << k << " ";
                fout << endl;
            }
        }
        fout << "////////// " << ksi << ":" << output <<endl;
        cout << ksi << " the end " << endl << flush;
    }
    fout.close();
}
