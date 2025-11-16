#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
class SeqContainer {
private:
    T* data;
    size_t size;
    size_t capacity;

	void ensure_capacity() {
		if (size == capacity) {
			size_t new_cap;
			if (capacity == 0) new_cap = 1;
			else new_cap = (size_t)(capacity * 1.5);

			T* new_data = new T[new_cap];
			for (size_t i = 0; i < size; ++i)
				new_data[i] = std::move(data[i]);
			delete[] data;
			data = new_data;
			capacity = new_cap;
		}
	}

    void clear() noexcept {
        data = nullptr;
        size = 0;
        capacity = 0;
    }

public:
    SeqContainer() : data(nullptr), size(0), capacity(0) {}
    ~SeqContainer() { delete[] data; }

    SeqContainer(const SeqContainer&) = delete;
    SeqContainer& operator=(const SeqContainer&) = delete;

    SeqContainer(SeqContainer&& other) noexcept
        : data(std::move(other.data))
        , size(std::move(other.size))
        , capacity(std::move(other.capacity))
    {
        other.clear();
    }

    SeqContainer& operator=(SeqContainer&& other) noexcept {
        if (this == &other) return *this;

        delete[] data;
        data     = std::move(other.data);
        size     = std::move(other.size);
        capacity = std::move(other.capacity);
        other.clear();

        return *this;
    }

    void push_back(const T& value) {
        ensure_capacity();
        data[size] = value;
        ++size;
    }

    void insert(size_t index, const T& value) {
        if (index > size) throw out_of_range("Insert index out of range");
        ensure_capacity();

        for (size_t i = size; i > index; --i)
            data[i] = std::move(data[i - 1]);
        data[index] = value;
        ++size;
    }

    void erase(size_t index) {
        if (index >= size) throw out_of_range("Erase index out of range");

        for (size_t i = index; i < size - 1; ++i)
            data[i] = std::move(data[i + 1]);
        --size;
    }

    size_t get_size() const { return size; }

    T& operator[](size_t index) {
        if (index >= size) throw out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) throw out_of_range("Index out of range");
        return data[index];
    }

    void print() const {
        for (size_t i = 0; i < size; ++i) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << endl;
    }

    class iterator {
        T* ptr;
    public:
        iterator(T* p) : ptr(p) {}
        T& operator*() { return *ptr; }
        iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size); }
};
