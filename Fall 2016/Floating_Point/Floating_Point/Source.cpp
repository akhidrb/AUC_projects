#include <iostream>
#include <cmath>
using namespace std;

int main() {
	double p, r;
	double i = -20.0;

	while (i < 20) {
		p = 0.15625 / pow(2.0, i);
		p -= 1.0;
		cout << log2(p) << endl;
		i++;
	}

	system("pause");
	return 0;
}