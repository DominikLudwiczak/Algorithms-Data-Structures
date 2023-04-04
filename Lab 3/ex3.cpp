#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <list>

using namespace std;

struct Vertex{
    int value;
};

struct Edge{
    Vertex* start;
    Vertex* finish;
};

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};

vector<Vertex*> vertices;

//Vertex Matrix
class VertexMatrix
{
public:
    map<Vertex*, map<Vertex*, bool>> matrix;

    void addVertex(Vertex* ver)
    {
        map<Vertex*, bool> t;
        for(const auto &[key, value] : matrix)
        {
            t.insert({key, false});
            matrix[key].insert({ver, false});
        }

        t.insert({ver, false});
        matrix.insert({ver, t});
    }


    void addConnection(Vertex* start, Vertex* finish)
    {
        if(matrix.count(start) == 0)
            addVertex(start);

        if(matrix.count(finish) == 0)
            addVertex(finish);

        matrix[start][finish] = true;
    }


    bool check_if_edge_egists(Vertex* start, Vertex* finish)
    {
        return matrix[start][finish];
    }
};



//Incident Matrix
class IncidentMatrix
{
public:
    map<Vertex*, map<Edge*, int>> matrix;

    void addVer(Vertex* ver)
    {
        map<Edge*, int> t;
        if(matrix.size() > 0)
            for(pair<Edge*, int> element : matrix.begin()->second)
                t.insert({element.first, 0});
        matrix.insert({ver, t});
    }


    void addEdg(Edge* edg)
    {
        for ( const auto &[key, value]: matrix )
            matrix[key].insert({edg, 0});
    }


    void addConnection(Vertex* ver, Edge* edg, int val)
    {
        if(matrix.count(ver) == 0)
            addVer(ver);

        if(matrix.begin()->second.count(edg) == 0)
            addEdg(edg);

        matrix[ver][edg] = val;
    }


    int check_if_egde_exists(Vertex* start, Vertex* finish)
    {
        map<Edge*, int>::iterator it1 = matrix[start].begin();
        map<Edge*, int>::iterator it2 = matrix[finish].begin();

        while(it1 != matrix[start].end())
        {
            if(it1->second == 1 && it2->second == 1)
                return 1;

            it1++;
            it2++;
        }
        return 0;
    }
};





//Edge List
class EdgeList
{
public:
    vector<Edge*> edges;

    void addEdge(Edge* edge)
    {
        edges.push_back(edge);
    }


    bool check_if_edge_exists(Vertex* start, Vertex* finish)
    {
        for(Edge* edg : edges)
            if(edg->start == start && edg->finish == finish)
                return true;

        return false;
    }
};






//List Of Incidents
class ListOfIncidents
{
public:
    map<Vertex*, vector<Vertex*>> incidentList;

    void addVertex(Vertex* ver)
    {
        vector<Vertex*> vec;
        incidentList.insert({ver, vec});
    }


    void addConnection(Vertex* a, Vertex* b)
    {
        Vertex* ver = a;
        Vertex* conn = b;

        if(incidentList.count(ver) == 0)
            addVertex(ver);

        incidentList[ver].push_back(conn);
    }


    bool check_if_edge_exists(Vertex* a, Vertex* b)
    {
        for(Vertex* i : incidentList[a])
            if(i == b)
                return true;

        return false;
    }

    void erase_list()
    {
        incidentList.clear();
    }


    void remove_all_occur(Vertex* ver)
    {
        for(const auto &[key, value] : incidentList)
        {
            vector<Vertex*>::iterator it = find(incidentList[key].begin(), incidentList[key].end(), ver);
            if(it != incidentList[key].end())
                incidentList[key].erase(it);
            if(incidentList[key].size() == 0)
                incidentList.erase(key);
        }
    }

    void print_li()
    {
        for(pair<Vertex*, vector<Vertex*>> element : incidentList)
        {
            cout<<element.first->value<<" -> ";
            for(Vertex* i : element.second)
                cout<<i->value<<" -> ";
            cout<<endl;
        }
    }

    vector<Vertex*> visited;

    void topological_sort(Vertex* v)
    {
        remove_all_occur(v);
        if(incidentList.size() > 0)
            topological_sort(incidentList.begin()->second[0]);
    }
};




