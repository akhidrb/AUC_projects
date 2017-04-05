#ifndef HEAP_H
#define HEAP_H

template <class Type>
class Heap
{

public:
	Heap<Type>(int MSize) :MaxSize(MSize) {
		array = new Type[MaxSize + 1];
		Nel = 0;
	}
	Heap<Type>(Heap<Type> &h):array(h.array),Nel(h.Nel) {}
	~Heap<Type>() {}

	bool Insert(Type item);
	bool DelMax(Type & item);
	Heap<Type> operator+( Heap<Type>& h);
	Heap<Type> operator+(int n);
	Heap<Type> operator+=(Heap<Type>& h);
	Heap<Type> operator+=(int n);
	Type operator[](int n);
	Heap<Type> operator=(Heap<Type>& h);

private:
	Type *array;
	int MaxSize, Nel;
	void Adjust(Type a[], int i, int n);
};

#endif

template <class Type>
bool Heap<Type>::Insert(Type item) {
	int i = ++Nel;
	if (i == (MaxSize + 1)) {
		std::cout << "heap size exceeded" << std::endl;
		std::cout << "Nel: " << Nel << std::endl;
		Nel--;
		return false;
	}
	while ((i > 1) && (array[i / 2] < item)) {
		array[i] = array[i / 2];
		i /= 2;
	}
	array[i] = item;
	return true;
}

template <class Type>
bool Heap<Type>::DelMax(Type & item) {
	if (!Nel) {
		//std::cout << "heap is empty" << std::endl; 
		return false;
	}
	item = array[1];
	array[1] = array[Nel--];
	Adjust(array, 1, Nel);
	return true;
}

template<class Type>
Heap<Type> Heap<Type>::operator+(Heap<Type>& h)
{
	Heap<Type> heap(MaxSize);
	int i = 1;
	while (i <= h.Nel) {
		Insert(h.array[i]);
		i++;
	}
	heap.array = array;
	heap.Nel = Nel;
	return heap;
}

template<class Type>
Heap<Type> Heap<Type>::operator+(int n)
{
	Heap<Type> heap(MaxSize);
	Insert(n);

	heap.array = array;
	heap.Nel = Nel;
	return heap;
}

template<class Type>
Heap<Type> Heap<Type>::operator+=(Heap<Type>& h)
{
	Heap<Type> heap(MaxSize);
	int i = 1;
	while (i <= h.Nel) {
		Insert(h.array[i]);
		i++;
	}
	heap.array = array;
	heap.Nel = Nel;
	return heap;
}

template<class Type>
Heap<Type> Heap<Type>::operator+=(int n)
{
	Heap<Type> heap(MaxSize);
	Insert(n);

	heap.array = array;
	heap.Nel = Nel;
	return heap;
}

template<class Type>
Type Heap<Type>::operator[](int n)
{
	Type p = NULL;
	try {
		if (n > 0 && n <= Nel) {
			p = array[n];
			return p;
		}
		else throw n;
	}
	catch (int i) {
		std::cerr << i << " is an out of bounds!!" << std::endl;
		return NULL;
	}

}

template<class Type>
Heap<Type> Heap<Type>::operator=(Heap<Type>& h)
{
	Heap<Type> heap(MaxSize);
	array = h.array;
	Nel = h.Nel;

	heap.array = array;
	heap.Nel = Nel;
	return heap;
}

template <class Type>
void Heap<Type>::Adjust(Type a[], int i, int n) {
	int j = 2 * i, item = a[i];
	while (j <= n) {
		if ((j < n) && (a[j] < a[j + 1])) j++;
		if (item >= a[j]) break;
		a[j / 2] = a[j];
		j *= 2;
	}
	a[j / 2] = item;
}

template <class Type>
std::ostream& operator<<(std::ostream& o, Heap<Type>& h)
{
	Type p;
	o << "(";
	h.DelMax(p);
	o << p;
	while (h.DelMax(p)) {
		o << ", " << p;
	}
	o << ")";
	return o;
}
