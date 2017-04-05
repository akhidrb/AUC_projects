#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "RandGen.h"
using namespace std;

#define n 5
#define num 10

void initArray(int(&A)[num]) {
	for (int i = 1; i <= n; i++) A[i] = 0;
}

int RandVar(int(&A)[num], bool cont) {
	int secret;
	secret = rand() % n + 1;
	while (A[secret] == 1 && cont) {
		secret = rand() % n + 1;
	}

	return secret;
}

void VarGen() {
	int secret, count = 0;
	bool cont = true;
	int A[num];

	initArray(A);

	for (int i = 1; i <= n; i++) {

		secret = RandVar(A, cont);

		count++;
		if (count >= n) cont = false;
		A[secret] = 1;
		cout << secret << " ";
	}
}

int main() {
	srand(time(NULL));

	RandGen arr = RandGen();
	arr.sec();

	system("pause");
	return 0;
}