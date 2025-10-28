#pragma once
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class SeqContainer {
private:
    T* data;
    size_t m_size;
    size_t m_capacity;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < m_size; ++i)
            new_data[i] = std::move(data[i]);
        delete[] data;
        data = new_data;
        m_capacity = new_capacity;
    }

public:
    SeqContainer() : data(nullptr), m_size(0), m_capacity(0) {}
    ~SeqContainer() { delete[] data; }

    SeqContainer(SeqContainer&& other) noexcept
        : data(other.data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    SeqContainer& operator=(SeqContainer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (m_size >= m_capacity) {
            size_t new_capacity = (m_capacity == 0) ? 2 : static_cast<size_t>(m_capacity * 1.5);
            reallocate(new_capacity);
        }
        data[m_size++] = value;
    }

    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("Invalid index");
        if (m_size >= m_capacity) reallocate((m_capacity == 0) ? 2 : static_cast<size_t>(m_capacity * 1.5));
        for (size_t i = m_size; i > pos; --i)
            data[i] = std::move(data[i - 1]);
        data[pos] = value;
        ++m_size;
    }

    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("Invalid index");
        for (size_t i = pos; i < m_size - 1; ++i)
            data[i] = std::move(data[i + 1]);
        --m_size;
    }

    size_t size() const { return m_size; }

    T& operator[](size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    void print() const {
        for (size_t i = 0; i < m_size; ++i)
            std::cout << data[i] << (i < m_size - 1 ? ", " : "\n");
    }

    class Iterator {
        T* ptr;
    public:
        Iterator(T* p) : ptr(p) {}
        T& operator*() const { return *ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() const { return Iterator(data); }
    Iterator end() const { return Iterator(data + m_size); }
};
