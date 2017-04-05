#include "Set.h"

Set::Set()
{
	for (int i = 0; i < 8; i++) a[i] = 0;
}

Set::Set(Set &s)
{
	for (int i = 0; i < 8; i++) a[i] = s.a[i];
}


Set::~Set()
{
}

void Set::add(int num) {

	try {
		if (!find(num)) {
			index = num / 32;
			bit = num % 32;
			a[index] += 1 << bit;
		}
		else throw num;

		std::cout << num << " is added" << std::endl;
	}
	catch (int e) { std::cerr << e << " already exists" << std::endl; }
	catch (...) { std::cerr << "Error!" << std::endl; }
}

bool Set::find(int num)
{	
	try {
		if (num < 256 && num >= 0) {
			index = num / 32;
			bit = num % 32;
			onebit = (a[index] >> bit) % 2;
			if (onebit) return true;
			else return false;
		}
		else throw num;
	}
	catch (int e) { std::cerr << e << " is an invalid number!!" << std::endl; }

	return false;
}

void Set::remove(int num)
{
	if (find(num)) {
		index = num / 32;
		bit = num % 32;
		a[index] -= 1 << bit;
		
		std::cout << num << " is removed." << std::endl;
	}
	else std::cerr << num << " does not exist." << std::endl;
}
