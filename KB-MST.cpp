#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

class UnionFind { // OOP style
private: vector <int> p, rank; 
public:
	UnionFind(int N) {
		rank.assign(N, 0);
		p.assign(N, 0); for (int i = 0; i < N; i++) p[i] = i;
	}
	int findSet(int i) { return (p[i] == i) ? i : (p[i] = findSet(p[i])); }
	bool isSameSet(int i, int j) { return findSet(i) == findSet(j); }
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) { // if from different set
			int x = findSet(i), y = findSet(j);
			if (rank[x] > rank[y]) p[y] = x; // rank keeps the tree short
			else {
				p[x] = y;
				if (rank[x] == rank[y]) rank[y]++;
			}
		}
	}
};

bool compFunc(const pair<pair<int,int>, int> &i, const pair<pair<int, int>, int> &j)
{
	return i.second < j.second;
}

int main() {

	ifstream inputFile ("graph.txt");
	ofstream outputFile;

	int n;
	int **arr = new int*[1];

	if (inputFile.is_open()) {
		while (inputFile >> n) {	
			
			arr = new int*[n];
			for (int i = 0; i < n; i++) arr[i] = new int[n];

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					inputFile >> arr[i][j];
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j != 0) cout << " ";
			cout << arr[i][j];
		}
		cout << endl;
	}
	
	vector<pair<pair<int, int>, int>> EdgeList;

	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (arr[i][j] > 0) {
				EdgeList.push_back(make_pair(make_pair(i, j), arr[i][j]));
			}
		}
	}

	sort(EdgeList.begin(), EdgeList.end(), compFunc);

	/*
	for (int i = 0; i < EdgeList.size(); i++) {
		cout << EdgeList[i].first.first << " " << EdgeList[i].first.second << " " << EdgeList[i].second << endl;
	}
	*/

	int mst_cost = 0;
	UnionFind UF(n); 
	for (int i = 0; i < EdgeList.size(); i++) { // for each edge, O(E)
		pair<pair<int, int>, int> front = EdgeList[i];
		if (!UF.isSameSet(front.first.first, front.first.second)) { // check
			mst_cost += front.second; // add the weight of e to MST
			UF.unionSet(front.first.first, front.first.second); // link them
		}
	}
	
	printf("%d\n", mst_cost);
	inputFile.close();

	outputFile.open("output_MST.txt");
	outputFile << n << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j != 0) outputFile << " ";
			outputFile << arr[i][j];
		}
		outputFile << endl;
	}
	outputFile << mst_cost << endl;
	outputFile.close();

	for (int i = 0; i < n; i++) {
		delete[] arr[i];
	}

	delete[] arr;

	return 0;
}
