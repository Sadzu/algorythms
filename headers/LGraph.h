// Файл: LGraph.h
#pragma once
#include <functional>

#include "GraphStructure.h"
#include <list>
#include <stdexcept>

template<typename VertexDesc, typename EdgeDesc>
class LGraph : public GraphStructure<VertexDesc, EdgeDesc> {
    using EdgeList = std::list<std::shared_ptr<EdgeDesc>>;
    std::vector<EdgeList> adjacency;
    size_t edge_count = 0;
    bool is_directed;

public:
    explicit LGraph(size_t initial_vertices = 0, bool directed = false)
        : adjacency(initial_vertices), is_directed(directed) {}

    // Конвертация из другого типа графа
    void convertFrom(const GraphStructure<VertexDesc, EdgeDesc>& other) override {
        adjacency.clear();
        adjacency.resize(other.vertexCount());
        edge_count = 0;
        is_directed = other.isDirected();

        auto all_edges = other.getEdges();
        for(const auto& edge : all_edges) {
            addEdge(edge);
        }
    }

    // Основные операции
    size_t edgeCount() const override { return edge_count; }

    size_t vertexCount() const override { return adjacency.size(); }

    void addVertex() override {
        adjacency.emplace_back();
    }

    void addEdge(std::shared_ptr<EdgeDesc> edge) override {
        const size_t src_id = edge->v1()->getId();
        const size_t dest_id = edge->v2()->getId();

        if(src_id >= adjacency.size() || dest_id >= adjacency.size()) {
            throw std::out_of_range("Vertex ID out of range");
        }

        // Проверка существования ребра
        for(const auto& e : adjacency[src_id]) {
            if(e->v2()->getId() == dest_id) {
                throw std::logic_error("Edge already exists");
            }
        }

        adjacency[src_id].push_back(edge);
        edge_count++;

        // Добавление обратного ребра для неориентированного графа
        if(!is_directed) {
            auto reverse_edge = std::make_shared<EdgeDesc>(*edge);
            reverse_edge->UpdateVertices(edge->v2(), edge->v1());
            adjacency[dest_id].push_back(reverse_edge);
            edge_count++;
        }
    }

    void removeEdge(std::shared_ptr<EdgeDesc> edge) override {
        const size_t src_id = edge->v1()->getId();
        const size_t dest_id = edge->v2()->getId();

        if(src_id >= adjacency.size()) return;

        // Удаление прямого ребра
        auto& edges = adjacency[src_id];
        for(auto it = edges.begin(); it != edges.end(); ++it) {
            if((*it)->v2()->getId() == dest_id) {
                edges.erase(it);
                edge_count--;
                break;
            }
        }

        // Удаление обратного ребра для неориентированного
        if(!is_directed) {
            auto& reverse_edges = adjacency[dest_id];
            for(auto it = reverse_edges.begin(); it != reverse_edges.end(); ++it) {
                if((*it)->v2()->getId() == src_id) {
                    reverse_edges.erase(it);
                    edge_count--;
                    break;
                }
            }
        }
    }



    void removeVertex(std::shared_ptr<VertexDesc> vertex) override {
        const size_t id = vertex->getId();
        if(id >= adjacency.size()) return;

        // Удаление всех связанных ребер
        edge_count -= adjacency[id].size();
        adjacency[id].clear();

        // Удаление ссылок на вершину в других списках
        for(auto& list : adjacency) {
            list.remove_if([id](const auto& edge) {
                return edge->v2()->getId() == id;
            });
        }

        // Сдвиг идентификаторов вершин после удаленной
        adjacency.erase(adjacency.begin() + id);
        for(auto& list : adjacency) {
            for(auto& edge : list) {
                if(edge->v1()->getId() > id) edge->v1()->SetId(edge->v1()->getId() - 1);
                if(edge->v2()->getId() > id) edge->v2()->SetId(edge->v2()->getId() - 1);
            }
        }
    }

    // Получение информации о графе
    std::vector<std::shared_ptr<EdgeDesc>> getEdges() const override {
        std::vector<std::shared_ptr<EdgeDesc>> edges;
        for(const auto& list : adjacency) {
            for(const auto& edge : list) {
                edges.push_back(edge);
            }
        }
        return edges;
    }

    std::vector<std::shared_ptr<EdgeDesc>> getOutEdges(
        std::shared_ptr<VertexDesc> vertex) const override
    {
        std::vector<std::shared_ptr<EdgeDesc>> result;
        if(vertex->getId() >= adjacency.size()) return result;

        for(const auto& edge : adjacency[vertex->getId()]) {
            result.push_back(edge);
        }
        return result;
    }

    bool hasEdge(std::shared_ptr<VertexDesc> v1,
                std::shared_ptr<VertexDesc> v2) const override
    {
        if(v1->getId() >= adjacency.size()) return false;

        for(const auto& edge : adjacency[v1->getId()]) {
            if(edge->v2()->getId() == v2->getId()) return true;
        }
        return false;
    }

    std::shared_ptr<EdgeDesc> getEdge(std::shared_ptr<VertexDesc> v1,
                                    std::shared_ptr<VertexDesc> v2) const override
    {
        if(v1->getId() >= adjacency.size()) return nullptr;

        for(const auto& edge : adjacency[v1->getId()]) {
            if(edge->v2()->getId() == v2->getId()) return edge;
        }
        throw std::logic_error("Edge does not exist");
    }

