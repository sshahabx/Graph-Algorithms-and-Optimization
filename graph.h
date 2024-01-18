#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <queue>
#include<stack>
#include<set>

using namespace std;

// Data structure to represent a graph
class Graph {
public:
    vector<vector<double> > adjacencyMatrix;

    // Constructor to initialize the graph with a given size
    Graph(int size) : adjacencyMatrix(size, vector<double>(size, 0.0)) {}

    // Function to add an edge to the graph
    void addEdge(int source, int target, double rating) {
        adjacencyMatrix[source][target] = rating;
    }

    // Function to create and return a graph from CSV file
    static Graph createGraphFromCSV(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error opening input file." << endl;
            exit(1);
        }

        // Data storage
        vector<int> sources, targets;
        vector<double> ratings,times;

        // Read CSV file
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string source, target, rating, time;

            if (getline(iss, source, ',') &&
                getline(iss, target, ',') &&
                getline(iss, rating, ',') &&
                getline(iss, time, ',')) {
                sources.push_back(stoi(source));
                targets.push_back(stoi(target));
                ratings.push_back(stod(rating));
                times.push_back(stod(time));
            }
        }

        // Normalize ratings 
        // ratings cant be negative (Dijkstra) so adding by 10
        vector<double> normalizedRatings;
        for (double rating : ratings) {
           double normalizedRating = rating + 10.0;
           normalizedRatings.push_back(normalizedRating);
        }


        ofstream outputFile("normalized_output.csv");
        if (!outputFile.is_open()) {
        cerr << "Error opening output file." << endl;
        return 1;
       }

       for (size_t i = 0; i < sources.size(); ++i) {
        outputFile << sources[i] << "," << targets[i] << "," << normalizedRatings[i] << "," << times[i] << endl;
        }

        // Create a graph and add edges
        int numNodes = *max_element(targets.begin(), targets.end()) + 1;
        Graph graph(numNodes);

        for (size_t i = 0; i < sources.size(); ++i) {
            graph.addEdge(sources[i], targets[i], normalizedRatings[i]);
        }
        

        return graph;
    }
    

    void dijkstra(int source,const string& outputPath) const {
        int numNodes = adjacencyMatrix.size();
        vector<double> distances(numNodes, numeric_limits<double>::infinity());
        vector<bool> visited(numNodes, false);

        distances[source] = 0;

        for (int i = 0; i < numNodes - 1; ++i) {
            int minNode = -1;
            for (int j = 0; j < numNodes; ++j) {
                if (!visited[j] && (minNode == -1 || distances[j] < distances[minNode])) {
                    minNode = j;
                }
            }

            visited[minNode] = true;

            for (int k = 0; k < numNodes; ++k) {
                if (!visited[k] && adjacencyMatrix[minNode][k] > 0) {
                    double newDistance = distances[minNode] + adjacencyMatrix[minNode][k];
                    if (newDistance < distances[k]) {
                        distances[k] = newDistance;
                    }
                }
            }
        }

         // Display the shortest paths
        cout << "Shortest Paths from Node " << source << ":\n";
        for (int i = 1; i < numNodes; ++i) {
            cout << "Node " << i << ": " << distances[i] << "\n";
        }


        ofstream outputFile(outputPath);
        if (!outputFile.is_open()) {
            cerr << "Error opening output file for shortest paths." << endl;
            exit(1);
        }
        
        outputFile<<" Dijkstra's Algorithm"<<endl;
        outputFile << "Shortest Paths from Node " << source << ":\n";
        for (int i = 0; i < numNodes; ++i) {
            if (i != source) {
                outputFile << "Node " << source << " to Node " << i << ": ";
                if (distances[i] < numeric_limits<double>::infinity()) {
                    outputFile << distances[i];
                } else {
                    outputFile << "No path";
                }
                outputFile << "\n";
            }
        }
     
     outputFile.close();
        
    }
    // Function to perform Bellman-Ford algorithm
    void bellmanFord(int source,const string& outputPath) const {
        int numNodes = adjacencyMatrix.size();
        vector<double> distances(numNodes, numeric_limits<double>::infinity());

        distances[source] = 0;

        // Relax edges repeatedly
        for (int i = 0; i < numNodes - 1; ++i) {
            for (int u = 0; u < numNodes; ++u) {
                for (int v = 0; v < numNodes; ++v) {
                    if (adjacencyMatrix[u][v] > 0) {
                        double newDistance = distances[u] + adjacencyMatrix[u][v];
                        if (newDistance < distances[v]) {
                            distances[v] = newDistance;
                        }
                    }
                }
            }
        }

        // Check for negative cycles
        for (int u = 0; u < numNodes; ++u) {
            for (int v = 0; v < numNodes; ++v) {
                if (adjacencyMatrix[u][v] > 0) {
                    double newDistance = distances[u] + adjacencyMatrix[u][v];
                    if (newDistance < distances[v]) {
                        cerr << "Graph contains negative cycle. Bellman-Ford not applicable.\n";
                        return;
                    }
                }
            }
        }

        // Display the shortest paths
        cout << "Shortest paths from node " << source <<endl;
        for (int i = 0; i < numNodes; ++i) {
            cout << "Node " << i << ": " << distances[i] << "\n";
        }

        ofstream outputFile(outputPath);
        if (!outputFile.is_open()) {
            cerr << "Error opening output file for shortest paths." << endl;
            exit(1);
        }
        
        
        outputFile<<" Bellman Ford Algorithm"<<endl;
        outputFile << "Shortest Paths from Node " << source << ":\n";
        for (int i = 0; i < numNodes; ++i) {
            if (i != source) {
                outputFile << "Node " << source << " to Node " << i << ": ";
                if (distances[i] < numeric_limits<double>::infinity()) {
                    outputFile << distances[i];
                } else {
                    outputFile << "No path";
                }
                outputFile << "\n";
            }
        }
             outputFile.close();

    }

    void prim(int source, const string& outputPath) const {
        auto start = chrono::steady_clock::now();

        int numNodes = adjacencyMatrix.size();

        // Vector to store the parent of each node in the MST
        vector<int> parent(numNodes, -1);

        // Vector to store the key values used to pick the minimum weight edge
        vector<double> key(numNodes, numeric_limits<double>::infinity());

        // Priority queue to store the vertices and their key values
        priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;

        // Insert the source vertex into the priority queue
        pq.push(make_pair(0, source));
        key[source] = 0;

        // Loop to construct the MST
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int v = 0; v < numNodes; ++v) {
                double weight = adjacencyMatrix[u][v];
                if (weight > 0 && weight < key[v]) {
                    parent[v] = u;
                    key[v] = weight;
                    pq.push(make_pair(key[v], v));
                }
            }
        }

        // Display the minimum spanning tree
       cout << "Minimum Spanning Tree (Prim's Algorithm):\n";
    ofstream outputFile(outputPath);
    if (!outputFile.is_open()) {
        cerr << "Error opening output file for MST." << endl;
        exit(1);
    }

    outputFile << "Minimum Spanning Tree (Prim's Algorithm):\n";
    for (int i = 2; i < numNodes; ++i) {
        cout << "Edge: " << parent[i] << " - " << i << " Weight: " << key[i] << "\n";
        outputFile << parent[i] << "," << i << "," << key[i] << "\n";
    }

    outputFile.close();

    }

    bool hasCycle() const {
    int numNodes = adjacencyMatrix.size();
    vector<bool> visited(numNodes, false);
    vector<bool> recursionStack(numNodes, false);

    for (int i = 0; i < numNodes; ++i) {
        if (!visited[i]) {
            visited[i] = true;
            recursionStack[i] = true;

            stack<int> dfsStack;
            dfsStack.push(i);

            while (!dfsStack.empty()) {
                int node = dfsStack.top();
                dfsStack.pop();

                for (int neighbor = 0; neighbor < adjacencyMatrix[node].size(); ++neighbor) {
                    if (adjacencyMatrix[node][neighbor] > 0) {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            recursionStack[neighbor] = true;
                            dfsStack.push(neighbor);
                        } else if (recursionStack[neighbor]) {
                            return true; // Cycle detected
                        }
                    }
                }

                recursionStack[node] = false;
            }
        }
    }

    return false;
}


    // Function to write DOT file
    // Modify your writeDOTFile function
void writeDOTFile(const string& filename) const {
    ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        cerr << "Error opening DOT file." << endl;
        exit(1);
    }

    dotFile << "digraph G {\n";
    dotFile << "  node [shape=circle, style=filled, fillcolor=lightblue, fontcolor=black];\n"; // Node attributes
    dotFile << "  edge [color=darkgreen, fontcolor=black];\n"; // Edge attributes

    for (int source = 0; source < adjacencyMatrix.size(); ++source) {
        for (int target = 0; target < adjacencyMatrix[source].size(); ++target) {
            double rating = adjacencyMatrix[source][target];
            if (rating > 0) {
                dotFile << source << " -> " << target << " [label=\"" << rating << "\", weight=" << rating << "];\n";
            }
        }
    }
    dotFile << "}\n";

    dotFile.close();
}

    
};