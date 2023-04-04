#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <time.h>

using namespace std;

int tabrand[5000000], tab[5000000], leftArray[2500000], rightArray[2500000];

void print_tab(int n)
{
    for(int i=0; i < n; i++)
        cout<<tab[i]<<endl;

    cout<<"---------------"<<endl;
}

void genrandtab(int n)
{
    for(int i=0; i < n; i++)
        tab[i] = tabrand[i];
}

void genconsttab(int n)
{
    for(int i=0; i < n; i++)
        tab[i] = 0;
}

void geninctab(int n)
{
    for(int i=0; i < n; i++)
        tab[i] = i+1;
}

void gendesctab(int n)
{
    for(int i=0; i < n; i++)
        tab[i] = n-i;
}

void genatab(int n)
{
    int a=1;
    for(int i=0; i < n/2; i++)
    {
        tab[i] = a;
        a += 2;
    }
    a -= 1;
    for(int i=n/2; i < n; i++)
    {
        tab[i] = a;
        a -= 2;
    }
}

void genvtab(int n)
{
    int v = n;
    for(int i=0; i < n/2; i++)
    {
        v -= 2;
        tab[i] = v;
    }
    v += 1;
    for(int i=n/2; i < n; i++)
    {
        tab[i] = v;
        v += 2;
    }
}


void quickSort(int a, int b)
{
    if(b <= a) return;

	int i = a, j = b,
	pivot = tab[(a+b)/2],
	pom;

	while(i <= j)
	{
		while(pivot > tab[i])
            i++;

		while(pivot < tab[j])
            j--;

		if( i <= j)
        {
            pom = tab[i];
            tab[i] = tab[j];
            tab[j] = pom;

            i++;
            j--;
        }
	}

	if(j > a)
        quickSort(a, j);
	if(i < b)
        quickSort(i, b);
}

double QS(int a, int b)
{
    clock_t start = clock();

    quickSort(a, b);

    clock_t stop = clock();
    return double(stop - start);
}


void heapify(int n, int i)
{
    int largest = i;
    int l = 2*i+1, r = 2*i+2;

    if(l < n && tab[l] > tab[largest])
        largest = l;

    if(r < n && tab[r] > tab[largest])
        largest = r;

    if(largest != i)
    {
        int p = tab[i];
        tab[i] = tab[largest];
        tab[largest] = p;

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
        int p = tab[0];
        tab[0] = tab[i];
        tab[i] = p;

        heapify(i, 0);
    }

    clock_t stop = clock();
    return double(stop - start);
}


void mer(int const left, int const mid, int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = tab[left + i];

    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = tab[mid + 1 + j];

    int indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            tab[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            tab[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        tab[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        tab[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(int const a, int const b)
{
    if (a >= b)
        return;

    int mid = a + (b - a) / 2;
    mergeSort(a, mid);
    mergeSort(mid + 1, b);
    mer(a, mid, b);
}


double MS(int const a, int const b)
{
    clock_t start = clock();

    mergeSort(a, b);

    clock_t stop = clock();
    return double(stop - start);
}

int main()
{
    for(int i=1; i <= 5000000; i++)
        tabrand[i] = rand() % 10000000 + 1;

    ofstream result("ex2.txt");
    result<<" ;QS-random;QS-constant;HS-random;HS-constant;MS-random;MS-constant;QS-increasing;QS-descending;HS-increasing;HS-descending;MS-increasing;MS-descending;HS-A;HS-V;MS-A;MS-V; ;QS-A; ;QS-V"<<endl;

    int n=0, nb=0, nc=0;
    double qsr, qsc, hsr, hsc, msr, msc, qsi, qsd, hsi, hsd, msi, msd, qsa, qsv, hsa, hsv, msa, msv;
    for(int i=0; i < 20; i++)
    {
        n += 250000;
        nb += 3000;
        nc += 8500;

        //rand
            //QS
            genrandtab(n);
            qsr = QS(0, n-1);
            //HS
            genrandtab(n);
            hsr = HS(n);
            //MS
            genrandtab(n);
            msr = MS(0, n-1);

        //const
            //QS
            genconsttab(n);
            qsc = QS(0,  n-1);
            //HS
            genconsttab(n);
            hsc = HS(n);
            //MS
            genconsttab(n);
            msc = MS(0, n-1);

        //inc
            //QS
            geninctab(n);
            qsi = QS(0, n-1);
            //HS
            geninctab(n);
            hsi = HS(n);
            //MS
            geninctab(n);
            msi = MS(0, n-1);

        //desc
            //QS
            gendesctab(n);
            qsd = QS(0, n-1);
            //HS
            gendesctab(n);
            hsd = HS(n);
            //MS
            gendesctab(n);
            msd = MS(0, n-1);

        //A
            //QS
            genatab(nb);
            qsa = QS(0, nb-1);
            //HS
            genatab(n);
            hsa = HS(n);
            //MS
            genatab(n);
            msa = MS(0, n-1);

        //V
            //QS
            genvtab(nc);
            qsv = QS(0, nc-1);
            //HS
            genvtab(n);
            hsv = HS(n);
            //MS
            genvtab(n);
            msv = MS(0, n-1);

        result<<n<<";"<<qsr<<";"<<qsc<<";"<<hsr<<";"<<hsc<<";"<<msr<<";"<<msc<<";"<<qsi<<";"<<qsd<<";"<<hsi<<";"<<hsd<<";"<<msi<<";"<<msd<<";"<<hsa<<";"<<hsv<<";"<<msa<<";"<<msv<<";"<<nb<<";"<<qsa<<";"<<nc<<";"<<qsv<<endl;
    }

    result.close();
    return 0;
}
