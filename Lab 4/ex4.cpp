#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

using namespace std;

struct Edge{
    int start;
    int finish;
};

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};



class graph
{
    public:
    map<int, vector<int>> incidentList;

    void addVertex(int ver)
    {
        vector<int> vec;
        incidentList.insert({ver, vec});
    }


    void addConnection(int a, int b)
    {
        if(incidentList.count(a) == 0)
            addVertex(b);

        incidentList[a].push_back(b);
    }


    bool check_if_edge_exists(int a, int b)
    {
        for(int i : incidentList[a])
            if(i == b)
                return true;

        return false;
    }



    void generateGraph(int n, double saturation)
    {
        incidentList.clear();
        vector<int> ver;
        int rand1, rand2;
        srand(time(NULL));

        for(int i=1; i <= n; i++)
            ver.push_back(i);
        random_shuffle(ver.begin(), ver.end());

        for(int i=1; i < n; i++)
        {
            addConnection(ver[i-1], ver[i]);
            addConnection(ver[i], ver[i-1]);
        }

        for(int i=0; i < n*(n-1)/2*saturation-n+1; i++)
        {
            rand1 = (rand() % n) + 1;
            rand2 = (rand() % n) + 1;

            while(rand1 == rand2 || check_if_edge_exists(rand1, rand2))
            {
                rand1 = (rand() % n) + 1;
                rand2 = (rand() % n) + 1;
            }

            addConnection(rand1, rand2);
            addConnection(rand2, rand1);
        }
    }

    double check_CE()
    {
        clock_t start = clock();
        int oddDegree = 0, degree;

        for(pair<int, vector<int>> element : incidentList)
        {
            degree = 0;
            for(auto i : element.second)
                if(check_if_edge_exists(element.first, i))
                    degree++;

            if(degree % 2 != 0)
                oddDegree++;
        }

        clock_t stop = clock();
        return double(stop - start);
    }



    bool isSafe(int v, vector<int>& path, int pos)
    {
        if(!check_if_edge_exists(path[pos-1], v))
            return false;

        for (int i = 0; i < pos; i++)
            if (path[i] == v)
                return false;

        return true;
    }


    bool check_CH(vector<int>& path, int pos, clock_t started)
    {
        clock_t now = clock();
        if(double(now - started) <= 300000)
        {
            if(pos == incidentList.size())
                return check_if_edge_exists(path[pos-1], path[0]);

            for (int v = 2; v <= incidentList.size(); v++)
            {
                if (isSafe(v, path, pos))
                {
                    path[pos] = v;

                    if (check_CH(path, pos + 1, started) == true)
                        return true;

                    path[pos] = -1;
                }
            }
        }
        return false;
    }



    double measure_CH()
    {
        clock_t start = clock();

        vector<int> path;
        for(int i=0; i < incidentList.size(); i++)
            path.push_back(-1);
        path[0] = 1;

        check_CH(path, 1, start);

        clock_t stop = clock();
        return double(stop - start);
    }
};


int main()
{
    int n=0, p=10;
    double o2, o3, o4, o6, o8, o95;
    double oh2, oh3, oh4, oh6, oh8, oh95;
    graph gr;
    ofstream result;
    result.open("result.txt");
    result<<" ;sat 0.2;sat 0.3;sat 0.4;sat 0.6;sat 0.8;sat 0.95; ;sat 0.2;sat 0.3;sat 0.4;sat 0.6;sat 0.8;sat 0.95"<<endl;
    result.imbue(locale(locale::classic(), new Comma));

    for(int i=0; i < 10; i++)
    {
        n += 100;
        o2=0, oh2=0;
        o3=0, oh3=0;
        o4=0, oh4=0;
        o6=0, oh6=0;
        o8=0, oh8=0;
        o95=0, oh95=0;

        for(int j=0; j < p; j++)
        {
            gr.generateGraph(n, 0.2);
            o2+=gr.check_CE();
            cout<<"02 "<<n<<endl;
            //cout<<gr.measure_CH()<<endl;
            oh2+=gr.measure_CH();

            gr.generateGraph(n, 0.3);
            o3+=gr.check_CE();
            cout<<"03 "<<n<<endl;
            //cout<<gr.measure_CH()<<endl;
            oh3+=gr.measure_CH();

            gr.generateGraph(n, 0.4);
            o4+=gr.check_CE();
            cout<<"04 "<<n<<endl;
            oh4+=gr.measure_CH();

            gr.generateGraph(n, 0.6);
            o6+=gr.check_CE();
            cout<<"06 "<<n<<endl;
            oh6+=gr.measure_CH();

            gr.generateGraph(n, 0.8);
            o8+=gr.check_CE();
            cout<<"08 "<<n<<endl;
            oh8+=gr.measure_CH();

            gr.generateGraph(n, 0.95);
            o95+=gr.check_CE();
            cout<<"095 "<<n<<endl;
            oh95+=gr.measure_CH();
        }
        result<<n<<";"<<o2/p<<";"<<o3/p<<";"<<o4/p<<";"<<o6/p<<";"<<o8/p<<";"<<o95/p<<"; ;"<<oh2/p<<";"<<oh3/p<<";"<<oh4/p<<";"<<oh6/p<<";"<<oh8/p<<";"<<oh95/p<<endl;
    }
    return 0;
}
