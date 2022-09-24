// Koya Sree Prabhav Sai | BT19CSE055 | AI Assignment 2

/*
Input File Format:
		N
		d11 d12 d13 ... d1N
		d21 d22 d23 ... d2N
		............
		............
		dN1 dN2 dN3 ... dNN

	where
	N = number of cities
	dij = distance between Cityi and Cityj
	If an edge between Cities i and j doesn't exist, then the input for the cell (i, j) should be -1
	
The graph of cities is represented as an adjacency matrix.
In the TSP solver, the state is represented by a vector of visited cities.
If edge weight is -1, it inficates there is no edge between those 2 cities

Commands to execute
	g++ tsp.cpp
	./a.exe filename

Example:
	input.txt
		5
		0 15 13 16 12
		15 0 14 11 17
		13 14 0 19 18
		16 11 19 0 14
		12 17 18 14 0

	i/p:
		g++ tsp.cpp
		./a.exe input.txt

	o/p:
		From -> To : Cost
		0 -> 2 : 13
		2 -> 1 : 14
		1 -> 3 : 11
		3 -> 4 : 14
		4 -> 0 : 12

		Total = 64
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <algorithm>
#include <queue>

#define YELLOW "\033[33m"
#define RESET "\033[0m"
// #define ENABLE_DEBUG
using namespace std;

// Class to store the state (set of visited cities) and its metadata, like current city, gcost (Cost covered till now) and fcost (gcost + heuristic)
class Node {
public:
	vector<int> visited;	// Set of visited cities in order
	int countVisited;		// Number of cities visited
	int currCity;	// current city
	long long gcost;	// g, the cost taken till now to reach current state
	long long fcost;	// f = g + h, where h is the heuristic value at current state

	// Parametrized Constructor
	Node(vector<int> visited, int countVisited, int currCity, long long gcost, long long fcost) {
		this->currCity = currCity;
		this->countVisited = countVisited;
		this->fcost = fcost;
		this->gcost = gcost;
		this->visited = visited;
	}
};

// Comparator Class used to make the Min Heap for "Node" objects, based on fcost
class Cmp {
public:
	// The priority_queue uses this function to maintain the elements sorted in a way that preserves heap properties (i.e., that the element popped is the last according to this strict weak ordering)
  bool operator()(Node& nd1, Node& nd2) {
    return nd1.fcost > nd2.fcost;
  }
};

/**
 * Function to find the unvisited cities, given the set of visited cities.
 * If visited cities = {0, 1} and total cities = 3, then unvisited cities = Total Cities - unvisited cities = {0, 1, 2} - {0, 1} = {2}
 *
 * @param visited Set of visited cities
 * @return Set of unvisited cities
 */
vector<int> findUnvisited(vector<int> visited, int noOfCities) {
	vector<int> unvisited;
	unordered_set<int> s(visited.begin(), visited.end());
	for (int i = 0; i < noOfCities; i++) {
		if (s.find(i) == s.end()) {
      unvisited.push_back(i);
		}
	}
	return unvisited;
}

/**
 * Function to find the MST of the subgraph formed with `vertices`, using Prim's Algorithm.
 *
 * @param graph Adjacency Matrix of the graph
 * @param vertices Set of vertices, of which the MST will be made of
 */
