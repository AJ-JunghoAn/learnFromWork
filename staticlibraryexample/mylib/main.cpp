#include <iostream>
#include "..\libServer\Math.h"

using namespace std;

/*
int add(int x, int y)
{

	return x + y;

}
*/


void main()
{

	int a = 10;
	int b = 20;
	
	/*
	int sum = a + b;

	cout << " Sum = " << sum << endl;
	*/


	
	int sum = CMath::add(a, b);

	cout << " Sum = " << sum << endl;
	


}
