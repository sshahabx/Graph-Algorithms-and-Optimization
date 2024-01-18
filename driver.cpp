#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>

#include"graph.h"

using namespace std;



int main() {

    auto start=chrono::steady_clock::now();
    // Create a graph from CSV file
    Graph graph = Graph::createGraphFromCSV("input.csv");

    // Get a pointer to the created graph
    Graph* graphPtr = &graph;

    // Call member functions using the pointer
    graphPtr->writeDOTFile("graph.dot");

    cout << "DOT file created successfully." << endl;
    graphPtr->dijkstra(1,"shortest_path_Dijsktra.txt");
    
    graphPtr->bellmanFord(1,"shortest_path_BellmanFord.txt");
    graphPtr->prim(1,"MST_prims.txt");
    
    if (graph.hasCycle()) {
        cout << "The graph contains a cycle.\n";
    } else {
        cout << "The graph does not contain a cycle.\n";
    }

    
    auto finish=chrono::steady_clock::now();
    double elapsed_seconds=chrono::duration<double>(finish-start).count();
    cout.precision(5);
    cout<<" Execution Time : "<<elapsed_seconds<<" seconds "<<endl;

    return 0;
}