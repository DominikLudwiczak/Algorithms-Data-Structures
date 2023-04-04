#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;


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


//node for BST
struct bstNode
{
    Student* student;
    bstNode* left;
    bstNode* right;
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


double insert_all_to_tree(BST* student_tree, int n)
{
    clock_t start = clock();

    for(int i=0; i < n; i++)
        student_tree->add_to_tree(students[i]);

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
        for(int i=0; i < 10; i++)
        {
            file>>name;
            file>>surname;
            file>>index;
            Student* student = new Student(name, surname, index);
            students.push_back(student);
        }
        file.close();
    }
    else
    {
        cout<<"Error while reading file";
        return 0;
    }

    BST* student_tree = new BST();

    insert_all_to_tree(student_tree, 10);
    student_tree->printBT();

    return 0;
}