    bool isDirected() const override { return is_directed; }

    // Итераторы
    class Iterator {
        typename EdgeList::const_iterator current;
        typename EdgeList::const_iterator end;
    public:
        Iterator(typename EdgeList::const_iterator begin,
                typename EdgeList::const_iterator end)
            : current(begin), end(end) {}

        Iterator& operator++() {
            ++current;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        const EdgeDesc& operator*() const { return **current; }
    };

    Iterator begin(size_t vertex_id) const {
        if(vertex_id >= adjacency.size()) throw std::out_of_range("Invalid vertex ID");
        return Iterator(adjacency[vertex_id].begin(), adjacency[vertex_id].end());
    }

    Iterator end(size_t vertex_id) const {
        if(vertex_id >= adjacency.size()) throw std::out_of_range("Invalid vertex ID");
        return Iterator(adjacency[vertex_id].end(), adjacency[vertex_id].end());
    }

    class LGraphOutEdgeIteratorImpl : public GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl {
        using EdgeList = std::list<std::shared_ptr<EdgeDesc>>;
        EdgeList::const_iterator current;
        EdgeList::const_iterator end;

    public:
        LGraphOutEdgeIteratorImpl(EdgeList::const_iterator begin,
                                EdgeList::const_iterator end)
            : current(begin), end(end) {}

        void next() override {
            if(current != end) ++current;
        }

        bool equals(const typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl* other) const override {
            const auto* rhs = dynamic_cast<const LGraphOutEdgeIteratorImpl*>(other);
            return rhs && current == rhs->current && end == rhs->end;
        }

        const EdgeDesc& getCurrent() const override {
            if(current == end) throw std::out_of_range("Iterator out of range");
            return **current;
        }

        std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl> clone() const {
            return std::make_unique<LGraphOutEdgeIteratorImpl>(*this);
        }
    };

    bool removeEdge(std::shared_ptr<VertexDesc> v1,
                  std::shared_ptr<VertexDesc> v2) override
    {
        auto& edges = adjacency[v1->GetId()];
        if (edges.empty()) return false;
        edges.remove_if([&](const auto& edge) {
            return edge->v2() == v2;
        });

        return true;
    }

    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl> outEdgesIterator(std::shared_ptr<VertexDesc> v) const override {
        size_t id = v->GetId();
        if (id >= adjacency.size()) throw std::out_of_range("Vertex ID out of range");
        return std::make_unique<LGraphOutEdgeIteratorImpl>(adjacency[id].begin(), adjacency[id].end());
    }

    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl>
    outEdgesEndIterator() const override {
        return std::make_unique<LGraphOutEdgeIteratorImpl>(
            typename EdgeList::const_iterator(),
            typename EdgeList::const_iterator()
        );
    }

    std::unique_ptr<GraphStructure<VertexDesc, EdgeDesc>> clone() const override {
        auto cloned = std::make_unique<LGraph<VertexDesc, EdgeDesc>>();
        cloned->adjacency = this->adjacency;
        cloned->edge_count = this->edge_count;
        cloned->is_directed = this->is_directed;
        return cloned;
    }

    void remap_vertices(std::function<std::shared_ptr<VertexDesc>(std::shared_ptr<VertexDesc>)> mapper) override {
        for (auto& edge_list : adjacency) {
            for (auto& edge : edge_list) {
                edge->UpdateVertices(mapper(edge->v1()), mapper(edge->v2()));
            }
        }
    }

    class LEdgeIteratorImpl : public GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl {
        typename std::vector<EdgeList>::const_iterator outer;
        typename EdgeList::const_iterator inner;
        const LGraph* graph;

    public:
        LEdgeIteratorImpl(const LGraph* g, bool end = false)
            : graph(g), outer(end ? g->adjacency.end() : g->adjacency.begin())
        {
            if (!end && outer != g->adjacency.end()) {
                inner = outer->begin();
                advance_to_valid();
            }
        }

        void next() override {
            if (outer == graph->adjacency.end()) {
                throw std::out_of_range("Incrementing end iterator");
            }
            ++inner;
            advance_to_valid();
        }

        bool equals(const typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl* other) const override {
            const auto* rhs = dynamic_cast<const LEdgeIteratorImpl*>(other);
            return rhs && outer == rhs->outer && inner == rhs->inner;
        }

        const EdgeDesc& current() const override {
            if (outer == graph->adjacency.end() || inner == outer->end()) {
                throw std::out_of_range("Dereferencing invalid iterator");
            }
            return **inner;
        }

        bool is_valid() const override {
            return outer != graph->adjacency.end() && inner != outer->end();
        }

        std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl> clone() const override {
            return std::make_unique<LEdgeIteratorImpl>(*this);
        }

    private:
        void advance_to_valid() {
            while (outer != graph->adjacency.end()) {
                if (inner != outer->end()) return;
                ++outer;
                if (outer != graph->adjacency.end()) {
                    inner = outer->begin();
                }
            }
        }
    };

    // Реализация методов для GraphStructure
    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl> edgesIterator() const override {
        return std::make_unique<LEdgeIteratorImpl>(this);
    }

    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl> edgesEndIterator() const override {
        return std::make_unique<LEdgeIteratorImpl>(nullptr);
    }
};
