#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

int RandInt(int i, int j) {
	int secret;
	secret = rand() % j + i;
	return secret;
}

int partition(int a[], int p, int r)
{
	int pivot = a[p];       // pivot is initially the first element 
	int i = p, j = r;
	do
	{
		do { i++; } while (a[i] < pivot);
		do { j--; } while (a[j] > pivot);
		if (i < j)  swap(a[i], a[j]);
	} while (i < j);
	a[p] = a[j];  a[j] = pivot;
	return j;
	// j is the final location of the pivot
}


int qselect(int A[], int p, int r, int k)
{
	if (p == r) return A[p];
	else
	{
		int q = partition(A, p, r + 1);
		int l = q - p + 1;
		if (k == l) return A[q];
		else if (k < l) return qselect(A, p, q - 1, k);
		else return qselect(A, q + 1, r, k - l);
	}
}


int main() {

	srand(time(NULL));
	int A[1002];
	int n = 1000, temp, t, secret;
	int k[7] = { 2, 100, 300, 500, 700, 900, 999 };

	for (int i = 1; i <= n; i++) A[i] = i;

	for (int i = 2; i <= n; i++) {
		t = RandInt(1, i);
		temp = A[i];
		A[i] = A[t];
		A[t] = temp;
	}
	A[n + 1] = 1001;
	//for (int i = 1; i <= n; i++) cout << A[i] << "\t";
	//cout << endl;
	for (int i = 0; i < 7; i++)
		cout << "(k: " << k[i] << ", kth smallest: " << qselect(A, 1, n, k[i]) << ")\n";

	system("pause");
	return 0;
}

