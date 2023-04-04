#include <iostream>

using namespace std;

struct osoba
{
	char* name;
	char* surname;
	int age;
};

using Data = int;

bool dataLess(Data data_1, Data data_2)
{
	//version for int
	if (data_1 < data_2)
	{
		return true;
	}
	return false;
}

Data copyData(Data data)
{
	//version for int
	return data;
}

struct BSTNode
{
	Data data;
	BSTNode *left;
	BSTNode *right;
};

void insertBSTNode(Data data, BSTNode* root)
{

	if (dataLess(data, root->data))//wymuszenie ruchu w lewo
	{
		if (root->left != nullptr)//niepusta lewa ga³¹Ÿ
			insertBSTNode(data, root->left); //
		else //pusta lewa ga³¹Ÿ
		{
			root->left = new BSTNode;
			root->left->data = copyData(data);
			root->left->left = nullptr;
			root->left->right = nullptr;
		}
	}
	else // ruch w prawo
	{
		if (root->right != nullptr) // niepusta prawa ga³¹Ÿ
			insertBSTNode(data, root->right);
		else //pusta prawa ga³¹Ÿ
		{
			root->right = new BSTNode;
			root->right->data = copyData(data);
			root->right->left = nullptr;
			root->right->right = nullptr;
		}
	}
}

void insertBST(Data data, BSTNode*& root)
{
	if (root != nullptr)
		insertBSTNode(data, root);
	else
	{
		root = new BSTNode;
		root->data = data;
		root->left = nullptr;
		root->right = nullptr;
	}
}

int main()
{
	BSTNode* root = nullptr;

	insertBST(10, root);
	insertBST(15, root);
	insertBST(12, root);
	insertBST(7, root);
	insertBST(3, root);
	insertBST(17, root);

	return 0;
}