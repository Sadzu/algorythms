#include <iostream>
#include <limits>
#include <memory>
#include "../headers/SimpleGraph.h"
#include "../headers/ShortestPathTask.h"

using namespace std;

template<typename Graph>
void PrintMenu() {
    cout << "\n===== Graph Operations =====" << endl;
    cout << "1.  Add Vertex" << endl;
    cout << "2.  Add Named Vertex" << endl;
    cout << "3.  Remove Vertex" << endl;
    cout << "4.  Add Edge" << endl;
    cout << "5.  Add Weighted Edge" << endl;
    cout << "6.  Remove Edge" << endl;
    cout << "7.  Visualize Graph" << endl;
    cout << "8.  Convert to List Graph" << endl;
    cout << "9.  Convert to Matrix Graph" << endl;
    cout << "10. Solve Shortest Paths" << endl;
    cout << "13. Show Graph Info" << endl;
    cout << "14. Visualize graph" << endl;
    cout << "15. Make new graph" << endl;
    cout << "16. Generate random graph" << endl;
    cout << "0.  Exit" << endl;
    cout << "============================" << endl;
    cout << "Enter your choice: ";
}

template<typename Graph>
void PrintVertices(const Graph& graph) {
    cout << "Vertices (" << graph.V() << "):" << endl;
    for(const auto& v : graph.getVertices()) {
        cout << "ID: " << v->GetId();
        if(v->IsNamed()) cout << " Name: " << v->GetName();
        cout << endl;
    }
}

void PrintMatrix(const vector<vector<int>>& matrix) {
    cout << "\nShortest Path Matrix:" << endl;
    for(const auto& row : matrix) {
        for(int val : row) {
            if(val == -1) cout << "∞ ";
            else cout << val << " ";
        }
        cout << endl;
    }
}

template<typename Graph>
void HandleShortestPath(Graph& graph) {
    try {
        ShortestPathTask<Graph> task(&graph);
        PrintMatrix(task.Result());
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

template<typename Graph>
void HandleAddEdge(Graph& graph, bool weighted) {
    size_t id1, id2;
    cout << "Enter source vertex ID: ";
    cin >> id1;
    cout << "Enter destination vertex ID: ";
    cin >> id2;

    try {
        auto v1 = graph.getVertices().at(id1);
        auto v2 = graph.getVertices().at(id2);

        if(weighted) {
            float weight;
            cout << "Enter edge weight: ";
            cin >> weight;
            graph.InsertE(v1, v2, weight);
        } else {
            graph.InsertE(v1, v2);
        }
        cout << "Edge added successfully!" << endl;
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

template<typename Graph>
void HandleRemoveVertex(Graph& graph) {
    size_t id;
    cout << "Enter vertex ID to remove: ";
    cin >> id;

    try {
        auto vertex = graph.getVertices().at(id);
        graph.DeleteV(vertex);
        cout << "Vertex removed successfully!" << endl;
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

template<typename Graph>
void HandleRemoveEdge(Graph& graph) {
    size_t id1, id2;
    cout << "Enter source vertex ID: ";
    cin >> id1;
    cout << "Enter destination vertex ID: ";
    cin >> id2;

    try {
        auto v1 = graph.getVertices().at(id1);
        auto v2 = graph.getVertices().at(id2);
        graph.DeleteE(v1, v2);
        cout << "Edge removed successfully!" << endl;
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    using Graph = SimpleGraph<string, int, float, string>;
    Graph graph = SimpleGraph<string, int, float, string>(0, true, true);
    int choice;
    bool directed;
    bool dense;


    while(true) {
        PrintMenu<Graph>();
        cin >> choice;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }

        try {
            switch(choice) {
                case 0:
                    cout << "Exiting..." << endl;
                    return 0;

                case 1: {
                    auto v = graph.InsertV();
                    cout << "Vertex created with ID: " << v->GetId() << endl;
                    break;
                }

                case 2: {
                    string name;
                    cout << "Enter vertex name: ";
                    cin >> name;
                    graph.InsertV(name);
                    cout << "Vertex created successfully!" << endl;
                    break;
                }

                case 3:
                    HandleRemoveVertex(graph);
                    break;

                case 4:
                    HandleAddEdge(graph, false);
                    break;

                case 5:
                    HandleAddEdge(graph, true);
                    break;

                case 6:
                    HandleRemoveEdge(graph);
                    break;

                case 7:
                    PrintVertices(graph);
                    break;

                case 8:
                    graph.ToListGraph();
                    cout << "Converted to List Graph" << endl;
                    break;

                case 9:
                    graph.ToMatrixGraph();
                    cout << "Converted to Matrix Graph" << endl;
                    break;

                case 10:
                    HandleShortestPath(graph);
                    break;

                case 13:
                    cout << "Graph Info:" << endl;
                    cout << "Vertices: " << graph.V() << endl;
                    cout << "Edges: " << graph.E() << endl;
                    cout << "Type: " << (graph.Directed() ? "Directed" : "Undirected") << endl;
                    cout << "Representation: " << (graph.Dense() ? "Matrix" : "List") << endl;
                    cout << "Density: " << graph.K() << endl;
                    break;

                case 14:
                    graph.print();
                    break;

                case 15:
                    int graphType, representation;
                    cout << "Choose graph type:\n";
                    cout << "1. Directed\n2. Undirected\n> ";
                    cin >> graphType;
                    directed = (graphType == 1);

                    cout << "Choose representation:\n";
                    cout << "1. Adjacency List (LGraph)\n2. Adjacency Matrix (MGraph)\n> ";
                    cin >> representation;
                    dense = (representation == 2);

                    int vertexCount;
                    cout << "Enter initial number of vertices: ";
                    cin >> vertexCount;

                    graph = SimpleGraph<string, int, float, string>(vertexCount, directed, dense);
                    cout << "Graph created successfully!\n";
                    break;
                case 16:
                    size_t v, e;
                    cout << "Enter vertices count: ";
                    cin >> v;
                    cout << "Enter edges count: ";
                    cin >> e;
                    graph.GenerateDirectedGraph(v, e);
                    cout << "Graph generated: " << v << " vertices, " << e << " edges\n";
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }
        } catch(const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}