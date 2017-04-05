#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	int pos[3];
	int n, x;
	cin >> n >> x;
	
	for (int i = 0; i < 3; i++) {
		if (i == x) pos[i] = 1;
		else pos[i] = 0;
	}

	int num = n % 6;

	for (int i = num; i > 0; i--) {
		if (i % 2 == 0) swap(pos[2], pos[1]);
		else swap(pos[0], pos[1]);
	}

	for (int i = 0; i < 3; i++) {
		if (pos[i] == 1) {
			cout << i << endl;
			break;
		}
	}

	return 0;
	system("pause");
}