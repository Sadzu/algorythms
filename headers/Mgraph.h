// Файл: MGraph.h
#pragma once
#include "GraphStructure.h"
#include <vector>
#include <stdexcept>
#include <memory>

template<typename VertexDesc, typename EdgeDesc>
class MGraph : public GraphStructure<VertexDesc, EdgeDesc> {
    using MatrixRow = std::vector<std::shared_ptr<EdgeDesc>>;
    std::vector<MatrixRow> matrix;
    size_t edge_count = 0;
    bool is_directed;

public:
    explicit MGraph(size_t initial_vertices = 0, bool directed = false)
        : matrix(initial_vertices, MatrixRow(initial_vertices, nullptr)),
          is_directed(directed) {}

    // Конвертация из другого типа графа
    void convertFrom(const GraphStructure<VertexDesc, EdgeDesc>& other) override {
        const size_t num_vertices = other.vertexCount();
        matrix.clear();
        matrix.resize(num_vertices, MatrixRow(num_vertices, nullptr));
        edge_count = 0;
        is_directed = other.isDirected();

        auto edges = other.getEdges();
        for(const auto& edge : edges) {
            size_t i = edge->v1()->getId();
            size_t j = edge->v2()->getId();

            if(matrix[i][j] == nullptr) {
                matrix[i][j] = edge;
                edge_count++;
            }
        }
    }

    // Основные операции
    size_t edgeCount() const override {
        return is_directed ? edge_count : edge_count / 2;
    }

    size_t vertexCount() const override { return matrix.size(); }

    void addVertex() override {
        const size_t new_size = matrix.size() + 1;
        for(auto& row : matrix) row.resize(new_size, nullptr);
        matrix.emplace_back(new_size, nullptr);
    }

    void addEdge(std::shared_ptr<EdgeDesc> edge) override {
        const size_t i = edge->v1()->getId();
        const size_t j = edge->v2()->getId();

        if(i >= matrix.size() || j >= matrix.size()) {
            throw std::out_of_range("Vertex ID out of range");
        }

        if(matrix[i][j] != nullptr) {
            throw std::logic_error("Edge already exists");
        }

        matrix[i][j] = edge;
        edge_count++;

        if(!is_directed && i != j) {
            auto reverse_edge = std::make_shared<EdgeDesc>(*edge);
            reverse_edge->UpdateVertices(edge->v2(), edge->v1());
            matrix[j][i] = reverse_edge;
            edge_count++;
        }
    }

    void removeEdge(std::shared_ptr<EdgeDesc> edge) override {
        const size_t i = edge->v1()->getId();
        const size_t j = edge->v2()->getId();

        if(i >= matrix.size() || j >= matrix.size()) return;

        if(matrix[i][j] != nullptr) {
            matrix[i][j].reset();
            edge_count--;
        }

        if(!is_directed && matrix[j][i] != nullptr) {
            matrix[j][i].reset();
            edge_count--;
        }
    }

    void removeVertex(std::shared_ptr<VertexDesc> vertex) override {
        const size_t id = vertex->getId();
        if(id >= matrix.size()) return;

        // Удаление всех связанных ребер
        for(size_t i = 0; i < matrix.size(); ++i) {
            edge_count -= (matrix[i][id] != nullptr) + (matrix[id][i] != nullptr);
            matrix[i][id].reset();
            matrix[id][i].reset();
        }

        // Удаление строки и столбца
        matrix.erase(matrix.begin() + id);
        for(auto& row : matrix) row.erase(row.begin() + id);

        // Обновление ID вершин
        for(size_t i = id; i < matrix.size(); ++i) {
            for(size_t j = 0; j < matrix.size(); ++j) {
                if(matrix[i][j] != nullptr) {
                    matrix[i][j]->v1()->SetId(i);
                    matrix[i][j]->v2()->SetId(j);
                }
            }
        }
    }

    // Получение информации о графе
    std::vector<std::shared_ptr<EdgeDesc>> getEdges() const override {
        std::vector<std::shared_ptr<EdgeDesc>> edges;
        for(size_t i = 0; i < matrix.size(); ++i) {
            for(size_t j = is_directed ? 0 : i; j < matrix.size(); ++j) {
                if(matrix[i][j] != nullptr) {
                    edges.push_back(matrix[i][j]);
                }
            }
        }
        return edges;
    }

    std::vector<std::shared_ptr<EdgeDesc>> getOutEdges(
        std::shared_ptr<VertexDesc> vertex) const override
    {
        std::vector<std::shared_ptr<EdgeDesc>> result;
        const size_t id = vertex->getId();
        if(id >= matrix.size()) return result;

        for(const auto& edge : matrix[id]) {
            if(edge != nullptr) result.push_back(edge);
        }
        return result;
    }

    bool hasEdge(std::shared_ptr<VertexDesc> v1,
                std::shared_ptr<VertexDesc> v2) const override
    {
        const size_t i = v1->getId();
        const size_t j = v2->getId();
        if(i >= matrix.size() || j >= matrix.size()) return false;
        return matrix[i][j] != nullptr;
    }

    std::shared_ptr<EdgeDesc> getEdge(std::shared_ptr<VertexDesc> v1,
                                    std::shared_ptr<VertexDesc> v2) const override
    {
        const size_t i = v1->getId();
        const size_t j = v2->getId();
        if(i >= matrix.size() || j >= matrix.size()) return nullptr;
        if (matrix[i][j] != nullptr) return matrix[i][j];
        throw std::logic_error("Edge does not exist");
    }

