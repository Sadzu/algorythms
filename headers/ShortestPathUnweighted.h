#pragma once

#include <queue>
#include <unordered_map>

template <typename VertexData = std::string>
class ShortestPathsUnweighted {
private:
    using Base = Graph<VertexData, int>;
    using VertexDescriptor = typename Base::VertexDescriptor;
    using EdgeDescriptor = typename Base::EdgeDescriptor;

    std::unordered_map<VertexDescriptor,
                      std::unordered_map<VertexDescriptor, int>> distances;
    Base* graph;

public:
    ShortestPathsUnweighted(Base& g) : graph(&g) {
        solve();
    }

    ShortestPathsUnweighted(const ShortestPathsUnweighted& other)
        : distances(other.distances), graph(other.graph) {}

    ~ShortestPathsUnweighted() = default;

    void Set(Base& g) {
        graph = &g;
        solve();
    }

    void Restart() {
        if (graph) solve();
    }

    const auto& Result() const {
        return distances;
    }

    int GetDistance(VertexDescriptor from, VertexDescriptor to) const {
        if (distances.count(from) && distances.at(from).count(to)) {
            return distances.at(from).at(to);
        }
        return -1;
    }

private:
    void solve() {
        distances.clear();

        if (!graph || graph->V() == 0) return;

        for (auto it = graph->beg(); it != graph->end(); ++it) {
            VertexDescriptor start = *it;
            std::unordered_map<VertexDescriptor, int> local_dist;
            std::queue<VertexDescriptor> q;

            for (auto vit = graph->beg(); vit != graph->end(); ++vit) {
                local_dist[*vit] = -1;
            }
            local_dist[start] = 0;
            q.push(start);

            while (!q.empty()) {
                VertexDescriptor current = q.front();
                q.pop();

                // Получаем информацию о ребрах через граф
                for (auto eit = graph->out_beg(current); eit != graph->out_end(current); ++eit) {
                    EdgeDescriptor edge = *eit;
                    auto edge_info = graph->GetEdgeInfo(edge); // Нужно добавить этот метод в Graph

                    VertexDescriptor neighbor = (graph->Directed()) ?
                        edge_info.to :
                        (edge_info.from == current ? edge_info.to : edge_info.from);

                    if (local_dist[neighbor] == -1) {
                        local_dist[neighbor] = local_dist[current] + 1;
                        q.push(neighbor);
                    }
                }
            }

            distances[start] = local_dist;
        }
    }
};