long long primMST(vector<vector<long long>> &graph, vector<int> &vertices) {
	int V = vertices.size();

	if (V == 0) {
		#ifdef ENABLE_DEBUG
			cout << "MST Cost: " << 0 << "\n";
		#endif
		return 0;
	}

	// Map to store constructed MST
	unordered_map<int, int> parent;

	// Key values used to pick minimum weight edge in cut	
	unordered_map<int, int> key;

	// Set of vertices included in MST. We add a vertex into the MST in each iteration
	// mstSet[v] is false for vertices not yet included in MST
	unordered_map<int, bool> mstSet;

	// Initialize all keys as INFINITE
	for (auto &vertex : vertices)
		key[vertex] = INT_MAX, mstSet[vertex] = false;

	// Always include first 1st vertex in MST.
	// Make key 0 so that this vertex is picked as first
	// vertex.
	key[vertices[0]] = 0;
	parent[vertices[0]] = -1; // First node is always root of MST

	// The MST will have V vertices, hence we iterate V times to find V vertices of MST
	for (int count = 0; count < V - 1; count++) {
		// Pick the minimum key vertex from the vertices not yet included in MST
		int min = INT_MAX, u;

		for (int v = 0; v < V; v++)
			if (mstSet[vertices[v]] == false && key[vertices[v]] < min)
				min = key[vertices[v]], u = vertices[v];

		// Add the picked vertex to the mstSet
		mstSet[u] = true;

		// Update key value and parent index of the adjacent vertices of the picked vertex.
		// Consider only those vertices which are not yet included in MST
		for (int v = 0; v < V; v++)
			// Update the key only if graph[u][v] is smaller than key[v], and v is not in mstSet
			if (u != vertices[v] && graph[u][vertices[v]] <= INT_MAX && mstSet[vertices[v]] == false && graph[u][vertices[v]] < key[vertices[v]])
				parent[vertices[v]] = u, key[vertices[v]] = graph[u][vertices[v]];
	}

	// Print the MST
	#ifdef ENABLE_DEBUG
		V == 1 && (cout << "Edge \tWeight\n");
		for (int i = 1; i < V; i++)
			cout << parent[vertices[i]] << " - " << vertices[i] << " \t"
				<< graph[vertices[i]][parent[vertices[i]]] << " \n";
	#endif

	// Find the total cost of MST, i.e., sum of all edges in the MST
	long long costOfMst = 0;
	for (int i = 1; i < V; i++) {
		if (graph[vertices[i]][parent[vertices[i]]] == INT_MAX)	// If graph is disconnected, then return INT_MAX
			return INT_MAX;
		costOfMst += graph[vertices[i]][parent[vertices[i]]];
	}

	#ifdef ENABLE_DEBUG
		cout << "MST Cost: " << costOfMst << "\n";
	#endif
	return costOfMst;
}

/**
 * Function to solve the Travelling Salesman Problem, using A* Search Algorithm and MST Heuristic.
 *
 * @param graph Adjacency Matrix of the graph
 * @param n Number of vertices in the graph
 */
