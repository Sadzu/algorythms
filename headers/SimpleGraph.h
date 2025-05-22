// Файл: SimpleGraph.h
#pragma once
#include "VertexDescriptor.h"
#include "EdgeDescriptor.h"
#include "GraphStructure.h"
#include "LGraph.h"
#include "MGraph.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <format>
#include <random>

template<
    typename NameType = std::string,
    typename VertexData = int,
    typename WeightType = float,
    typename EdgeData = std::string
>
class SimpleGraph {
public:
    using VertexDesc = VertexDescriptor<NameType, VertexData>;
    using EdgeDesc = EdgeDescriptor<VertexDesc, WeightType, EdgeData>;
    using Structure = GraphStructure<VertexDesc, EdgeDesc>;

private:
    std::vector<std::shared_ptr<VertexDesc>> vertices;
    std::unique_ptr<Structure> structure;
    std::unordered_map<NameType, std::shared_ptr<VertexDesc>> name_map;
    bool is_directed;

    // Вспомогательные методы
    void validate_vertex(const std::shared_ptr<VertexDesc>& v) const {
        if(!v || v->GetId() >= vertices.size() || vertices[v->GetId()] != v)
            throw std::invalid_argument("Invalid vertex descriptor");
    }

public:
    // Конструкторы
    SimpleGraph() : structure(new LGraph<VertexDesc, EdgeDesc>()), is_directed(false) {}

    SimpleGraph(size_t V, bool directed, bool dense)
    : structure(dense ?
        static_cast<Structure*>(new MGraph<VertexDesc, EdgeDesc>(V, directed)) :
        static_cast<Structure*>(new LGraph<VertexDesc, EdgeDesc>(V, directed)))
{
    vertices.reserve(V);
    is_directed = directed;
    for(size_t i = 0; i < V; ++i) {
        auto v = std::make_shared<VertexDesc>();
        v->SetId(i);
        vertices.push_back(v);
    }
}

    SimpleGraph(const SimpleGraph& other) {
        *this = other;
    }

    const std::vector<std::shared_ptr<VertexDesc>>& getVertices() const {
        return vertices;
    }

    // Оператор присваивания
    SimpleGraph& operator=(const SimpleGraph& other) {
        if(this != &other) {
            is_directed = other.is_directed;
            vertices.clear();
            name_map.clear();

            // Копируем вершины
            for(const auto& v : other.vertices) {
                auto new_v = std::make_shared<VertexDesc>(*v);
                vertices.push_back(new_v);
                if(v->IsNamed())
                    name_map[v->GetName()] = new_v;
            }

            // Копируем структуру
            structure = other.structure->clone();

            // Обновляем указатели в структуре
            structure->remap_vertices([this](auto old_v) {
                return vertices[old_v->GetId()];
            });
        }
        return *this;
    }

    // Основные методы
    size_t V() const { return vertices.size(); }
    size_t E() const { return structure->edgeCount(); }
    bool Directed() const { return is_directed; }
    bool Dense() const { return dynamic_cast<MGraph<VertexDesc, EdgeDesc>*>(structure.get()); }

    float K() const {
        size_t max_edges = is_directed ?
            V() * (V() - 1) :
            V() * (V() - 1) / 2;
        return max_edges > 0 ? static_cast<float>(E()) / max_edges : 0.0f;
    }

    std::shared_ptr<VertexDesc> vertex_at(size_t index) {
        if(index >= vertices.size()) {
            throw std::out_of_range("Vertex index out of range");
        }
        return vertices[index];
    }

    void ToListGraph() {
        if(Dense()) {
            auto new_structure = std::make_unique<LGraph<VertexDesc, EdgeDesc>>();
            new_structure->convertFrom(*structure);
            structure = std::move(new_structure);
        }
    }

    void ToMatrixGraph() {
        if(!Dense()) {
            auto new_structure = std::make_unique<MGraph<VertexDesc, EdgeDesc>>();
            new_structure->convertFrom(*structure);
            structure = std::move(new_structure);
        }
    }

    std::shared_ptr<VertexDesc> InsertV() {
        auto v = std::make_shared<VertexDesc>();
        v->SetId(vertices.size());
        vertices.push_back(v);
        structure->addVertex();
        return v;
    }