    bool isDirected() const override { return is_directed; }

    // Итераторы
    class Iterator {
        typename MatrixRow::const_iterator current;
        typename MatrixRow::const_iterator end;
        size_t row;
    public:
        Iterator(size_t row, typename MatrixRow::const_iterator begin,
                typename MatrixRow::const_iterator end)
            : row(row), current(begin), end(end) {}

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
        if(vertex_id >= matrix.size()) throw std::out_of_range("Invalid vertex ID");
        return Iterator(vertex_id, matrix[vertex_id].begin(), matrix[vertex_id].end());
    }

    Iterator end(size_t vertex_id) const {
        if(vertex_id >= matrix.size()) throw std::out_of_range("Invalid vertex ID");
        return Iterator(vertex_id, matrix[vertex_id].end(), matrix[vertex_id].end());
    }

    class MGraphOutEdgeIteratorImpl : public GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl {
        using MatrixRow = std::vector<std::shared_ptr<EdgeDesc>>;
        MatrixRow::const_iterator current;
        MatrixRow::const_iterator end;

    public:
        MGraphOutEdgeIteratorImpl(MatrixRow::const_iterator begin,
                                MatrixRow::const_iterator end)
            : current(begin), end(end)
        {
            advance_to_next_valid();
        }

        void next() {
            if(current != end) {
                ++current;
                advance_to_next_valid();
            }
        }

        bool equals(const typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl* other) const {
            const auto* rhs = dynamic_cast<const MGraphOutEdgeIteratorImpl*>(other);
            return rhs && current == rhs->current && end == rhs->end;
        }

        const EdgeDesc& getCurrent() const {
            if(current == end) throw std::out_of_range("Iterator out of range");
            return **current;
        }

        std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl> clone() const override {
            return std::make_unique<MGraphOutEdgeIteratorImpl>(*this);
        }

    private:
        void advance_to_next_valid() {
            while(current != end && !*current) ++current;
        }
    };

    void removeEdge(std::shared_ptr<VertexDesc> v1,
                 std::shared_ptr<VertexDesc> v2) override
    {
        matrix[v1->GetId()][v2->GetId()].reset();
    }

    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl>
outEdgesIterator(std::shared_ptr<VertexDesc> vertex) const override {
        return std::make_unique<MGraphOutEdgeIteratorImpl>(
            matrix[vertex->GetId()].begin(),
            matrix[vertex->GetId()].end()
        );
    }

    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::OutEdgeIteratorImpl>
    outEdgesEndIterator() const override {
        return std::make_unique<MGraphOutEdgeIteratorImpl>(
            typename MatrixRow::const_iterator(),
            typename MatrixRow::const_iterator()
        );
    }

    std::unique_ptr<GraphStructure<VertexDesc, EdgeDesc>> clone() const override {
        auto cloned = std::make_unique<MGraph<VertexDesc, EdgeDesc>>();
        cloned->matrix = this->matrix;
        cloned->edge_count = this->edge_count;
        cloned->is_directed = this->is_directed;
        return cloned;
    }

    void remap_vertices(std::function<std::shared_ptr<VertexDesc>(std::shared_ptr<VertexDesc>)> mapper) override {
        for (auto& row : matrix) {
            for (auto& edge : row) {
                if (edge) {
                    edge->UpdateVertices(mapper(edge->v1()), mapper(edge->v2()));
                }
            }
        }
    }

    class MEdgeIteratorImpl : public GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl {
        typename std::vector<MatrixRow>::const_iterator row_it;
        typename MatrixRow::const_iterator col_it;
        const MGraph* graph;

    public:
        MEdgeIteratorImpl(const MGraph* g, bool end = false)
            : graph(g), row_it(end ? g->matrix.end() : g->matrix.begin())
        {
            if (!end && row_it != g->matrix.end()) {
                col_it = row_it->begin();
                advance_to_valid();
            }
        }

        void next() override {
            if (row_it == graph->matrix.end()) {
                throw std::out_of_range("Incrementing end iterator");
            }
            ++col_it;
            advance_to_valid();
        }

        bool equals(const typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl* other) const override {
            const auto* rhs = dynamic_cast<const MEdgeIteratorImpl*>(other);
            return rhs && row_it == rhs->row_it && col_it == rhs->col_it;
        }

        const EdgeDesc& current() const override {
            if (row_it == graph->matrix.end() || col_it == row_it->end()) {
                throw std::out_of_range("Dereferencing invalid iterator");
            }
            return **col_it;
        }

        bool is_valid() const override {
            return row_it != graph->matrix.end() && col_it != row_it->end();
        }

        std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl> clone() const override {
            return std::make_unique<MEdgeIteratorImpl>(*this);
        }

    private:
        void advance_to_valid() {
            while (row_it != graph->matrix.end()) {
                while (col_it != row_it->end()) {
                    if (*col_it) return;
                    ++col_it;
                }
                ++row_it;
                if (row_it != graph->matrix.end()) {
                    col_it = row_it->begin();
                }
            }
        }
    };

    // Реализация методов для GraphStructure
    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl> edgesIterator() const override {
        return std::make_unique<MEdgeIteratorImpl>(this);
    }

    std::unique_ptr<typename GraphStructure<VertexDesc, EdgeDesc>::EdgeIteratorImpl> edgesEndIterator() const override {
        return std::make_unique<MEdgeIteratorImpl>(nullptr);
    }
};
