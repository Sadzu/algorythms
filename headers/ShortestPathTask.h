#pragma once
#include "SimpleGraph.h"
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <string>

template<typename Graph>
class ShortestPathTask {
    using VertexDesc = typename Graph::VertexDesc;
    using EdgeDesc = typename Graph::EdgeDesc;

    const Graph* graph_ptr = nullptr;
    std::vector<std::vector<int>> distance_matrix;
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

    const std::vector<std::vector<int>>& Result() const {
        return distance_matrix;
    }

private:
    void Solve() {
        if(!graph_ptr->Directed()) {
            throw std::logic_error("This task requires a directed graph");
        }

        const size_t V = graph_ptr->V();
        distance_matrix.clear();
        distance_matrix.resize(V, std::vector<int>(V, INF));

        for(size_t i = 0; i < V; ++i) {
            BFS(i, distance_matrix[i]);
        }
    }

    void BFS(size_t start_idx, std::vector<int>& distances) {
        const size_t num_vertices = graph_ptr->V();
        std::queue<size_t> q;
        q.push(start_idx);
        distances[start_idx] = 0;

        while(!q.empty()) {
            size_t u_idx = q.front();
            q.pop();

            auto u = graph_ptr->vertex_at(u_idx);
            for(auto it = graph_ptr->out_edges_begin(u); it != graph_ptr->out_edges_end(u); ++it) {
                try {
                    const auto& edge = *it;
                    size_t v_idx;
                    try {
                        v_idx = edge.v2()->GetId();
                    } catch(const std::exception& e) {
                        std::cerr << "Error accessing vertex ID: " << e.what() << std::endl;
                        continue;
                    }

                    if (v_idx >= num_vertices) {
                        std::cerr << "Warning: Invalid vertex ID " << v_idx
                                  << " detected in edge. Skipping." << std::endl;
                        continue;
                    }

                    if (distances[v_idx] == INF) {
                        distances[v_idx] = distances[u_idx] + 1;
                        q.push(v_idx);
                    }
                } catch (std::out_of_range &e) {
                    break;
                }

            }
        }

        // Замена INF на -1 для недостижимых вершин
        for(auto& d : distances) {
            if(d == INF) d = -1;
        }
    }
};