    std::shared_ptr<VertexDesc> InsertV(const NameType& name) {
        if(name_map.count(name))
            throw std::invalid_argument("Vertex name already exists");

        auto v = InsertV();
        v->SetName(name);
        name_map[name] = v;
        return v;
    }

    std::shared_ptr<VertexDesc> InsertV(const NameType& name, const VertexData& data) {
        if(name_map.count(name))
            throw std::invalid_argument("Vertex name already exists");

        auto v = InsertV();
        v->SetName(name);
        v->SetData(data);
        name_map[name] = v;
        return v;
    }

    bool DeleteV(std::shared_ptr<VertexDesc> v) {
        try {
            validate_vertex(v);
        } catch (const std::invalid_argument& e) {
            return false;
        }

        const size_t deleted_id = v->getId();

        // 1. Удаление из name_map
        if (v->IsNamed()) {
            name_map.erase(v->GetName());
        }

        // 2. Удаление вершины из вектора
        vertices.erase(vertices.begin() + deleted_id);

        std::cout << "DeleteV" << std::endl;
        for (auto v : vertices) {
            std::cout << v->getId() << " ";
        }
        std::cout << std::endl;
        // 3. Обновление ID в оставшихся вершинах
        for (size_t i = deleted_id; i < vertices.size(); ++i) {
            vertices[i]->SetId(i); // Важно: обновление до вызова removeVertex!
        }
        std::cout << "DeleteV" << std::endl;
        for (auto v : vertices) {
            std::cout << v->getId() << " ";
        }
        std::cout << std::endl;

        // 4. Удаление из структуры графа
        structure->removeVertex(v);
        std::cout << "DeleteV" << std::endl;
        for (auto v : vertices) {
            std::cout << v->getId() << " ";
        }
        std::cout << std::endl;

        // 5. Обновление name_map для именованных вершин
        for (size_t i = deleted_id; i < vertices.size(); ++i) {
            // if (vertices[i]->IsNamed()) {
            //     name_map[vertices[i]->GetName()] =
        }

        return true;
    }

    std::shared_ptr<EdgeDesc> InsertE(std::shared_ptr<VertexDesc> v1,
                                    std::shared_ptr<VertexDesc> v2)
    {
        validate_vertex(v1);
        validate_vertex(v2);

        auto edge = std::make_shared<EdgeDesc>(v1, v2);
        structure->addEdge(edge);
        return edge;
    }

    std::shared_ptr<EdgeDesc> InsertE(std::shared_ptr<VertexDesc> v1,
                                    std::shared_ptr<VertexDesc> v2,
                                    WeightType weight)
    {
        auto edge = InsertE(v1, v2);
        edge->SetW(weight);
        return edge;
    }

    bool DeleteE(std::shared_ptr<VertexDesc> v1,
            std::shared_ptr<VertexDesc> v2)
    {
        validate_vertex(v1);
        validate_vertex(v2);
        return structure->removeEdge(v1, v2); // Теперь передаем 2 вершины
    }

    std::shared_ptr<EdgeDesc> GetEdge(std::shared_ptr<VertexDesc> v1,
                                    std::shared_ptr<VertexDesc> v2) const
    {
        validate_vertex(v1);
        validate_vertex(v2);
        return structure->getEdge(v1, v2);
    }

    // Итераторы
    class VertexIterator {
        typename std::vector<std::shared_ptr<VertexDesc>>::const_iterator current;
        typename std::vector<std::shared_ptr<VertexDesc>>::const_iterator end;

    public:
        VertexIterator(decltype(current) begin, decltype(current) end)
            : current(begin), end(end) {}

        // Безопасное разыменование с проверкой
        const VertexDesc& operator*() const {
            if (current == end) {
                throw std::out_of_range("Dereferencing end iterator");
            }
            return **current;
        }

        // Проверка перед инкрементом
        VertexIterator& operator++() {
            if (current == end) {
                throw std::out_of_range("Incrementing end iterator");
            }
            ++current;
            return *this;
        }

        bool operator!=(const VertexIterator& other) const {
            return current != other.current;
        }

        // Дополнительный метод для проверки валидности
        bool is_valid() const {
            return current != end;
        }
    };

