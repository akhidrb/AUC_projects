#include <iostream>
#include <stack>
#include <fstream>
using namespace std;
#define n 4

char numtoChar(int i) {
	return i + 64;
}

int factorial(int num)
{
	return (num == 1 || num == 0) ? 1 : factorial(num - 1) * num;
}

int main() {
	stack<char> graph;
	char num[10000][n];
	for (int i = 1; i <= n; i++) {
		num[0][i] = numtoChar(i); //returns A for i=1, B for i=2 and so on
	}
	int f = factorial(n - 1); //gets the factorial of n-1
	int k1 = n - 1, k2 = n;
	for (int i = 1; i < f; i++) {
		char c1 = num[i - 1][k1];
		char c2 = num[i - 1][k2];
		graph.push(c1);
		graph.push(c2);
		for (int j = 1; j <= n; j++) {
			if (j != k1 && j != k2) {
				num[i][j] = num[i - 1][j];
			}
			else {
				char c;
				c = graph.top();
				graph.pop();
				num[i][j] = c;
			}
		}
		if (k1 > 2) {
			k1--;
			k2--;
		}
		else {
			k1 = n - 1;
			k2 = n;
		}
	}
	int count = 1;
	for (int i = 0; i < f; i++) {
		cout << count++ << ": ";
		for (int j = 1; j <= n; j++) {
			cout << num[i][j] << " ";
		}
		cout << endl;
	}
	system("pause");
	return 0;
}