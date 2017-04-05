#ifndef SET_H
#define SET_H

#include <iostream>
#include <exception>

class Set
{


public:
	Set(); 
	Set(Set &);
	~Set(); 
	
	void add(int num);
	bool find(int num);
	void remove(int num);

private:
	int a[32];
	int index, bit, onebit;
	std::exception e;

};


#endif