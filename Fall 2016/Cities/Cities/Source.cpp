#include <iostream>
#include <string>
#include <fstream>
#include "Queuet.h"
#include "Queuet.cpp"
using namespace std;

#define def 128

char numToChar(int i);
int infoFile(int(&v)[128], int(&a)[128][128]);
void BFS(int N, Queuet<int> &Q, int(&v)[128], int(&a)[128][128]);
int minDistance(int N, int dist[], bool sptSet[]);
void printSolution(int N, int dist[]);
void dijkstra(int N, int graph[def][def], int src);

int main() {
	Queuet<int> Q;
	int v[def];
	int A[def][def];

	int N = infoFile(v, A);
	BFS(N, Q, v, A);
	dijkstra(N, A, 1);

	system("pause");
	return 0;
}

char numToChar(int i) {
	return i + 64;
}

//Gather information from cities file
int infoFile(int(&v)[128], int(&a)[128][128]) {
	//States of V
	int unseen = -2;
	int hold = -1;
	int order = 0;

	//Open file
	string file = "CitiesG.txt";
	ifstream in;
	in.open(file);
	int ch;

	//Take the number of cities
	in >> ch;

	//initialize all cities with unseen state
	int N = ch;
	for (int i = 1; i <= N; i++) {
		v[i] = unseen;
	}

	//Take all the weights of cities from file
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			in >> ch;
			a[i][j] = ch;
		}
	}
	in.close();

	return N;
}

void BFS(int N, Queuet<int> &Q, int(&val)[128], int(&a)[128][128])
{
	int unseen = -2;
	int hold = -1;
	int order = 0;
	int t;
	int k = 1;

	Q.enqueue(k);
	while (!Q.queueIsEmpty())
	{
		Q.dequeue(k);
		val[k] = ++order;
		// Scan from left to right
		for (t = 1; t <= N; t++) {
			if (a[k][t] != 0) {
				if (val[t] == unseen) {
					Q.enqueue(t);
					val[t] = hold;
				}
			}
		}
	}

	//Output v (order of visited cities)
	for (int i = 1; i <= N; i++) {
		cout << "Order of City " << numToChar(i) << ": " << val[i] << endl;
	}

}

int minDistance(int N, int dist[], bool sptSet[])
{
	// Initialize min value
	int min = 100, min_index;

	for (int v = 1; v <= N; v++) {
		if (sptSet[v] == false && dist[v] <= min) {
			min = dist[v];
			min_index = v;
		}
	}
	return min_index;
}

void printSolution(int N, int dist[])
{
	//printf("Vertex   Distance from Source\n");
	for (int i = 1; i <= N; i++) {
		cout << numToChar(i) << " " << dist[i] << endl;
	}
	//printf("%d \t\t %d\N", i, dist[i]);
}

void dijkstra(int N, int graph[def][def], int src) {
	/*
	Repeat
	Find j = index of unprocessed node closest to (S)
	Mark (j) as now processed
	For each node (i) not yet processed:
	if (i) is adjacent to (j) then {
	new_distance = Distance[j] + Wij
	if  new_distance < Distance[i] then
	{   Distance[i] = new_distance ;
	Via[i] = j ; }
	}
	Until all vertices are processed
	*/

	int dist[def];     // The output array.  dist[i] will hold the shortest
					   // distance from src to i

	bool sptSet[def]; // sptSet[i] will true if vertex i is included in shortest
					  // path tree or shortest distance from src to i is finalized

					  // Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 1; i <= N; i++) {
		dist[i] = 100;
		sptSet[i] = false;
	}


	// Distance of source vertex from itself is always 0
	dist[1] = 0;

	// Find shortest path for all vertices
	for (int count = 1; count <= N - 1; count++)
	{
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in first iteration.
		int u = minDistance(N, dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		for (int v = 1; v <= N; v++)

			// Update dist[v] only if is not in sptSet, there is an edge from 
			// u to v, and total weight of path from src to  v through u is 
			// smaller than current value of dist[v]
			if (!sptSet[v] && graph[u][v] && dist[u] != 100
				&& dist[u] + graph[u][v] < dist[v])
				dist[v] = dist[u] + graph[u][v];
	}

	// print the constructed distance array
	printSolution(N, dist);


}