bool check_if_vertex_in_vector(vector<Vertex*>& vec, int val)
{
    bool result = false;

    for(Vertex* i : vec)
        if(i->value == val)
        {
            result = true;
            break;
        }

    return result;
}

Vertex* find_vertex_in_vector(vector<Vertex*>& vec, int val)
{
    for(Vertex* i : vec)
        if(i->value == val)
            return i;
}


VertexMatrix VerMatx;
IncidentMatrix IncMatx;
EdgeList EdgLi;
ListOfIncidents LiInc;


void load_graph(string fname, int n, double saturation, bool undirected = true)
{
    ifstream graph;
    graph.open(fname);
    int vertex1, vertex2, p;
    Vertex* ver1;
    Vertex* ver2;
    Edge* edg;
    Edge* edg2;

    if(undirected)
        p=2;
    else
        p=1;


    for(int i=0; i < n*(n-1)/p*saturation; i++)
    {
        graph>>vertex1;
        graph>>vertex2;

        if(!check_if_vertex_in_vector(vertices, vertex1))
        {
            ver1 = new Vertex;
            ver1->value = vertex1;
            vertices.push_back(ver1);
        }
        else
            ver1 = find_vertex_in_vector(vertices, vertex1);

        if(!check_if_vertex_in_vector(vertices, vertex2))
        {
            ver2 = new Vertex;
            ver2->value = vertex2;
            vertices.push_back(ver2);
        }
        else
            ver2 = find_vertex_in_vector(vertices, vertex2);

        edg = new Edge;
        edg->start = ver1;
        edg->finish = ver2;


        VerMatx.addConnection(ver1, ver2);
        if(undirected)
            VerMatx.addConnection(ver2, ver1);


        IncMatx.addConnection(ver1, edg, 1);
        if(undirected)
            IncMatx.addConnection(ver2, edg, 1);
        else
            IncMatx.addConnection(ver2, edg, -1);


        EdgLi.addEdge(edg);
        if(undirected)
        {
            edg2 = new Edge;
            edg2->start = ver2;
            edg2->finish = ver1;

            EdgLi.addEdge(edg2);
        }


        LiInc.addConnection(ver1, ver2);
        if(undirected)
            LiInc.addConnection(ver2, ver1);
    }
}


double exp_vermat(map<Vertex*, vector<Vertex*>>& random_vertices)
{
    clock_t start = clock();

    for(pair<Vertex*, vector<Vertex*>> element : random_vertices)
        for(auto i : element.second)
            VerMatx.check_if_edge_egists(element.first, i);

    clock_t stop = clock();
    return double(stop - start);
}


double exp_incmat(map<Vertex*, vector<Vertex*>>& random_vertices)
{
    clock_t start = clock();

    for(pair<Vertex*, vector<Vertex*>> element : random_vertices)
        for(auto i : element.second)
            IncMatx.check_if_egde_exists(element.first, i);

    clock_t stop = clock();
    return double(stop - start);
}


double exp_edgli(map<Vertex*, vector<Vertex*>>& random_vertices)
{
    clock_t start = clock();

    for(pair<Vertex*, vector<Vertex*>> element : random_vertices)
        for(auto i : element.second)
            EdgLi.check_if_edge_exists(element.first, i);

    clock_t stop = clock();
    return double(stop - start);
}


double exp_incli(map<Vertex*, vector<Vertex*>>& random_vertices)
{
    clock_t start = clock();

    for(pair<Vertex*, vector<Vertex*>> element : random_vertices)
        for(auto i : element.second)
            LiInc.check_if_edge_exists(element.first, i);

    clock_t stop = clock();
    return double(stop - start);
}



bool CheckAcyclic(int edge[][2], int ed, bool check[], int v)
{
	int i;
	if(check[v] == true)
	{
		return false;
	}
	else
	{
		check[v] = true;
		for(i = ed; i >= 0; i--)
		{
			if(edge[i][0] == v)
			{
				return CheckAcyclic(edge, ed, check, edge[i][1]);
			}
		}
	}
	check[v] = false;

	if(i == 0)
		return true;
}


bool CheckExist(int edge[][2], int a, int b, int size)
{
	for(int i=0; i < size; i++)
	{
		if (edge[i][0] == a && edge[i][1] == b)
			return true;
		if (edge[i][0] == b && edge[i][1] == a)
			return true;
	}
	return false;
}


