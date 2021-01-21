#include <iostream>
#include <vector>
using namespace std;


struct
{

};

//compiler optimization
vector<int> getVector(int n)
{
	vector<int> numbers;
	for (int i = 0; i < n; i++)
	{
		numbers.push_back(i);
	}
	return numbers;
}

int main()
{

}