    class EdgeIterator {
        std::unique_ptr<typename Structure::EdgeIteratorImpl> impl;

    public:
        EdgeIterator(std::unique_ptr<typename Structure::EdgeIteratorImpl> it)
            : impl(std::move(it)) {}

        // Безопасное разыменование
        const EdgeDesc& operator*() const {
            return impl->current(); // Будет брошено исключение если невалидно
        }

        // Доступ к членам
        const EdgeDesc* operator->() const {
            return &impl->current();
        }

        // Префиксный инкремент
        EdgeIterator& operator++() {
            impl->next();
            return *this;
        }

        // Постфиксный инкремент
        EdgeIterator operator++(int) {
            EdgeIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        // Сравнение
        bool operator!=(const EdgeIterator& other) const {
            // Оба итератора end
            if (!impl && !other.impl) return false;

            // Один из итераторов end
            if (!impl || !other.impl) return true;

            // Сравнение реализаций
            return !impl->equals(other.impl.get());
        }

        // Явная проверка валидности
        bool is_valid() const {
            return impl->is_valid();
        }

        // Безопасное получение ребра
        bool try_get(EdgeDesc& result) const {
            if (is_valid()) {
                result = **this;
                return true;
            }
            return false;
        }
    };

public:
    void GenerateDirectedGraph(size_t vertex_count, size_t edge_count, float min_weight = 1.0f, float max_weight = 10.0f) {
        // Очистка текущего графа
        structure = std::make_unique<LGraph<VertexDesc, EdgeDesc>>(0, true);
        vertices.clear();
        name_map.clear();

        // Создание вершин
        for(size_t i = 0; i < vertex_count; ++i) {
            std::cout << structure->vertexCount() << " vertices, " << structure->edgeCount() << " edges\n";
            InsertV("name " + std::to_string(i), i);
        }

        // Генерация случайных рёбер
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> vertex_dist(0, vertex_count - 1);
        std::uniform_real_distribution<float> weight_dist(min_weight, max_weight);

        size_t created_edges = 0;
        while(created_edges < edge_count) {
            size_t src = vertex_dist(gen);
            size_t dest = vertex_dist(gen);

            // Исключаем петли и дубликаты
            if(src == dest) continue;
            if(structure->hasEdge(vertices[src], vertices[dest])) continue;

            try {
                auto e = InsertE(vertices[src], vertices[dest], weight_dist(gen));
                created_edges++;
                e->SetData("data " + std::to_string(created_edges));
            } catch(const std::exception&) {
                // Игнорируем ошибки (например, при дубликатах)
            }
        }
    }

    std::vector<EdgeDesc> out_edges(const std::shared_ptr<VertexDesc>& v) const {
        std::vector<EdgeDesc> result;
        for(const auto& e : structure->getOutEdges(v)) {
            if(e->v1() == v) {
                result.push_back(*e);
            }
        }
        return result;
    }

    class OutEdgeIterator {
        std::unique_ptr<typename Structure::OutEdgeIteratorImpl> impl;
    public:
        OutEdgeIterator(std::unique_ptr<typename Structure::OutEdgeIteratorImpl> it)
            : impl(std::move(it)) {}

        OutEdgeIterator& operator++() { impl->next(); return *this; }
        bool operator!=(const OutEdgeIterator& other) const { return !impl->equals(other.impl.get()); }
        const EdgeDesc& operator*() const { return impl->getCurrent(); }
    };

    // Методы итераторов
    VertexIterator vertex_begin() const { return VertexIterator(vertices.begin(), vertices.end()); }
    VertexIterator vertex_end() const { return VertexIterator(vertices.end(), vertices.end()); }

    EdgeIterator edges_begin() const {
        return EdgeIterator(structure->edgesIterator());
    }

    EdgeIterator edges_end() const {
        return EdgeIterator(nullptr);
    }

    OutEdgeIterator out_edges_begin(std::shared_ptr<VertexDesc> v) const {
        validate_vertex(v);
        return OutEdgeIterator(structure->outEdgesIterator(v));
    }

    OutEdgeIterator out_edges_end(std::shared_ptr<VertexDesc> v) const {
        validate_vertex(v);
        return OutEdgeIterator(structure->outEdgesEndIterator());
    }

