// Файл: EdgeDescriptor.h
#pragma once
#include <memory>
#include <stdexcept>

template<typename VertexDesc, typename WeightType, typename DataType>
class EdgeDescriptor {
    std::shared_ptr<VertexDesc> source;
    std::shared_ptr<VertexDesc> target;
    WeightType weight;
    DataType data;
    bool weighted = false;

public:
    EdgeDescriptor(std::shared_ptr<VertexDesc> v1,
                  std::shared_ptr<VertexDesc> v2)
        : source(v1), target(v2) {}

    EdgeDescriptor(std::shared_ptr<VertexDesc> v1,
                  std::shared_ptr<VertexDesc> v2,
                  WeightType w)
        : source(v1), target(v2), weight(w), weighted(true) {}

    // Доступ к вершинам
    std::shared_ptr<VertexDesc> v1() const noexcept { return source; }
    std::shared_ptr<VertexDesc> v2() const noexcept { return target; }

    // Управление весом
    bool SetW(WeightType w) {
        weight = w;
        weighted = true;

        return true;
    }

    WeightType GetW() const {
        if(!weighted)
            throw std::logic_error("Edge is not weighted");
        return weight;
    }

    // Работа с данными
    bool SetData(const DataType& d) noexcept { data = d; return true; }
    DataType GetData() const noexcept { return data; }

    // Проверки
    bool IsWeighted() const noexcept { return weighted; }
    bool IsValid() const noexcept {
        return source && source->IsValid() && target && target->IsValid();
    }

    std::pair<size_t, size_t> getFromTo() const {
        return std::make_pair(source->GetId(), target->GetId());
    }

    // Операторы сравнения
    bool operator==(const EdgeDescriptor& other) const noexcept {
        return source == other.source && target == other.target;
    }

    bool operator!=(const EdgeDescriptor& other) const noexcept {
        return !(*this == other);
    }

    // Утилиты для графа
    void UpdateVertices(std::shared_ptr<VertexDesc> new_v1,
                      std::shared_ptr<VertexDesc> new_v2) noexcept {
        source = new_v1;
        target = new_v2;
    }
};