void GenerateRandGraphs(int v)
{
    LiInc.erase_list();
    vertices.clear();
    int e = v*(v-1)*0.3;
	int i, j, edge[e][2], count;
	Vertex* ver1;
	Vertex* ver2;

	bool check[v+1];

	i = 0;
	while(i < e)
	{
		edge[i][0] = rand()%v+1;
		edge[i][1] = rand()%v+1;

		for(j = 1; j <= v; j++)check[j] = false;

		if(CheckAcyclic(edge, i, check, edge[i][0]) == true && CheckExist(edge, edge[i][0], edge[i][1], i) == false)
			i++;
	}

	for(i = 0; i < v; i++)
	{
		count = 0;
		for(j = 0; j < e; j++)
		{
			if(edge[j][0] == i+1)
			{
			    if(!check_if_vertex_in_vector(vertices, i+1))
                {
                    ver1 = new Vertex;
                    ver1->value = i+1;
                    vertices.push_back(ver1);
                }
                else
                    ver1 = find_vertex_in_vector(vertices, i+1);

                if(!check_if_vertex_in_vector(vertices, edge[j][1]))
                {
                    ver2 = new Vertex;
                    ver2->value = edge[j][1];
                    vertices.push_back(ver2);
                }
                else
                    ver2 = find_vertex_in_vector(vertices, edge[j][1]);


                LiInc.addConnection(ver1, ver2);
				count++;
			}
			else if(edge[j][1] == i+1)
				count++;
		}
	}
}


double exp_sort()
{
    clock_t start = clock();

    LiInc.topological_sort(LiInc.incidentList.begin()->first);

    clock_t stop = clock();
    return double(stop - start);
}





int main()
{
    srand(time(NULL));
    int n=450, random1, random2;
    Vertex* r1;
    Vertex* r2;

    cout<<"loading graph..."<<endl;
    load_graph("graph.txt", n, 0.6);
    cout<<"graph loaded"<<endl;

    int p=0, max_edges, last_max_edges=0;
    ofstream result;
    result.open("result.txt");

    result<<" ;vertex matrix;incident matrix;edge list;list of incidents"<<endl;
    result.imbue(std::locale(std::locale::classic(), new Comma));

    map<int, vector<int>> random_vertices;
    map<Vertex*, vector<Vertex*>> random_vertices_pointers;

    for(int i=0; i < 15; i++)
    {
        p += 30;
        max_edges = p*(p-1)/2;

        for(int j=0; j < max_edges-last_max_edges; j++)
        {
            random1 = (rand() % p);
            random2 = (rand() % p);

            while(random1 == random2 || find(random_vertices[max(random1, random2)].begin(), random_vertices[max(random1, random2)].end(), min(random1, random2)) != random_vertices[max(random1, random2)].end())
            {
                random1 = (rand() % p);
                random2 = (rand() % p);
            }

            if(random_vertices.count(max(random1, random2)) == 0)
            {
                vector<int> vec;
                vec.push_back(min(random1, random2));
                random_vertices.insert({max(random1, random2), vec});
            }
            else
                random_vertices[max(random1, random2)].push_back(min(random1, random2));

            r1 = vertices[random1];
            r2 = vertices[random2];
            if(random_vertices_pointers.count(max(r1, r2)) == 0)
            {
                vector<Vertex*> t;
                t.push_back(min(r1, r2));
                random_vertices_pointers.insert({max(r1, r2), t});
            }
            else
                random_vertices_pointers[max(r1, r2)].push_back(min(r1, r2));
        }
        last_max_edges = max_edges;
        result<<p<<";"<<exp_vermat(random_vertices_pointers)<<";"<<exp_incmat(random_vertices_pointers)<<";"<<exp_edgli(random_vertices_pointers)<<";"<<exp_incli(random_vertices_pointers)<<endl;
    }

    result<<endl;
    result<<" ;sort"<<endl;
    p=0;
    for(int i=0; i < 15; i++)
    {
        p += 50;
        GenerateRandGraphs(p);
        result<<p<<";"<<exp_sort()<<endl;
    }

    result.close();

    return 0;
}
