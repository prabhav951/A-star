#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <queue>
using namespace std;

class Node {
public:
	unordered_set<int> visited;
	int currCity;
	int fcost;

	Node(unordered_set<int> visited, int currCity, int fcost) {
		this->currCity = currCity;
		this->fcost = fcost;
		this->visited = visited;
	}
};

class Cmp {
public:
  bool operator()(Node nd1, Node nd2) {
    return nd1.fcost > nd2.fcost;
  }
};

// void tsp(, int n, int count, int cost, int& ans)
void tsp(vector<vector<int>> &graph, int n) {
	priority_queue<Node, vector<Node>, Cmp> q;
	int f, g, h;
	// g = cost taken to reach current state
	// h = heuristic
	// f = g + h

	q.push(Node({0}, 0, 0));
	while (!q.empty()) {
		Node nd = q.top();
		q.pop();

		if (nd.visited.size() == n && graph[nd.currCity][0] != INT_MAX) {
			cout << "ANS = " << nd.fcost + graph[nd.currCity][0];
			return;
		}

		for (int i = 0; i < n; i++) {
			if (i != nd.currCity && nd.visited.find(i) == nd.visited.end() && graph[nd.currCity][i] != INT_MAX) {
				unordered_set<int> newVisited = nd.visited;
				newVisited.insert(i);
				q.push(Node(newVisited, i, nd.fcost + graph[nd.currCity][i]));
			}
		}
	}

	cout << "Solution doesn't exist\n";
}

int main(int argc, char const *argv[]) {
	string inputFilename(argv[1]);
	fstream input(inputFilename);
	
	int n;
	input >> n;

	vector<vector<int>> adjMat(n, vector<int>(n));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			input >> adjMat[i][j];
			adjMat[i][j] = (adjMat[i][j] == -1 ? INT_MAX : adjMat[i][j]);
		}
	}

	vector<int> vertices = {0, 3, 4};

	// Print the solution
	// primMST(adjMat, vertices);
	tsp(adjMat, n);
	return 0;
}

/*
5
0 2 0 6 0
2 0 3 8 5
0 3 0 0 7
6 8 0 0 9
0 5 7 9 0
*/


/*
5
0 1 3 -1 -1
1 0 6 2 4
3 6 0 8 2
-1 2 8 0 3
-1 4 2 3 0
*/
