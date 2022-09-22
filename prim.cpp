#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

// A utility function to print the constructed MST stored in parent[]
void printMST(vector<int> &vertices, unordered_map<int, int> &parent, vector<vector<int>> &graph)
{
	int V = vertices.size();
	cout << "Edge \tWeight\n";
	for (int i = 1; i < V; i++)
		cout << parent[vertices[i]] << " - " << vertices[i] << " \t"
			<< graph[vertices[i]][parent[vertices[i]]] << " \n";
}

// Function to construct an MST for a graph with given vertices
int primMST(vector<vector<int>> &graph, vector<int> &vertices) {
	int V = vertices.size();

	if (V == 0) {
		cout << "Cost: " << 0 << "\n";
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

	int costOfMst = 0;
	for (int i = 1; i < V; i++) {
		if (graph[vertices[i]][parent[vertices[i]]] == INT_MAX)
			return INT_MAX;
		costOfMst += graph[vertices[i]][parent[vertices[i]]];
	}
	cout << "Cost: " << costOfMst << "\n";

	return costOfMst;
}

// Driver's code
int main(int argc, char const *argv[])
{
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

	vector<int> vertices = {0, 1, 2, 3, 4};

	// Print the solution
	primMST(adjMat, vertices);
	return 0;
}
