#include "Set.h"
#include "Bag.h"
#include "Heap.h"
#include <iostream>
using namespace std;

int main() {
	// your code goes here
	Heap<int> h3(10);
	Heap<int> h2(10);
	Heap<int> h1(10);
	Heap<int> h4(10);

	h2.Insert(1);
	h1.Insert(3);
	h4.Insert(5);

	h3 = h1 + h2;
	h4 += h3;
	h4 += 10;
	
	cout << "h4: " << h4 << endl;

	system("pause");
	return 0;
}