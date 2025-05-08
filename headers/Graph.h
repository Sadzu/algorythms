#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <memory>

template <typename VertexData = std::string, typename EdgeWeight = int>
class Graph {
public:
    // Типы представления графа
    enum class RepresentationType { LIST, MATRIX };

    // Тип графа
    enum class GraphType { DIRECTED, UNDIRECTED };

    // Дескрипторы вершин и ребер
    using VertexDescriptor = size_t;
    using EdgeDescriptor = size_t;

    // Структуры для хранения данных
    struct VertexInfo {
        VertexData data;
        bool has_name;
    };

    struct EdgeInfo {
        VertexDescriptor from;
        VertexDescriptor to;
        EdgeWeight weight;
        bool has_weight;
    };

private:
    // Внутреннее представление графа
    RepresentationType rep_type;
    GraphType graph_type;

    // Хранение вершин и ребер
    std::unordered_map<VertexDescriptor, VertexInfo> vertices;
    std::unordered_map<EdgeDescriptor, EdgeInfo> edges;

    // Для L-графа (списки смежности)
    std::unordered_map<VertexDescriptor, std::list<EdgeDescriptor>> adjacency_list;

    // Для M-графа (матрица смежности)
    std::vector<std::vector<EdgeDescriptor>> adjacency_matrix;

    // Генерация случайных чисел
    std::random_device rd;
    std::mt19937 gen;

    // Счетчики для дескрипторов
    VertexDescriptor next_vertex_desc;
    EdgeDescriptor next_edge_desc;

public:
    // Конструкторы
    Graph() : rep_type(RepresentationType::LIST), graph_type(GraphType::UNDIRECTED),
              next_vertex_desc(0), next_edge_desc(0), gen(rd()) {}

    Graph(size_t V, GraphType D, RepresentationType F) : graph_type(D), rep_type(F),
        next_vertex_desc(0), next_edge_desc(0), gen(rd()) {
        for (size_t i = 0; i < V; ++i) {
            InsertV();
        }
        if (rep_type == RepresentationType::MATRIX) {
            adjacency_matrix.resize(V, std::vector<EdgeDescriptor>(V, 0));
        }
    }

    Graph(size_t V, size_t E, GraphType D, RepresentationType F) : graph_type(D), rep_type(F),
        next_vertex_desc(0), next_edge_desc(0), gen(rd()) {
        for (size_t i = 0; i < V; ++i) {
            InsertV();
        }

        if (rep_type == RepresentationType::MATRIX) {
            adjacency_matrix.resize(V, std::vector<EdgeDescriptor>(V, 0));
        }

        std::uniform_int_distribution<VertexDescriptor> dist(0, V-1);
        for (size_t i = 0; i < E; ) {
            VertexDescriptor v1 = dist(gen);
            VertexDescriptor v2 = dist(gen);
            if (v1 != v2 && !GetEdge(v1, v2)) {
                InsertE(v1, v2);
                i++;
            }
        }
    }

    // Конструктор копирования
    Graph(const Graph& other) : vertices(other.vertices), edges(other.edges),
        adjacency_list(other.adjacency_list), adjacency_matrix(other.adjacency_matrix),
        rep_type(other.rep_type), graph_type(other.graph_type),
        next_vertex_desc(other.next_vertex_desc), next_edge_desc(other.next_edge_desc),
        gen(rd()) {}

    // Деструктор
    ~Graph() = default;

    // Основные методы
    size_t V() const { return vertices.size(); }
    size_t E() const { return edges.size(); }
    bool Directed() const { return graph_type == GraphType::DIRECTED; }
    bool Dense() const { return rep_type == RepresentationType::MATRIX; }

    const EdgeInfo& GetEdgeInfo(EdgeDescriptor e) const {
        return edges.at(e);
    }

    double K() const {
        size_t max_edges = Directed() ? V() * (V() - 1) : V() * (V() - 1) / 2;
        return max_edges == 0 ? 0.0 : static_cast<double>(E()) / max_edges;
    }

