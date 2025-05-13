#pragma once
#include "VertexDescriptor.h"
#include "EdgeDescriptor.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <list>
#include <stdexcept>

template <typename TName = std::string,
          typename TVertexData = int,
          typename TWeight = int,
          typename TEdgeData = int>
class Graph {
public:
    using VertexDesc = VertexDescriptor<TName, TVertexData>;
    using EdgeDesc = EdgeDescriptor<VertexDesc, TWeight, TEdgeData>;

    enum class Direction { DIRECTED, UNDIRECTED };
    enum class RepType { LIST, MATRIX };

private:
    Direction direction;
    RepType repType;
    std::vector<VertexDesc> vertices;
    std::vector<EdgeDesc> edges;
    std::unordered_map<TName, size_t> nameMap;
    size_t nextVertexId = 0;
    size_t nextEdgeId = 0;

public:
    Graph(Direction dir = Direction::UNDIRECTED, RepType rt = RepType::LIST)
        : direction(dir), repType(rt) {}

    VertexDesc InsertV(const TName& name, TVertexData data = TVertexData{}) {
        VertexDesc v(nextVertexId++, name, data);
        vertices.push_back(v);
        nameMap[name] = v.getId();
        return v;
    }

    EdgeDesc InsertE(const VertexDesc& from, const VertexDesc& to, TWeight weight = TWeight{}) {
        if(from.getId() >= vertices.size() || to.getId() >= vertices.size()) {
            throw std::out_of_range("Invalid vertex descriptor");
        }

        EdgeDesc e(nextEdgeId++, from, to, weight);
        edges.push_back(e);
        return e;
    }

    void DeleteV(const VertexDesc& v) {
        auto it = std::find(vertices.begin(), vertices.end(), v);
        if(it != vertices.end()) {
            vertices.erase(it);
            nameMap.erase(v.getName());
        }
    }

    void DeleteE(const EdgeDesc& e) {
        auto it = std::find(edges.begin(), edges.end(), e);
        if(it != edges.end()) {
            edges.erase(it);
        }
    }

    size_t V() const { return vertices.size(); }
    size_t E() const { return edges.size(); }

    const std::vector<VertexDesc>& getVertices() const { return vertices; }
    const std::vector<EdgeDesc>& getEdges() const { return edges; }

    VertexDesc getVertex(const TName& name) const {
        auto it = nameMap.find(name);
        if(it == nameMap.end()) {
            throw std::runtime_error("Vertex not found");
        }
        return vertices[it->second];
    }

    class VertexIterator {
        typename std::vector<VertexDesc>::const_iterator it;
    public:
        VertexIterator(typename std::vector<VertexDesc>::const_iterator iter) : it(iter) {}

        VertexIterator& operator++() { ++it; return *this; }
        const VertexDesc& operator*() const { return *it; }
        bool operator!=(const VertexIterator& other) const { return it != other.it; }
    };

    VertexIterator begin() const { return VertexIterator(vertices.begin()); }
    VertexIterator end() const { return VertexIterator(vertices.end()); }
};