#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <map>
#include <fstream>
#include <cmath>
#include<bits/stdc++.h>

using namespace std;

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};

vector<pair<int, int>> products;

void genProducts(int n)
{
    products.clear();
    srand(time(NULL));
    for(int i=0; i < n; i++)
        products.push_back({rand() % 1000 + 1, rand() % 1000 + 1});
}


void genSpecProducts(int n)
{
    products.clear();
    products.push_back({2, 1});
    for(int i=1; i < n; i++)
        products.push_back({10, 4});
}


int BF_exp(int w, int n)
{
    int j, tempWeight, tempValue, bestValue = 0;

    vector<int> A;
    for(int i=0; i < n; i++)
        A.push_back(0);

    for(int i=0; i < pow(2, n); i++)
    {
        j = n-1;
        tempWeight = 0;
        tempValue = 0;

        while(A[j] != 0 && j > 0)
        {
            A[j] = 0;
            j--;
        }

        A[j] = 1;
        for(int k=0; k < n; k++)
            if(A[k] == 1)
            {
                tempWeight += products[k].first;
                tempValue += products[k].second;
            }

        if(tempValue > bestValue && tempWeight <= w)
            bestValue = tempValue;
    }

    return bestValue;
}

double BF(int w, int n)
{
    clock_t start = clock();

    BF_exp(w, n);

    clock_t stop = clock();
    return double(stop - start);
}


int DP_exp(int w, int n)
{
    vector<vector<int>> mat;

    for(int i=0; i <= n; i++)
    {
        vector<int> pom;
        for(int j=0; j <= w; j++)
            pom.push_back(0);
        mat.push_back(pom);
    }

    for (int item = 1; item <= n; item++)
        for (int capacity = 1; capacity <= w; capacity++)
        {
            int maxValWithoutCurr = mat[item - 1][capacity];
            int maxValWithCurr = 0;
            int weightOfCurr = products[item - 1].first;
            if (capacity >= weightOfCurr)
            {
                maxValWithCurr = products[item - 1].second;

                int remainingCapacity = capacity - weightOfCurr;
                maxValWithCurr += mat[item - 1][remainingCapacity];
            }

            mat[item][capacity] = max(maxValWithoutCurr, maxValWithCurr);
        }

    return mat[n][w];
}


double DP(int w , int n)
{
    clock_t start = clock();

    DP_exp(w, n);

    clock_t stop = clock();
    return double(stop - start);
}


bool cmp(pair<int, float>& a, pair<int, float>& b)
{
    return a.second > b.second;
}

int H_exp(int w, int n)
{
    vector<pair<int, float>> ratios;

    for(int i=0; i < n; i++)
        ratios.push_back({i, (float)products[i].second/(float)products[i].first});

    sort(ratios.begin(), ratios.end(), cmp);
    int packed = 0, value = 0;

    for(int i=0; i < n; i++)
    {
        if(packed + products[ratios[i].first].first <= w)
        {
            packed += products[ratios[i].first].first;
            value += products[ratios[i].first].second;
        }

        if(packed == w)
            break;
    }

    return value;
}

double H(int w, int n)
{
    clock_t start = clock();

    H_exp(w, n);

    clock_t stop = clock();
    return double(stop - start);
}




int main()
{
    ofstream result;
    result.open("result.txt");
    result.imbue(locale(locale::classic(), new Comma));

    /*
    for(int i=5; i <= 25; i++)
    {
        genProducts(i);
        result<<i<<";"<<BF(0.75*i, i)<<endl;
    }

    result<<endl;

    for(int i=100; i <= 2000; i+=100)
    {
        double sum1 = 0, sum2 = 0;
        for(int j=0; j < 10; j++)
        {
            genProducts(i);
            sum1 += DP(0.75*i, i);
            sum2 += H(0.75*i, i);
        }
        result<<i<<";"<<sum1/10<<";"<<sum2/10<<endl;
    }

    result<<endl;
    */

    /*
    for(int i=500; i <= 5000; i+=500)
    {
        double sum = 0, p = 100;

        for(int j=0; j < p; j++)
        {
            genProducts(i);
            int w = 0.75*i;
            sum += (double)H_exp(w, i) / (double)DP_exp(w, i);
        }

        result<<i<<";"<<1<<";"<<1<<";"<<sum/p<<endl;
    }
    */


    genSpecProducts(10);
    cout<<H_exp(10, 10)<<endl;
    cout<<DP_exp(10, 10);

    result.close();
    return 0;
}
