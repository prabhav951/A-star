#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <algorithm>
#include <queue>

#define YELLOW "\033[33m"
#define RESET "\033[0m"
using namespace std;

class Node {
public:
	vector<int> visited;
	int currCity;
	int fcost;

	Node(vector<int> visited, int currCity, int fcost) {
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
	int f = 0;
	// g = cost taken to reach current state
	// h = heuristic
	// f = g + h
	// cout << YELLOW << "PUSH\n" << RESET;
	// cout << "State: 0";
	// cout << " f=" << f << "\n\n";

	q.push(Node({0}, 0, 0));
	while (!q.empty()) {
		Node nd = q.top();
		q.pop();

		// cout << YELLOW << "POP\n" << RESET;
		// for (auto &vertex : nd.visited) {
		// 	cout << " -> " << vertex;
		// }
		// cout << " f=" << nd.fcost << "\n\n";

		if (nd.visited.size() == n && graph[nd.currCity][0] != INT_MAX) {
			cout << "From -> To : Cost\n";
			for (int i = 0; i < n-1; i++) {
				cout << nd.visited[i] << " -> " << nd.visited[i+1] << " : " << graph[nd.visited[i]][nd.visited[i+1]] <<"\n";
			}
			cout << nd.visited[n-1] << " -> " << nd.visited[0] << " : " << graph[nd.visited[n-1]][nd.visited[0]] << "\n";
			
			cout << "\nTotal = " << nd.fcost + graph[nd.currCity][0] << "\n";
			return;
		}

		for (int i = 0; i < n; i++) {
			if (i != nd.currCity && find(nd.visited.begin(), nd.visited.end(), i) == nd.visited.end() && graph[nd.currCity][i] != INT_MAX) {
				vector<int> newVisited = nd.visited;
				newVisited.push_back(i);

				// cout << YELLOW << "PUSH\n" << RESET;
				// for (auto &vertex : newVisited) {
				// 	cout << " -> " << vertex;
				// }

				// cout << " f=" << nd.fcost + graph[nd.currCity][i] << "\n\n";

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