    // Дополнительные методы
    std::shared_ptr<VertexDesc> FindByName(const NameType& name) const {
        auto it = name_map.find(name);
        return it != name_map.end() ? it->second : nullptr;
    }

public:
    void print() const {
        std::cout << "\n=== Graph ===" << std::endl;
        std::cout << "Vertexes: " << V() << ", Edges: " << E() << std::endl;
        std::cout << "Type: " << (Directed() ? "Oriented" : "Unoriented") << std::endl;
        std::cout << "Form: " << (Dense() ? "Adjust matrix" : "Adjust list") << "\n" << std::endl;

        // Вывод вершин
        std::cout << "Vertexes:" << std::endl;
        for (const auto& v : getVertices()) {
            std::cout << "[ID: " << v->GetId();
            if (v->IsNamed()) {
                std::cout << " Name: '" << v->GetName() << "'";
            }
            // if (v->GetData()) {
            //     std::cout << " Data: " << v->GetData();
            // }
            std::cout << "]" << std::endl;
        }

        // Вывод рёбер
        std::cout << "\nEdges:" << std::endl;
        if (Dense()) {
            printMatrix();
        } else {
            printAdjList();
        }
    }

    const std::shared_ptr<VertexDesc>& vertex_at(size_t index) const {
        if(index >= vertices.size()) {
            throw std::out_of_range("Vertex index out of range");
        }
        return vertices[index];
    }

    std::shared_ptr<EdgeDesc> GetEdge(std::shared_ptr<VertexDesc>& v1, std::shared_ptr<VertexDesc>& v2) const {
        return structure->GetEdge(v1, v2);
    }

private:
    void printAdjList() const {
        for (const auto& v : getVertices()) {
            std::cout << v->GetId() << " -> ";
            bool first = true;
            for (auto it = out_edges_begin(v); it != out_edges_end(v); ++it) {
                if (!first) std::cout << ", ";
                try {
                    const auto& edge = *it;
                    std::cout << edge.v2()->GetId();
                    // if (edge.IsWeighted()) {
                    //     std::cout << " (w=" << edge.GetW() << ")";
                    // }
                    first = false;
                } catch(const std::exception&) {
                    break;
                }
            }
            std::cout << std::endl;
        }
    }

    void printMatrix() const {
        const size_t n = V();
        // Шапка таблицы
        std::cout << "     ";
        for (size_t i = 0; i < n; ++i) std::cout << i << "  ";
        std::cout << "\n    ";
        for (size_t i = 0; i < n; ++i) std::cout << "-----";
        std::cout << std::endl;

        // Тело таблицы
        for (size_t i = 0; i < n; ++i) {
            std::cout << i << " | ";
            for (size_t j = 0; j < n; ++j) {
                try {
                    auto edge = GetEdge(getVertices()[i], getVertices()[j]);
                    if (edge) {
                        if (edge->IsWeighted()) {
                            std::cout << " " << 1 << " ";
                        } else {
                            std::cout << " X ";
                        }
                    } else {
                        std::cout << " 0 ";
                    }
                } catch(const std::exception&) {
                    std::cout << " 0 ";
                }
            }
            std::cout << std::endl;
        }
    }

    Structure getStructure() {
        return structure;
    }
};


























// пасхалка момент
//       ╭┬───┬╮
//      ╭╯│   │╰╮
//     ╭╯ │   │ ╰╮
//    ╱   │   │   ╲
//   ╱    ░▄████▄░  ╲
//  │    ██████████  │▒
//  │    ██■■■■■■██  │▒
//  │    ██▒▒▒▒▒▒██  │▓
//   ╲  ██▒▒▒▒▒▒██  ╱▓
//    ╲  ░▀████▀░  ╱▓
//     ╲_    ░   _╱ ▓
//       │   ░▒▓│ ▓
//      ╭┴───┬──┴╮
//    _(   ╭⊙┬⊙╮  )_
//  (    \│▒▓▓│/     )
//   \____(╲_╱╱)____/
//      │║║│║║│
//      │║║│║║│
//      ╘╧╧│╧╧╛
//        ░▒▓██
//       «HARD»
