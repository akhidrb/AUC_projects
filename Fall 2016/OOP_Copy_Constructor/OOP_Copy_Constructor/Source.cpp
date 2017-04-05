#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

class A {
public:
	void print() {
		cout << "id==" << id << "\toid==" << oid << "\tx==" << x << endl;
	}

	A(const A& a) :x(a.x), oid(++id) {
		cout << "A(const A&) ";
		print();
	}

	A(int y = 0) :x(y), oid(++id) {
		cout << "A(int) ";
		print();
	}

	~A() {
		cout << "~A ";
		print();
	}

private:
	int x;
	int oid;
	static int id;
};

int A::id = 0;

class B {
public:
	B(int v) :a(v) {
		cout << "B(int)\n";
		if (v % 2) throw v;
	}
	B(const A& w):a(w) {
		cout << "B(const A&)\n";
	}
	B(const B& v) :a(v.a) {
		cout << "B(const B&)\n";
	}
	~B() {
		cout << "~B()\n";
	}
private: 
	A a;
};

int main() {
	try {
		A a1(1);
		B b2(5);
	}
	catch (int x) {
		cerr << "\n\nBad Value: " << x << endl;
	}

	system("pause");
	return 0;
}