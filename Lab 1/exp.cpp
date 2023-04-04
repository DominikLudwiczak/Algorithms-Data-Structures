#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

int tab[100001];

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


int main()
{
    int n=99999;

    for(int i=0; i < 3; i++)
    {
        n += i;
        cout<<n<<endl;
        //A
        genatab(n);
        cout<<"A: "<<QS(0, n-1)<<endl;

        //V
        genvtab(n);
        cout<<"V: "<<QS(0, n-1)<<endl;
    }

    return 0;
}
