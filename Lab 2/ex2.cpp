#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <bits/stdc++.h>
#include <locale>

using namespace std;

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};

//student class
class Student
{
    public:
        string name;
        string surname;
        int index;

        Student(string _name, string _surname, int _index)
        {
            name = _name;
            surname = _surname;
            index = _index;
        }
};

vector<Student*> students;
vector<Student*> ordered_students;
vector<int> to_delete;

void quickSort(int a, int b)
{
    if(b <= a) return;

	int i = a, j = b,
	pivot = ordered_students[(a+b)/2]->index;
	Student* pom;

	while(i <= j)
	{
		while(pivot > ordered_students[i]->index)
            i++;

		while(pivot < ordered_students[j]->index)
            j--;

		if( i <= j)
        {
            pom = ordered_students[i];
            ordered_students[i] = ordered_students[j];
            ordered_students[j] = pom;

            i++;
            j--;
        }
	}

	if(j > a)
        quickSort(a, j);
	if(i < b)
        quickSort(i, b);
}

//node for list
struct listNode
{
    Student* student;
    listNode* next;
};


//node for BST
struct bstNode
{
    Student* student;
    bstNode* left;
    bstNode* right;
};


//list
class List
{
    listNode* head;

public:
    List()
    {
        head = NULL;
    }

    double add_to_list(int n)
    {
        clock_t start = clock();

        for(int i=0; i < n; i++)
        {
            listNode* node = new listNode;
            node->student = ordered_students[i];
            node->next = NULL;

            if(head == NULL)
            {
                head = node;
                node = NULL;
            }
            else
            {
                listNode* curr = head;

                while(curr->next != NULL)
                    curr = curr->next;

                curr->next = node;
            }
        }

        clock_t stop = clock();
        return double(stop - start);
    }


    void search_in_list(int index)
    {
        listNode* curr = head;

        while(curr != NULL)
        {
            if(curr->student->index == index)
                break;
            curr = curr->next;
        }
    }


