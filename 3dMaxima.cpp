#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "AVLTree.h"
#include "Util.h"

using namespace std;

int noOfPoints;
int** points;
vector<int> setOfMaximalPoints;
vector<int> indices;

void readInInputFile ( string filename )
{
	vector<string> lines;
	string line;
	ifstream myfile ( filename.c_str () );
	if ( myfile.is_open ( ) )
	{
		while ( getline ( myfile, line ) )
		{
			lines.push_back ( line );
		}
		myfile.close ( );
	}
	else
	{
		cout << "Unable to open input file";
		exit ( 0 );
	}

	noOfPoints = atof ( lines[0].c_str () );

	int n = lines.size ( ) - 1;
	int ** tempPoints = new int*[n];
	for ( int i = 0; i < n; ++i )
		tempPoints[i] = new int[4];


	for ( int i = 0; i < n; i++ )
	{
		string tempLine = lines[ i + 1 ];
		string elements[3];
		splitString ( tempLine, " ", elements, 3 );

		for ( int j = 0; j < 3; j++ )
			tempPoints[i][j] = atof ( elements[j].c_str () );
		tempPoints[i][3] = i;
	}
	points = tempPoints;
}

void printResult(string filename)
{
    ofstream ofile(filename);
		ofile<< setOfMaximalPoints.size() << "\n";
    for ( int i = 0; i < setOfMaximalPoints.size(); i++ )
    {
      ofile<< setOfMaximalPoints.at(i) << "\n";
    }
    ofile.close();
}

void swap(int i, int j)
{
    int colLen = 4;
    int* temp = new int[colLen];
    for (int k = 0; k < colLen; k++)
      temp[k] = points[i][k];

    for (int k = 0; k < colLen; k++)
      points[i][k] = points[j][k];

    for (int k = 0; k < colLen; k++)
      points[j][k] = temp[k];
}

int partition (int** points, int low, int high)
{
    int pivot = points[high][0];
    int i = low;

    for (int j = low; j < high; j++)
    {
        if (points[j][0] > pivot){swap(i, j); i++;}
    }
    swap(i, high);
    return i;
}

void quickSort(int** points, int low, int high)
{
    if (low < high)
    {
        int part = partition(points, low, high);
        quickSort(points, low, part - 1);
        quickSort(points, part + 1, high);
    }
}

void preOrder(struct Node *node)
{
    if(node != NULL)
    {
        indices.push_back(node->idx);
        preOrder(node->left);
        preOrder(node->right);
    }
}

int main(int argc, char const *argv[])
{

	readInInputFile(argv[1]);

	int arr[25] = {823, 844, 856, 882, 890, 896, 912, 913, 915, 917, 918, 921, 923, 924, 926, 951, 961, 962, 986, 993, 995, 996, 997, 998, 999};
	int i = 0;
	for (int id = 0; id < 25; id++)
	{
		i = arr[id];
		if (i == 844 || i == 896 || i == 912 || i == 915 || i == 917 || i == 918 || i == 924)
			std::cout << "Extra point -" << '\n';
		std::cout << points[i][0] << " " <<  points[i][1] << " " <<  points[i][2] << " " <<  points[i][3] << '\n';
	}

	quickSort(points, 0, noOfPoints-1);

	struct Node* root = NULL;
	struct Combo* combo = NULL;

	int y = 0, z = 0;
	bool b;
	for (int i = 0; i < noOfPoints; i++)
	{
		y = points[i][1];
		z = points[i][2];
		combo = searchIfMaximal(root, root, y, z, i, points[i][0]);
		root = combo->node;
		b = combo->b;
		// std::cout << count(root) << '\n';
		// if (b)
		// {
		// 	// std::cout << b << '\n';
		// 	setOfMaximalPoints.push_back(points[i][3]);
		// }
	}

	preOrder(root);
	vector<int>::iterator idg;
	for (idg = indices.begin(); idg != indices.end() ; idg++)
		setOfMaximalPoints.push_back(points[*idg][3]);


	sort(setOfMaximalPoints.begin(), setOfMaximalPoints.end());
	printResult(argv[2]);
  return 0;
}
