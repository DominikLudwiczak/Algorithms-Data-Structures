#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fstream>

using namespace std;

int tab[5000000], tab1[5000000], pom[10000000], pom1[5000000];

void copytab(int n)
{
    for(int i=0; i < n; i++)
        tab1[i] = tab[i];
}

double BS(int n)
{
    clock_t start = clock();

    for(int i=0; i < n-1; i++)
       for(int j=i; j < n; j++)
            if(tab1[i] > tab1[j])
            {
                int p = tab1[i];
                tab1[i] = tab1[j];
                tab1[j] = p;
            }

    clock_t stop = clock();
    return double(stop - start);
}

void heapify(int n, int i)
{
    int largest = i;
    int l = 2*i+1, r = 2*i+2;

    if(l < n && tab1[l] > tab1[largest])
        largest = l;

    if(r < n && tab1[r] > tab1[largest])
        largest = r;

    if(largest != i)
    {
        int p = tab1[i];
        tab1[i] = tab1[largest];
        tab1[largest] = p;

        heapify(n, largest);
    }
}

double HS(int n)
{
    clock_t start = clock();

    for(int i = n/2-1; i >= 0; i--)
        heapify(n, i);

    for(int i = n-1; i > 0; i--)
    {
        int p = tab1[0];
        tab1[0] = tab1[i];
        tab1[i] = p;

        heapify(i, 0);
    }

    clock_t stop = clock();
    return double(stop - start);
}

double CS(int n)
{
    clock_t start = clock();
    int largest = tab1[0];

    for(int i=1; i < n; i++)
        if(tab1[i] > largest)
            largest = tab1[i];

    int cnt=0, sum=0;

    for(int i=0; i <= largest; i++)
        pom[i] = 0;

    for(int i=0; i < n; i++)
        pom[tab1[i]]++;

    for(int i=1; i <= largest; i++)
        pom[i] += pom[i-1];

    for(int i = n-1; i >= 0; i--)
    {
        pom1[pom[tab1[i]]-1] = tab1[i];
        pom[tab1[i]]--;
    }

    //for(int i=0; i < n; i++)
        //tab1[i] = pom1[i];

    clock_t stop = clock();
    return double(stop - start);
}


double ShS(int n)
{
    clock_t start = clock();
    int gap = n/2, temp;

    while(gap >= 1)
    {
        for(int i=gap; i < n; i++)
        {
            temp = tab1[i];
            int j;
            for(j=i; j >= gap && tab1[j - gap] > temp; j-=gap)
                tab1[j] = tab1[j - gap];
            tab1[j] = temp;
        }
        gap /= 2;
    }

    clock_t stop = clock();
    return double(stop - start);
}

int main()
{
    for(int i=0; i < 5000000; i++)
        tab[i] = rand() % 10000000 + 1;

    ofstream result("ex1.txt");
    result<<" ;BS; ;HP;CS;ShS"<<endl;

    int n = 0, nb = 0, p;
    double bs, hs, cs, shs;

    for(int a=0; a < 20; a++)
    {
        n += 250000;
        nb += 3000;

        //BS
        copytab(nb);
        bs = BS(nb);
        //HS
        copytab(n);
        hs = HS(n);
        //CS
        copytab(n);
        cs = CS(n);
        //ShS
        copytab(n);
        shs = ShS(n);

        result<<nb<<";"<<bs<<";"<<n<<";"<<hs<<";"<<cs<<";"<<shs<<endl;
    }

    result.close();
    return 0;
}
