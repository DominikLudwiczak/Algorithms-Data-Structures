#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

int main()
{
    ofstream graph;
    graph.open("graph.txt");

    map<int, vector<int>> used;
    int n=450, random1, random2;
    int max_edges = n*(n-1)/2*0.6;
    srand(time(NULL));

    for(int i=0; i < max_edges; i++)
    {
        random1 = (rand() % n) + 1;
        random2 = (rand() % n) + 1;
        while(random1 == random2 || find(used[max(random1, random2)].begin(), used[max(random1, random2)].end(), min(random1, random2)) != used[max(random1, random2)].end())
        {
            random1 = (rand() % n) + 1;
            random2 = (rand() % n) + 1;
        }

        if(used.count(max(random1, random2)) == 0)
        {
            vector<int> vec;
            vec.push_back(min(random1, random2));
            used.insert({max(random1, random2), vec});
        }
        else
            used[max(random1, random2)].push_back(min(random1, random2));

        graph<<max(random1, random2)<<" "<<min(random1, random2)<<endl;
    }

    graph.close();
    return 0;
}
