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
	long long gcost;
	long long fcost;

	Node(vector<int> visited, int currCity, long long gcost, long long fcost) {
		this->currCity = currCity;
		this->fcost = fcost;
		this->gcost = gcost;
		this->visited = visited;
	}
};

class Cmp {
public:
  bool operator()(Node& nd1, Node& nd2) {
    return nd1.fcost > nd2.fcost;
  }
};

vector<int> findUnvisited(vector<int> visited, int totalNoOfVertices) {
	vector<int> ans;
	unordered_set<int> s(visited.begin(), visited.end());
	for (int i = 0; i < totalNoOfVertices; i++) {
		if (s.find(i) == s.end()) {
      ans.push_back(i);
			// cout << i << " ";
		}
	}
	// cout << "\n";
	return ans;
}

// A utility function to print the constructed MST stored in parent[]
void printMST(vector<int> &vertices, unordered_map<int, int> &parent, vector<vector<long long>> &graph)
{
	int V = vertices.size();
	V == 1 && (cout << "Edge \tWeight\n");
	for (int i = 1; i < V; i++)
		cout << parent[vertices[i]] << " - " << vertices[i] << " \t"
			<< graph[vertices[i]][parent[vertices[i]]] << " \n";
}

// Function to construct an MST for a graph with given vertices
long long primMST(vector<vector<long long>> &graph, vector<int> &vertices) {
	int V = vertices.size();

	if (V == 0) {
		cout << "MST Cost: " << 0 << "\n";
		return 0;
	}

	// Array to store constructed MST
	unordered_map<int, int> parent;

	// Key values used to pick minimum weight edge in cut
	unordered_map<int, int> key;

	// To represent set of vertices included in MST
	unordered_map<int, bool> mstSet;

	// Initialize all keys as INFINITE
	for (auto &vertex : vertices)
		key[vertex] = INT_MAX, mstSet[vertex] = false;

	// Always include first 1st vertex in MST.
	// Make key 0 so that this vertex is picked as first
	// vertex.
	key[vertices[0]] = 0;
	parent[vertices[0]] = -1; // First node is always root of MST

	// The MST will have V vertices
	for (int count = 0; count < V - 1; count++) {
		// Pick the minimum key vertex from the
		// set of vertices not yet included in MST
		// int u = minKey(vertices, key, mstSet);

		int min = INT_MAX, u;

		for (int v = 0; v < V; v++)
			if (mstSet[vertices[v]] == false && key[vertices[v]] < min)
				min = key[vertices[v]], u = vertices[v];

		// Add the picked vertex to the MST Set
		mstSet[u] = true;

		// Update key value and parent index of
		// the adjacent vertices of the picked vertex.
		// Consider only those vertices which are not
		// yet included in MST
		for (int v = 0; v < V; v++)

			// graph[u][v] is non zero only for adjacent
			// vertices of m mstSet[v] is false for vertices
			// not yet included in MST Update the key only
			// if graph[u][v] is smaller than key[v]
			if (u != vertices[v] && graph[u][vertices[v]] != INT_MAX && mstSet[vertices[v]] == false && graph[u][vertices[v]] < key[vertices[v]])
				parent[vertices[v]] = u, key[vertices[v]] = graph[u][vertices[v]];
	}

	// print the constructed MST
	printMST(vertices, parent, graph);

	long long costOfMst = 0;
	for (int i = 1; i < V; i++) {
		if (graph[vertices[i]][parent[vertices[i]]] == INT_MAX)
			return INT_MAX;
		costOfMst += graph[vertices[i]][parent[vertices[i]]];
	}
	cout << "MST Cost: " << costOfMst << "\n";

	return costOfMst;
}

void tsp(vector<vector<long long>> &graph, int n) {
	priority_queue<Node, vector<Node>, Cmp> q;
	long long f, g, h1, h2, h3, h;
	// g = cost taken to reach current state
	// h = heuristic
	// f = g + h

	vector<int> unvisited = findUnvisited({0}, n);
	g = 0;
	h1 = INT_MAX;
	for (auto &vertex : unvisited) {
		h1 = min(h1, graph[0][vertex]);
	}
	if (unvisited.size() == 0) h1 = 0;
	h2 = primMST(graph, unvisited);
	h3 = INT_MAX;
	for (auto &vertex : unvisited) {
		h3 = min(h3, graph[vertex][0]);
	}
	if (unvisited.size() == 0) h3 = 0;
	h = h1+h2+h3;
	f = g + h;

	cout << YELLOW << "PUSH\n" << RESET;
	cout << "State: 0";
	cout << "\nh1=" << h1;
	cout << " h2=" << h2;
	cout << " h3=" << h3;
	cout << " h=" << h;
	cout << " g=" << g;
	cout << " f=" << f << "\n\n";


	q.push(Node({0}, 0, g, f));
	while (!q.empty()) {
		Node nd = q.top();
		q.pop();

		cout << YELLOW << "POP\n" << RESET;
		for (auto &vertex : nd.visited) {
			cout << " -> " << vertex;
		}
		cout << " g=" << nd.gcost;
		cout << " f=" << nd.fcost << "\n\n";

		if (nd.visited.size() == n && graph[nd.currCity][0] < INT_MAX) {
			cout << "From -> To : Cost\n";
			for (int i = 0; i < n-1; i++) {
				cout << nd.visited[i] << " -> " << nd.visited[i+1] << " : " << graph[nd.visited[i]][nd.visited[i+1]] <<"\n";
			}
			cout << nd.visited[n-1] << " -> " << nd.visited[0] << " : " << graph[nd.visited[n-1]][nd.visited[0]] << "\n";
			
			cout << "\nTotal = " << nd.gcost + graph[nd.currCity][0] << "\n";
			return;
		}

		for (int i = 0; i < n; i++) {
			if (i != nd.currCity && find(nd.visited.begin(), nd.visited.end(), i) == nd.visited.end() && graph[nd.currCity][i] < INT_MAX) {
				vector<int> newVisited = nd.visited;
				newVisited.push_back(i);

				unvisited = findUnvisited(newVisited, n);
				g = nd.gcost + graph[nd.currCity][i];
				h1 = INT_MAX;
				for (auto &vertex : unvisited) {
					h1 = min(h1, graph[i][vertex]);
				}
				if (unvisited.size() == 0) h1 = 0;
				h2 = primMST(graph, unvisited);
				h3 = INT_MAX;
				for (auto &vertex : unvisited) {
					h3 = min(h3, graph[vertex][0]);
				}
				if (unvisited.size() == 0) h3 = 0;
				h = h1+h2+h3;
				f = g + h;

				cout << YELLOW << "PUSH\n" << RESET;
				for (auto &vertex : newVisited) {
					cout << " -> " << vertex;
				}
				cout << "\nh1=" << h1;
				cout << " h2=" << h2;
				cout << " h3=" << h3;
				cout << " h=" << h;
				cout << " g=" << g;
				cout << " f=" << f << "\n\n";

				q.push(Node(newVisited, i, g, f));
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

	vector<vector<long long>> adjMat(n, vector<long long>(n));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			input >> adjMat[i][j];
			adjMat[i][j] = (adjMat[i][j] == -1 ? INT_MAX : adjMat[i][j]);
		}
	}

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
