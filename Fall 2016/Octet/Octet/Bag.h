#ifndef BAG_H
#define BAG_H

#include <iostream>
#include <exception>

class Bag
{


public:
	Bag();
	Bag(Bag &b);
	~Bag();

	void add(int num);
	bool find(int num);
	void remove(int num);

private:
	int a[256];
	int index, bit, onebit;
	std::exception e;

};


#endif