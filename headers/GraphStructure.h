// Файл: GraphStructure.h
#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "EdgeDescriptor.h"

template<typename VertexDesc, typename EdgeDesc>
class GraphStructure {
public:
    virtual ~GraphStructure() = default;

    virtual size_t edgeCount() const = 0;
    virtual void addEdge(std::shared_ptr<EdgeDesc> edge) = 0;
    virtual void removeEdge(std::shared_ptr<EdgeDesc> edge) = 0;
    virtual void removeVertex(std::shared_ptr<VertexDesc> vertex) = 0;
    virtual std::vector<std::shared_ptr<EdgeDesc>> getEdges() const = 0;
    virtual std::vector<std::shared_ptr<EdgeDesc>> getOutEdges(
        std::shared_ptr<VertexDesc> vertex) const = 0;
    virtual bool hasEdge(std::shared_ptr<VertexDesc> v1,
                       std::shared_ptr<VertexDesc> v2) const = 0;
    virtual std::shared_ptr<EdgeDesc> getEdge(
        std::shared_ptr<VertexDesc> v1,
        std::shared_ptr<VertexDesc> v2) const = 0;
    virtual void convertFrom(const GraphStructure& other) = 0;
    virtual void addVertex() = 0;
    virtual void removeEdge(std::shared_ptr<VertexDesc> v1,
                          std::shared_ptr<VertexDesc> v2) = 0;
    virtual size_t vertexCount() const = 0;
    virtual bool isDirected() const = 0;

    class OutEdgeIteratorImpl {
    public:
        virtual ~OutEdgeIteratorImpl() = default;
        virtual void next() = 0;
        virtual bool equals(const OutEdgeIteratorImpl* other) const = 0;
        virtual const EdgeDesc& getCurrent() const = 0;
        virtual std::unique_ptr<OutEdgeIteratorImpl> clone() const = 0;
    };

    virtual std::unique_ptr<OutEdgeIteratorImpl> outEdgesIterator(std::shared_ptr<VertexDesc> vertex) const = 0;
    virtual std::unique_ptr<OutEdgeIteratorImpl> outEdgesEndIterator() const = 0;
    virtual std::unique_ptr<GraphStructure> clone() const = 0;
    virtual void remap_vertices(std::function<std::shared_ptr<VertexDesc>(std::shared_ptr<VertexDesc>)> mapper) = 0;
};
