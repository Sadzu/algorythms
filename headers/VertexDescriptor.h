// Файл: VertexDescriptor.h
#pragma once
#include <memory>
#include <stdexcept>

template<typename NameType, typename DataType>
class VertexDescriptor {
private:
    NameType name;
    DataType data;
    bool name_defined = false;
    size_t id;

public:

    size_t getId() const {
        return id;
    }

    VertexDescriptor() : id(static_cast<size_t>(-1)) {}
    explicit VertexDescriptor(const NameType& name)
        : name(name), name_defined(true), id(static_cast<size_t>(-1)) {}

    // Управление идентификатором
    void SetId(size_t new_id) noexcept { id = new_id; }
    size_t GetId() const noexcept { return id; }

    // Работа с именем
    bool SetName (const NameType& new_name) {
        name = new_name;
        name_defined = true;
        return true;
    }

    NameType GetName() const {
        if(!name_defined)
            throw std::logic_error("Vertex name not defined");
        return name;
    }

    // Работа с данными
    bool SetData(const DataType& new_data) { data = new_data; return true; }
    DataType GetData() const noexcept { return data; }

    // Проверки
    bool IsNamed() const noexcept { return name_defined; }
    bool IsValid() const noexcept { return id != static_cast<size_t>(-1); }

    // Операторы сравнения
    bool operator==(const VertexDescriptor& other) const noexcept {
        return id == other.id;
    }

    bool operator!=(const VertexDescriptor& other) const noexcept {
        return !(*this == other);
    }
};