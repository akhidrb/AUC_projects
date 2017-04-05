#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include "RandGen.h"


RandGen::RandGen() {
	for (int i = 1; i <= n; i++) A[i] = 0;
}

RandGen::~RandGen() {
}

void RandGen::sec() {
	
	for (int i = 1; i <= n; i++) {
		secret = rand() % n + 1;
		while (A[secret] == 1 && cont) {
			secret = rand() % n + 1;
		}

		count++;
		if (count >= n) cont = false;
		A[secret] = 1;
		std::cout << secret << " ";
	}

}