void tspSolver(vector<vector<long long>> &graph, int n) {
	priority_queue<Node, vector<Node>, Cmp> q;	// Min Heap to store Node objects based on fcost
	long long f, g, h1, h2, h3, h, countNodesExpanded = 0, countNodesFringe = 0;

	/*
	These variables are used to store intermediate values of fcost, gcost and heuristic values, 
		while calculating for new Node objects.	
	
	g = g(n), the cost taken to reach state 'n', also known as gcost
	h = h(n), the heuristic value at state 'n'
	f = g + h, the total cost of the state 'n', also known as fcost
	h = h1 + h2 + h3, is the heuristic function, where
		h1 = distance to the nearest unvisited city from the current city
		h2 = estimated distance to travel all the unvisited cities (MST heuristic used here)
		h3 = nearest distance from an unvisited city to the start city
	*/

	/* Finding fcost for initial state */

	vector<int> unvisited = findUnvisited({0}, n);	// Since 0 is the start city, it is considered as visited
	int countUnvisited = unvisited.size();
	g = 0;	// gcost for initial state is 0

	// Calculate h1
	if (countUnvisited == 0) { // If all cities are visited, then h1 = 0
		h1 = 0;
	}
	else {
		h1 = INT_MAX;
		for (auto &vertex : unvisited) {
			h1 = min(h1, graph[0][vertex]);
		}
	}

	// Calculate h2
	h2 = primMST(graph, unvisited);

	// Calculate h3
	if (countUnvisited == 0) {// If all cities are visited, then h3 = 0
		h3 = 0;
	}
	else {
		h3 = INT_MAX;
		for (auto &vertex : unvisited) {
			h3 = min(h3, graph[vertex][0]);
		}
	}

	h = h1+h2+h3;
	f = g + h;

	// Debugging
	#ifdef ENABLE_DEBUG
		cout << YELLOW << "PUSH\n" << RESET;
		cout << "State: 0";
		cout << "\nh1=" << h1;
		cout << " h2=" << h2;
		cout << " h3=" << h3;
		cout << " h=" << h;
		cout << " g=" << g;
		cout << " f=" << f << "\n\n";
	#endif

	q.push(Node({0}, 1, 0, g, f));	// Pushing initial state into the min heap
	while (!q.empty()) {
		Node nd = q.top();	// Pop the element with least fcost
		q.pop();
		countNodesExpanded++;

		// Debugging
		# ifdef ENABLE_DEBUG
			cout << YELLOW << "POP\n" << RESET;
			for (auto &vertex : nd.visited) {
				cout << " -> " << vertex;
			}
			cout << " g=" << nd.gcost;
			cout << " f=" << nd.fcost << "\n\n";
		#endif

		// If all cities are visited and
		// there exists an edge from last city to 0(start city),
		// then we have reached the goal state
		if (nd.countVisited == n && graph[nd.currCity][0] < INT_MAX) {
			cout << "Goal State Reached!\n";
			cout << "From -> To : Cost\n";
			for (int i = 0; i < n-1; i++) {
				cout << nd.visited[i] << " -> " << nd.visited[i+1] << " : " << graph[nd.visited[i]][nd.visited[i+1]] <<"\n";
			}
			cout << nd.visited[n-1] << " -> " << nd.visited[0] << " : " << graph[nd.visited[n-1]][nd.visited[0]] << "\n";
			
			cout << "\nTotal = " << nd.gcost + graph[nd.currCity][0] << "\n";
			cout << "No. of Expanded Nodes: " << countNodesExpanded << "\n";
			cout << "No. of Generated nodes in Fringe List: " << countNodesFringe << "\n";
			return;
		}

		// Converting vector of cities into set for faster search
		unordered_set<int> setVisited(nd.visited.begin(), nd.visited.end());

		// Finding all neighbours where to move next (Successor Function)
		for (int i = 0; i < n; i++) {
			// If there exists an edge from current city to the unvisited neighbour city
			if (i != nd.currCity && setVisited.find(i) == setVisited.end() && graph[nd.currCity][i] < INT_MAX) {
				// Consider we visited the neighbour city
				vector<int> newVisited = nd.visited;
				newVisited.push_back(i);

				/* Finding fcost for initial state */

				unvisited = findUnvisited(newVisited, n);
				countUnvisited = unvisited.size();
				g = nd.gcost + graph[nd.currCity][i];	// gcost for new state = gcost of current state + cost of edge from current state to new state
				
				// Calculate h1
				if (countUnvisited == 0) { // If all cities are visited, then h1 = 0
					h1 = 0;
				}
				else {
					h1 = INT_MAX;
					for (auto &vertex : unvisited) {
						h1 = min(h1, graph[i][vertex]);
					}
				}
				
				// Calculate h2
				h2 = primMST(graph, unvisited);
				
				// Calculate h3
				if (countUnvisited == 0) {// If all cities are visited, then h3 = 0
					h3 = 0;
				}
				else {
					h3 = INT_MAX;
					for (auto &vertex : unvisited) {
						h3 = min(h3, graph[vertex][0]);
					}
				}
				h = h1+h2+h3;
				f = g + h;

				// Debugging
				#ifdef ENABLE_DEBUG
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
				#endif
				
				// Push the Node object of the new state into the Min Heap
				q.push(Node(newVisited, nd.countVisited+1, i, g, f));
				countNodesFringe++;
			}
		}
	}

	// If the while loop ends, all possibilities have been explored, hence no solution exists
	cout << "Solution doesn't exist\n";
}

int main(int argc, char const *argv[]) {
	// Scan filename input from command line
	string inputFilename(argv[1]);
	fstream input(inputFilename);
	
	// Scan number of vertices
	int n;
	input >> n;

	// Scan adjacency matrix of the graph
	vector<vector<long long>> adjMat(n, vector<long long>(n));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			input >> adjMat[i][j];
			// In input, -1 means no edge. In code we replace it with INT_MAX, which is considered as infinity
			adjMat[i][j] = (adjMat[i][j] == -1 ? INT_MAX : adjMat[i][j]);
		}
	}

	tspSolver(adjMat, n);
	return 0;
}
