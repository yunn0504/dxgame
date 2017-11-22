#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

typedef struct position
{
	float x, y, z;
}position;
typedef struct textureUV
{
	float u, v;
}textureUV;
typedef struct triangle
{
	int v1, v2, v3;
	int t1, t2, t3;
}triangle;

int main(void)
{
	ifstream inf;
	ofstream of;
	int v = 0;
	int vt = 0;
	int ff = 0;
	int vertexCount = 0;
	char input;
	vector<vertexTemp> position1;
	vector<textureTemp> texture1;
	vector<indexTemp> triangle1;
	vertexTemp temp;
	textureTemp temp2;
	indexTemp temp3;
	
	inf.open("cube.obj");
	of.open("cube.txt");
	while (!inf.eof())
	{
		inf.get(input);
		if (input == 'v')
		{
			inf.get(input);
			if (input == ' ')
			{
				inf >> temp.x >> temp.y >> temp.z;
				v++;
				of << v << " " << temp.x << " " << temp.y << " " << temp.z << endl;
				position1.push_back(temp);
			}
			else if (input == 't')
			{
				inf >> temp2.u >> temp2.v;
				vt++;
				of << vt << " " << temp2.u << " " << temp2.v << endl;
				texture1.push_back(temp2);
			}
		}
		else if (input == 'f')
		{
			inf.get(input);
			if (input == ' ')
			{
				char tempchar;
				ff++;
				inf >> temp3.v1 >> tempchar >> temp3.t1;
				of << ff << " " << temp3.v1 << " " << temp3.t1 << " ";
				inf >> temp3.v2 >> tempchar >> temp3.t2;
				of << temp3.v2 << " " << temp3.t2 <<" ";
				inf >> temp3.v3 >> tempchar >> temp3.t3;
				of << temp3.v3 << " " << temp3.t3 << endl;
				triangle1.push_back(temp3);
				vertexCount = ff * 3;
			}
		}
	}
	cout << v << " " << vt << " " << ff <<" " <<vertexCount <<endl;
	return 0;
}
