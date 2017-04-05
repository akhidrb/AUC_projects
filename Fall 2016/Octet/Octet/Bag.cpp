#include "Bag.h"

Bag::Bag()
{
	for (int i = 0; i < 256; i++) a[i] = 0;
}

Bag::Bag(Bag & b)
{
	for (int i = 0; i < 256; i++) a[i] = b.a[i];
}

Bag::~Bag()
{
}

void Bag::add(int num)
{
	a[num]++;
}

bool Bag::find(int num)
{
	if (a[num] > 0) return true;
	else return false;
}

void Bag::remove(int num)
{
	a[num]--;
}
