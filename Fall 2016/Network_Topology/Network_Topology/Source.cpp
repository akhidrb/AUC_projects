#include <iostream>
using namespace std;

void bruteForce(bool(&A)[6][6]) {
	bool star = true, ring = true, mesh = true;
	int count = 0;
	//Mesh
	for (int i = 0; i < 6 && mesh; i++) {
		for (int j = 0; j < 6; j++) {
			count++;
			if (!A[i][j]) { 
				mesh = false;
				break; 
			}
		}
	}
	//Star
	if (!mesh) {
		for (int i = 0; i < 6; i++) {
			count++;
			if (!A[0][i] || !A[i][0]) {
				star = false;
				break;
			}
		}
	}
	cout << "Count: " << count << endl;
	if (mesh) cout << "Fully Connected mesh" << endl;
	else if (star) cout << "Star" << endl;
	else cout << "Ring" << endl;
	
}

int main() {
	bool A[6][6];
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			//if (i == 0 || j == 0 || i == j) A[i][j] = 1;
			//else A[i][j] = 0;
			//A[i][j] = 1;

			//cout << A[i][j];
		}
		//cout << endl;
	}

	bruteForce(A);

	system("pause");
	return 0;
}