    void ToListGraph() {
        if (rep_type == RepresentationType::LIST) return;

        // Конвертация из матрицы в список
        adjacency_list.clear();
        for (VertexDescriptor v = 0; v < V(); ++v) {
            adjacency_list[v] = std::list<EdgeDescriptor>();
        }

        for (const auto& edge_pair : edges) {
            VertexDescriptor from = edge_pair.second.from;
            VertexDescriptor to = edge_pair.second.to;
            adjacency_list[from].push_back(edge_pair.first);
            if (!Directed()) {
                adjacency_list[to].push_back(edge_pair.first);
            }
        }

        rep_type = RepresentationType::LIST;
        adjacency_matrix.clear();
    }

    void ToMatrixGraph() {
        if (rep_type == RepresentationType::MATRIX) return;

        // Конвертация из списка в матрицу
        adjacency_matrix.resize(V(), std::vector<EdgeDescriptor>(V(), 0));

        for (const auto& edge_pair : edges) {
            VertexDescriptor from = edge_pair.second.from;
            VertexDescriptor to = edge_pair.second.to;
            adjacency_matrix[from][to] = edge_pair.first;
            if (!Directed()) {
                adjacency_matrix[to][from] = edge_pair.first;
            }
        }

        rep_type = RepresentationType::MATRIX;
        adjacency_list.clear();
    }

    VertexDescriptor InsertV() {
        VertexDescriptor desc = next_vertex_desc++;
        vertices[desc] = VertexInfo{"", false};

        if (rep_type == RepresentationType::LIST) {
            adjacency_list[desc] = std::list<EdgeDescriptor>();
        } else {
            // Для матрицы нужно добавить новый столбец и строку
            for (auto& row : adjacency_matrix) {
                row.push_back(0);
            }
            adjacency_matrix.push_back(std::vector<EdgeDescriptor>(V(), 0));
        }

        return desc;
    }

    VertexDescriptor InsertV(const VertexData& name) {
        VertexDescriptor desc = InsertV();
        vertices[desc].data = name;
        vertices[desc].has_name = true;
        return desc;
    }

