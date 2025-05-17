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
    cout << "17. Set vertex data by id" << endl;
    cout << "18. Set vertex name by id" << endl;
    cout << "19. Get vertex data by id" << endl;
    cout << "20. Get vertex name by id" << endl;
    cout << "21. v1() by id" << endl;
    cout << "22. v2() by id" << endl;
    cout << "23. SetW by id of 2 vertices" << endl;
    cout << "24. GetW by id of 2 vertices" << endl;
    cout << "25. Set edge data by id of 2 vertices" << endl;
    cout << "26. Get edge data by id of 2 vertices" << endl;
    cout << "27. Vertex begin()" << endl;
    cout << "28. Vertex end()" << endl;
    cout << "29. Vertex++" << endl;
    cout << "30. Vertex*" << endl;
    cout << "31. Vertex==end" << endl;
    cout << "32. Edge begin()" << endl;
    cout << "33. Edge end()" << endl;
    cout << "34. Edge++" << endl;
    cout << "35. Edge*" << endl;
    cout << "36. Edge==end" << endl;
    cout << "37. Out edge begin()" << endl;
    cout << "38. Out edge end()" << endl;
    cout << "39. Out edge++" << endl;
    cout << "40. Out edge*" << endl;
    cout << "41. Out edge==end" << endl;
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

#include <iomanip>  // Для форматирования вывода

