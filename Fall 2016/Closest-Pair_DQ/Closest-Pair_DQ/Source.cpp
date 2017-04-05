#include <iostream>
#include <cmath>
#include <algorithm>
#include <time.h> 
using namespace std;

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


int closest_pair(int s[], int st, int n) {
	if (n == 1) return -1;
	else if (n == 2) return abs(s[2] - s[1]);
	else {
		int m = partition(s, 1, n+1);
		int s1[100], s2[100];
		int k = 1, f = 1;
		for (int i = 1; i <= n; i++) {
			if (i <= m) {
				s1[k] = s[i];
				k++;
			}
			else {
				s2[f] = s[i];
				f++;
			}
		}

		int d1 = closest_pair(s1, 1, k);
		int d2 = closest_pair(s2, 1, f);
		int d3 = abs(s[m+1] - s[m]);
		int minD = min(d1, d2);
		minD = min(minD, d3);
		return minD;
	}
}

int RandInt(int i, int j) {
	int secret;
	secret = rand() % j + i;
	return secret;
}

int main() {
	srand(time(NULL));
	int A[1002];
	int n = 10, temp, t, secret;
	int k[7] = { 2, 100, 300, 500, 700, 900, 999 };

	for (int i = 1; i <= n; i++) A[i] = i;

	for (int i = 2; i <= n; i++) {
		t = RandInt(1, i);
		temp = A[i];
		A[i] = A[t];
		A[t] = temp;
	}
	A[n + 1] = 1001;
	
	int smallestD = closest_pair(A, 0, n);
	cout << smallestD << endl;

	system("pause");
	return 0;
}