#pragma once
#include "SimpleGraph.h"
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <memory>

template<typename Graph>
class ShortestPathTask {
    using VertexDesc = typename Graph::VertexDesc;
    using EdgeDesc = typename Graph::EdgeDesc;

    const Graph* graph_ptr = nullptr;
    std::vector<std::vector<int>> distance_matrix;
    std::vector<std::vector<int>> predecessor_matrix;
    static constexpr int INF = std::numeric_limits<int>::max();

public:
    explicit ShortestPathTask(const Graph* g) : graph_ptr(g) {
        if(g == nullptr) throw std::invalid_argument("Graph cannot be null");
        Solve();
    }

    void Set(const Graph* g) {
        if(g == nullptr) throw std::invalid_argument("Graph cannot be null");
        graph_ptr = g;
        Solve();
    }

    void Restart() {
        if(graph_ptr == nullptr) throw std::logic_error("Graph not set");
        Solve();
    }

    const std::vector<std::vector<int>>& GetDistances() const {
        return distance_matrix;
    }

    std::vector<int> GetPath(int start_idx, int end_idx) const {
        if(start_idx < 0 || end_idx < 0 ||
           static_cast<size_t>(start_idx) >= distance_matrix.size() ||
           static_cast<size_t>(end_idx) >= distance_matrix.size()) {
            throw std::out_of_range("Invalid vertex index");
        }

        if(distance_matrix[start_idx][end_idx] == -1) {
            return {};
        }

        std::vector<int> path;
        for(int at = end_idx; at != start_idx; at = predecessor_matrix[start_idx][at]) {
            if(at == -1 || predecessor_matrix[start_idx][at] == -1) {
                throw std::runtime_error("Path reconstruction failed");
            }
            path.push_back(at);
        }
        path.push_back(start_idx);

        std::reverse(path.begin(), path.end());
        return path;
    }

private:
    void Solve() {
        if(!graph_ptr->Directed()) {
            throw std::logic_error("This task requires a directed graph");
        }

        const size_t V = graph_ptr->V();
        distance_matrix.assign(V, std::vector<int>(V, INF));
        predecessor_matrix.assign(V, std::vector<int>(V, -1));

        for(size_t i = 0; i < V; ++i) {
            BFS(i);
        }

        for(auto& row : distance_matrix) {
            for(auto& d : row) {
                if(d == INF) d = -1;
            }
        }
    }

    void BFS(size_t start_idx) {
        const size_t num_vertices = graph_ptr->V();
        std::queue<size_t> q;
        q.push(start_idx);
        distance_matrix[start_idx][start_idx] = 0;
        predecessor_matrix[start_idx][start_idx] = static_cast<int>(start_idx);

        while(!q.empty()) {
            size_t u_idx = q.front();
            q.pop();

            try {
                auto u = graph_ptr->vertex_at(u_idx);

                // Получаем диапазон исходящих ребер
                std::vector<EdgeDesc> edges = graph_ptr->out_edges(u);

                for(const auto& edge : edges) {
                    try {
                        size_t v_idx = edge.v2()->GetId();

                        if(v_idx >= num_vertices) {
                            continue;
                        }

                        if(distance_matrix[start_idx][v_idx] == INF) {
                            distance_matrix[start_idx][v_idx] = distance_matrix[start_idx][u_idx] + 1;
                            predecessor_matrix[start_idx][v_idx] = static_cast<int>(u_idx);
                            q.push(v_idx);
                        }
                    } catch(const std::exception& e) {
                        continue;
                    }
                }
            } catch(const std::exception& e) {
                continue;
            }
        }
    }
};