void PrintMatrix(const vector<vector<int>>& matrix) {
    if(matrix.empty()) {
        cout << "Matrix is empty!" << endl;
        return;
    }

    const size_t n = matrix.size();
    const int col_width = 3;  // Ширина колонки для выравнивания

    cout << "\nShortest Path Matrix:\n";

    // Заголовок - номера столбцов
    cout << setw(col_width) << " " << " |";
    for(size_t col = 0; col < n; ++col) {
        cout << setw(col_width) << col;
    }
    cout << "\n----|-" << string(n * col_width, '--') << endl;

    // Тело матрицы с номерами строк
    for(size_t row = 0; row < n; ++row) {
        // Номер строки
        cout << setw(col_width) << row << " |";

        // Элементы строки
        for(const auto& val : matrix[row]) {
            if(val == -1) {
                cout << setw(col_width) << "X";
            } else {
                cout << setw(col_width) << val;
            }
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
        cerr << "Exception: " << e.what() << endl;
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
        cerr << "Exception: " << e.what() << endl;
    }
}

template<typename Graph>
void HandleRemoveVertex(Graph& graph) {
    size_t id;
    cout << "Enter vertex ID to remove: ";
    cin >> id;

    try {
        auto vertex = graph.getVertices().at(id);
        cout << graph.DeleteV(vertex) << endl;
    } catch(const exception& e) {
        cerr << "Exception: " << e.what() << endl;
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
        cout << graph.DeleteE(v1, v2) << endl;
    } catch(const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

int main() {
    using Graph = SimpleGraph<string, int, float, string>;
    Graph graph = SimpleGraph<string, int, float, string>(0, true, true);
    int choice;
    bool directed;
    bool dense;
    string str;
    int data;
    float weight;
    size_t vertex_id;
    size_t vertex_id_2;
    Graph::VertexIterator vertexIterator = graph.vertex_begin();
    Graph::EdgeIterator edgeIterator = graph.edges_begin();
    Graph::OutEdgeIterator outEdgeIterator{nullptr};
    Graph::OutEdgeIterator outEdgeIteratorEnd{nullptr};

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
                case 17:
                    cout << "Enter vertex id: ";
                    cin >> vertex_id;
                    cout << "Enter new data: ";
                    cin >> data;
                    cout << graph.vertex_at(vertex_id)->SetData(data) << endl;
                    break;
                case 18:
                    cout << "Enter vertex id: ";
                    cin >> vertex_id;
                    cout << "Enter new name: ";
                    cin >> str;
                    cout << graph.vertex_at(vertex_id)->SetName(str) << endl;
                    break;
                case 19:
                    cout << "Enter vertex id: ";
                    cin >> vertex_id;
                    cout << graph.vertex_at(vertex_id)->GetData() << endl;
                    break;
                case 20:
                    cout << "Enter vertex id: ";
                    cin >> vertex_id;
                    cout << graph.vertex_at(vertex_id)->GetName() << endl;
                    break;
                case 23:
                    cout << "Enter first vertex id: ";
                    cin >> vertex_id;
                    cout << "Enter second vertex id: ";
                    cin >> vertex_id_2;
                    cout << "Enter new weight: ";
                    cin >> weight;
                    try {
                        cout << graph.GetEdge(graph.vertex_at(vertex_id), graph.vertex_at(vertex_id_2))->SetW(weight) << endl;
                    } catch (std::exception &e) {
                        cerr << "Exception: " << e.what() << endl;
                    }
                    break;
                case 24:
                    cout << "Enter first vertex id: ";
                    cin >> vertex_id;
                    cout << "Enter second vertex id: ";
                    cin >> vertex_id_2;
                    cout << graph.GetEdge(graph.vertex_at(vertex_id), graph.vertex_at(vertex_id_2))->GetW() << endl;
                    break;
                case 25:
                    cout << "Enter first vertex id: ";
                    cin >> vertex_id;
                    cout << "Enter second vertex id: ";
                    cin >> vertex_id_2;
                    cout << "Enter new data: ";
                    cin >> str;
                    cout << graph.GetEdge(graph.vertex_at(vertex_id), graph.vertex_at(vertex_id_2))->SetData(str) << endl;
                    break;
                case 26:
                    cout << "Enter first vertex id: ";
                    cin >> vertex_id;
                    cout << "Enter second vertex id: ";
                    cin >> vertex_id_2;
                    cout << graph.GetEdge(graph.vertex_at(vertex_id), graph.vertex_at(vertex_id_2))->GetData() << endl;
                    break;
                case 27:
                    vertexIterator = graph.vertex_begin();
                    break;
                case 28:
                    vertexIterator = graph.vertex_end();
                    break;
                case 29:
                    ++vertexIterator;
                    break;
                case 30:
                    cout << vertexIterator.operator*().GetData() << endl;
                    break;
                case 31:
                    cout << !vertexIterator.operator!=(graph.vertex_end()) << endl;
                    break;
                case 32:
                    edgeIterator = graph.edges_begin();
                    break;
                case 33:
                    edgeIterator = graph.edges_end();
                    break;
                case 34:
                    ++edgeIterator;
                    break;
                case 35:
                    cout << edgeIterator.operator*().GetData() << endl;
                    break;
                case 36:
                    try {
                        *edgeIterator;
                        cout << 0 << endl;
                    } catch (std::exception &e) {
                        cout << 1 << endl;
                    }
                    //cout << !edgeIterator.operator!=(graph.edges_end()) << endl;
                    break;
                case 37:
                    cout << "Enter vertex ID: ";
                    cin >> vertex_id;
                    try {
                        auto vert = graph.vertex_at(vertex_id);
                        outEdgeIterator = graph.out_edges_begin(vert);
                        outEdgeIteratorEnd = graph.out_edges_end(vert);
                        cout << "Out edge iterator initialized for vertex " << vertex_id << endl;
                    } catch (const exception& e) {
                        cerr << "Exception: " << e.what() << endl;
                    }
                    break;
                case 38:
                    cout << "Enter vertex ID: ";
                    cin >> vertex_id;
                    try {
                        auto v = graph.vertex_at(vertex_id);
                        outEdgeIterator = graph.out_edges_end(v);
                        cout << "Out edge end iterator initialized for vertex " << vertex_id << endl;
                    } catch (const exception& e) {
                        cerr << "Exception: " << e.what() << endl;
                    }
                    break;
                case 39:
                    try {
                        ++outEdgeIterator;
                        cout << "Iterator advanced to next out edge" << endl;
                    } catch (const exception& e) {
                        cerr << "Exception: " << e.what() << endl;
                    }
                    break;
                case 40:
                    try {
                        auto edge = *outEdgeIterator;
                        cout << "Current edge: " << edge.v1()->GetId() << " -> " << edge.v2()->GetId();
                        if (edge.IsWeighted()) {
                            cout << " (weight: " << edge.GetW() << ")";
                        }
                        cout << endl;
                    } catch (const exception& e) {
                        cerr << "Exception: " << e.what() << endl;
                    }
                    break;
                case 41:
                    try {
                        bool is_end = (outEdgeIterator != outEdgeIteratorEnd);
                        cout << is_end << endl;
                    } catch (const exception& e) {
                        cerr << "Exception: " << e.what() << endl;
                    }
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }
        } catch(const exception& e) {
            cerr << "Exception: " << e.what() << endl;
        }
    }
}