    void DeleteV(VertexDescriptor v) {
        if (vertices.find(v) == vertices.end()) return;

        // Удаление всех связанных ребер
        if (rep_type == RepresentationType::LIST) {
            // Удаление исходящих ребер
            for (auto it = adjacency_list[v].begin(); it != adjacency_list[v].end(); ) {
                EdgeDescriptor e = *it;
                VertexDescriptor other = (edges[e].from == v) ? edges[e].to : edges[e].from;

                // Удаление ребра из списка смежности другой вершины (для неориентированного)
                if (!Directed()) {
                    adjacency_list[other].remove(e);
                }

                edges.erase(e);
                it = adjacency_list[v].erase(it);
            }

            // Удаление входящих ребер (для ориентированного)
            if (Directed()) {
                for (auto& pair : adjacency_list) {
                    for (auto it = pair.second.begin(); it != pair.second.end(); ) {
                        if (edges[*it].to == v) {
                            edges.erase(*it);
                            it = pair.second.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
            }

            adjacency_list.erase(v);
        } else {
            // Удаление из матрицы
            for (VertexDescriptor i = 0; i < V(); ++i) {
                if (adjacency_matrix[i][v] != 0) {
                    edges.erase(adjacency_matrix[i][v]);
                    adjacency_matrix[i][v] = 0;
                }
                if (adjacency_matrix[v][i] != 0) {
                    edges.erase(adjacency_matrix[v][i]);
                    adjacency_matrix[v][i] = 0;
                }
            }

            // Удаление строки и столбца
            adjacency_matrix.erase(adjacency_matrix.begin() + v);
            for (auto& row : adjacency_matrix) {
                row.erase(row.begin() + v);
            }
        }

        vertices.erase(v);
    }

    EdgeDescriptor InsertE(VertexDescriptor v1, VertexDescriptor v2) {
        return InsertE(v1, v2, EdgeWeight{});
    }

    EdgeDescriptor InsertE(VertexDescriptor v1, VertexDescriptor v2, EdgeWeight w) {
        if (vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end()) {
            throw std::invalid_argument("Vertex not found");
        }

        EdgeDescriptor desc = next_edge_desc++;
        edges[desc] = EdgeInfo{v1, v2, w, true};

        if (rep_type == RepresentationType::LIST) {
            adjacency_list[v1].push_back(desc);
            if (!Directed()) {
                adjacency_list[v2].push_back(desc);
            }
        } else {
            adjacency_matrix[v1][v2] = desc;
            if (!Directed()) {
                adjacency_matrix[v2][v1] = desc;
            }
        }

        return desc;
    }

    void DeleteE(VertexDescriptor v1, VertexDescriptor v2) {
        EdgeDescriptor e = GetEdge(v1, v2);
        if (e == 0) return;

        if (rep_type == RepresentationType::LIST) {
            adjacency_list[v1].remove(e);
            if (!Directed()) {
                adjacency_list[v2].remove(e);
            }
        } else {
            adjacency_matrix[v1][v2] = 0;
            if (!Directed()) {
                adjacency_matrix[v2][v1] = 0;
            }
        }

        edges.erase(e);
    }

    EdgeDescriptor GetEdge(VertexDescriptor v1, VertexDescriptor v2) const {
        if (vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end()) {
            return 0;
        }

        if (rep_type == RepresentationType::LIST) {
            for (const auto& e : adjacency_list.at(v1)) {
                const EdgeInfo& edge = edges.at(e);
                if ((edge.from == v1 && edge.to == v2) ||
                    (!Directed() && edge.from == v2 && edge.to == v1)) {
                    return e;
                }
            }
        } else {
            return adjacency_matrix[v1][v2];
        }

        return 0;
    }

    // Итератор вершин графа
    class VertexIterator {
    private:
        typename std::unordered_map<VertexDescriptor, VertexInfo>::const_iterator it;

    public:
        VertexIterator(typename std::unordered_map<VertexDescriptor, VertexInfo>::const_iterator iterator)
            : it(iterator) {}

        VertexIterator& operator++() {
            ++it;
            return *this;
        }

        VertexDescriptor operator*() const {
            return it->first;
        }

        bool operator!=(const VertexIterator& other) const {
            return it != other.it;
        }
    };

    VertexIterator beg() const {
        return VertexIterator(vertices.begin());
    }

    VertexIterator end() const {
        return VertexIterator(vertices.end());
    }

    // Итератор ребер графа
    class EdgeIterator {
    private:
        typename std::unordered_map<EdgeDescriptor, EdgeInfo>::const_iterator it;

    public:
        EdgeIterator(typename std::unordered_map<EdgeDescriptor, EdgeInfo>::const_iterator iterator)
            : it(iterator) {}

        EdgeIterator& operator++() {
            ++it;
            return *this;
        }

        EdgeDescriptor operator*() const {
            return it->first;
        }

        bool operator!=(const EdgeIterator& other) const {
            return it != other.it;
        }
    };

    EdgeIterator ebeg() const {
        return EdgeIterator(edges.begin());
    }

    EdgeIterator eend() const {
        return EdgeIterator(edges.end());
    }

    // Итератор исходящих ребер вершины
    class OutEdgeIterator {
    private:
        const Graph& graph;
        VertexDescriptor vertex;
        typename std::list<EdgeDescriptor>::const_iterator it;

    public:
        OutEdgeIterator(const Graph& g, VertexDescriptor v,
                       typename std::list<EdgeDescriptor>::const_iterator iterator)
            : graph(g), vertex(v), it(iterator) {}

        OutEdgeIterator& operator++() {
            ++it;
            return *this;
        }

        EdgeDescriptor operator*() const {
            return *it;
        }

        bool operator!=(const OutEdgeIterator& other) const {
            return it != other.it;
        }
    };

    OutEdgeIterator out_beg(VertexDescriptor v) const {
        if (rep_type != RepresentationType::LIST) {
            throw std::logic_error("OutEdgeIterator only available for list representation");
        }
        return OutEdgeIterator(*this, v, adjacency_list.at(v).begin());
    }

    OutEdgeIterator out_end(VertexDescriptor v) const {
        if (rep_type != RepresentationType::LIST) {
            throw std::logic_error("OutEdgeIterator only available for list representation");
        }
        return OutEdgeIterator(*this, v, adjacency_list.at(v).end());
    }
};