    void delete_from_list(int index)
    {
        listNode* prev = head;
        listNode* curr = NULL;

        if(head != NULL)
        {
            curr = head->next;

            if(head->student->index == index)
            {
                head = head->next;
                delete prev;
                return;
            }
        }

        while(curr != NULL)
        {
            if(curr->student->index == index)
            {
                prev->next = curr->next;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }


    void print_list()
    {
        listNode* curr = head;

        while(curr != NULL)
        {
            cout<<curr->student->name<<" "<<curr->student->surname<<" "<<curr->student->index<<endl;
            curr = curr->next;
        }
    }
};



//BST
class BST
{
    bstNode* root;

public:
    BST()
    {
        root = NULL;
    }


    void insert_to_tree(bstNode* curr, bstNode* node)
    {
        if(node->student->index <= curr->student->index) //move to left
        {
            if(curr->left == NULL)
                curr->left = node;
            else
                insert_to_tree(curr->left, node);
        }
        else //move to right
        {
            if(curr->right == NULL)
                curr->right = node;
            else
                insert_to_tree(curr->right, node);
        }
    }


    void add_to_tree(Student* _student)
    {
        bstNode* node = new bstNode;
        node->student = _student;
        node->left = NULL;
        node->right = NULL;

        if(root == NULL)
        {
            root = node;
            node = NULL;
        }
        else
            this->insert_to_tree(root, node);
    }




    void search_tree(int index)
    {
        bstNode* curr = root;

        while(curr != NULL)
        {
            if(curr->student->index == index)
                break;

            if(index <= curr->student->index) //move to left
                curr = curr->left;
            else //move to right
                curr = curr->right;
        }
    }



    bstNode* min_in_subtree(bstNode* node)
    {
        bstNode* prev = node;
        while(node->left != NULL)
        {
            prev = node;
            node = node->left;
        }

        return prev;
    }


    bstNode* max_in_subtree(bstNode* node)
    {
        bstNode* prev = node;
        while(node->right != NULL)
        {
            prev = node;
            node = node->right;
        }

        return prev;
    }



    void delete_from_tree(int index)
    {
        bstNode* prev = NULL;
        bstNode* curr = root;
        string last_move = "";

        while(curr != NULL)
        {
            if(curr->student->index == index)
            {
                if(curr->left != NULL || curr->right != NULL)
                {
                    if(curr->right != NULL)
                    {
                        bstNode* min_node = min_in_subtree(curr->right);

                        if(min_node == curr->right)
                        {
                            curr->right->left = curr->left;
                            if(last_move == "right") //right
                                prev->right = min_node;
                            else if(last_move == "left") //left
                                prev->left = min_node;
                            else //root
                            {
                                curr->right->left = root->left;
                                root = curr->right;
                            }
                        }
                        else
                        {
                            min_node->left->left = curr->left;
                            bstNode* pom = min_node->left->right;
                            min_node->left->right = curr->right;

                            if(last_move == "right") //right
                                prev->right = min_node->left;
                            else if(last_move == "left") //left
                                prev->left = min_node->left;
                            else //root
                                root = min_node->left;
                            min_node->left = pom;
                        }
                    }
                    else
                    {
                        bstNode* max_node = max_in_subtree(curr->left);

                        if(max_node == curr->left)
                        {
                            if(last_move == "right") //right
                                prev->right = max_node;
                            else if(last_move == "left") //left
                                prev->left = max_node;
                            else //root
                            {
                                curr->left->right = root->right;
                                root = curr->left;
                            }
                        }
                        else
                        {
                            max_node->right->right = curr->right;
                            bstNode* pom = max_node->right->left;
                            max_node->right->left = curr->left;

                            if(last_move == "right") //right
                                prev->right = max_node->right;
                            else if(last_move == "left") //left
                                prev->left = max_node->right;
                            else //root
                                root = max_node->right;
                            max_node->right = pom;
                        }
                    }
                }
                else
                {
                    if(last_move == "right")
                        prev->right = NULL;
                    else if(last_move == "left")
                        prev->left = NULL;
                    else
                        root = NULL;
                }

                delete curr;
                break;
            }

            prev = curr;
            if(index <= curr->student->index) //move to left
            {
                curr = curr->left;
                last_move = "left";
            }
            else //move to right
            {
                curr = curr->right;
                last_move = "right";
            }
        }
    }





    void printBT(const std::string& prefix, const bstNode* node, bool isLeft)
    {
        if (node != NULL)
        {
            std::cout << prefix;
            std::cout << (isLeft ? "|--" : "'--" );

            // print the value of the node
            std::cout << node->student->index << std::endl;

            // enter the next tree level - left and right branch
            printBT( prefix + (isLeft ? "|   " : "    "), node->left, true);
            printBT( prefix + (isLeft ? "|   " : "    "), node->right, false);
        }
    }

    void printBT()
    {
        printBT("", root, false);
    }
};

double delete_all_from_list(List* student_list, int n)
{
    clock_t start = clock();

    for(int i=0; i < n; i++)
        student_list->delete_from_list(ordered_students[to_delete[i]]->index);

    clock_t stop = clock();
    return double(stop - start);
}

double search_all_elements_in_list(List* student_list, int n)
{
    clock_t start = clock();

    for(int i=0; i < n; i++)
        student_list->search_in_list(ordered_students[to_delete[i]]->index);

    clock_t stop = clock();
    return double(stop - start);
}


void insert_to_bbst(BST* student_tree, int a, int b)
{
    if(a != b)
    {
        int p  = (a+b)/2;
        student_tree->add_to_tree(ordered_students[p]);
        if(a != p && b != p)
        {
            insert_to_bbst(student_tree, a, p);
            insert_to_bbst(student_tree, p+1, b);
        }
    }
}


double insert_all_to_bbst(BST* student_tree, int n)
{
    clock_t start = clock();

    insert_to_bbst(student_tree, 0, n-1);
    student_tree->add_to_tree(ordered_students[n-1]);

    clock_t stop = clock();
    return double(stop - start);
}


double insert_all_to_tree(BST* student_tree, int n)
{
    clock_t start = clock();

    for(int i=0; i < n; i++)
        student_tree->add_to_tree(students[i]);

    clock_t stop = clock();
    return double(stop - start);
}


double delete_all_from_tree(BST* student_tree, int n, string type="bbst")
{
    clock_t start = clock();

    for(int i=0; i < n; i++)
    {
        if(type=="tree")
            student_tree->delete_from_tree(students[to_delete[i]]->index);
        else
            student_tree->delete_from_tree(ordered_students[to_delete[i]]->index);
    }

    clock_t stop = clock();
    return double(stop - start);
}


double search_all_elements_in_tree(BST* student_tree, int n, string type="bbst")
{
    clock_t start = clock();

    for(int i=0; i < n; i++)
    {
            if(type=="tree")
                student_tree->search_tree(students[to_delete[i]]->index);
            else
                student_tree->search_tree(ordered_students[to_delete[i]]->index);
    }

    clock_t stop = clock();
    return double(stop - start);
}


int main()
{
    ifstream file("students.txt");

    if(file.is_open())
    {
        string name, surname;
        int index;
        for(int i=0; i < 1000000; i++)
        {
            file>>name;
            file>>surname;
            file>>index;
            Student* student = new Student(name, surname, index);
            students.push_back(student);
            ordered_students.push_back(student);
        }
        quickSort(0, 999999);
        file.close();
    }
    else
    {
        cout<<"Error while reading file";
        return 0;
    }

    List* student_list = new List();
    BST* student_tree = new BST();
    BST* student_bbst = new BST();
    int n = 0, l=0;

    ofstream result("result.txt");
    result<<" ;insert List;search List; delete List; ;insert BST;insert BBST;search BST;search BBST;delete BST;delete BBST"<<endl;
    result.imbue(std::locale(std::locale::classic(), new Comma));

    double al, ab, abb, sl, sb, sbb, dl, db, dbb;
    int r=3;

    for(int i=0; i < 20; i++)
    {
        n += 50000;
        l += 1000;

        for(int j=0; j < n; j++)
            to_delete.push_back(j);
        random_shuffle(to_delete.begin(), to_delete.end());

        al = 0;
        ab = 0;
        abb = 0;
        sl = 0;
        sb = 0;
        sbb = 0;
        dl = 0;
        db = 0;
        dbb = 0;

        for(int j=0; j < r; j++)
        {
            al += student_list->add_to_list(l)/l;
            sl += search_all_elements_in_list(student_list, l)/l;
            dl += delete_all_from_list(student_list, l)/l;

            ab += insert_all_to_tree(student_tree, n)/n;
            sb += search_all_elements_in_tree(student_tree, n, "tree")/n;
            db += delete_all_from_tree(student_tree, n, "tree")/n;

            abb += insert_all_to_bbst(student_bbst, n)/n;
            sbb += search_all_elements_in_tree(student_bbst, n)/n;
            dbb += delete_all_from_tree(student_bbst, n)/n;
        }

        result<<l<<";";
        result<<al/r<<";";
        result<<sl/r<<";";
        result<<dl/r<<";";

        result<<n<<";";
        result<<ab/r<<";";
        result<<abb/r<<";";

        result<<sb/r<<";";
        result<<sbb/r<<";";

        result<<db/r<<";";
        result<<dbb/r<<endl;

        to_delete.erase(to_delete.begin(), to_delete.end());
        cout<<i<<endl;
    }
    result.close();
    return 0;
}
