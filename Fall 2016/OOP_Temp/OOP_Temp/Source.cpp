#include <iostream>
using namespace std;

class A {
	static int nextid;
	int id, val;
public:

	void print(string s, ostream& o = std::cout) const {
		o << "A::" << s << " id== " << id
			<< " val== " << val << endl;
	}

	A(int v = 0) :val(v), id(++nextid) {
		print("A(int)");
	}

	A(const A& a) :val(a.val), id(++nextid) {
		print("A(A&)");
	}

	A(A&& a) :val(a.val), id(++nextid) {
		a.val = 0;
		print("A(A&&)");
	}

	~A() {
		print("~A()");
	}

	operator int() {
		print("operator int() ");
		return val;
	}

};

int A::nextid = 0;

A f(A x) {
	return x + 1;
}

int main() {
	cout << "main:\n";
	A a(100);
	A b(f(a));
	cout << "end of main\n";
}

