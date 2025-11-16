#pragma once
#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class SinglyListContainer {
private:
    struct Node {
        T data;
        Node* next;

        explicit Node(const T& val) : data(val), next(nullptr) {}
        explicit Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };

    Node* head = nullptr;
    size_t m_size = 0;

    void clear() noexcept {
        Node* p = head;
        while (p) {
            Node* tmp = p->next;
            delete p;
            p = tmp;
        }
        head = nullptr;
        m_size = 0;
    }

public:
    SinglyListContainer() = default;
    ~SinglyListContainer() { clear(); }

    SinglyListContainer(const SinglyListContainer&) = delete;
    SinglyListContainer& operator=(const SinglyListContainer&) = delete;

    SinglyListContainer(SinglyListContainer&& other) noexcept
        : head(std::move(other.head)), m_size(std::move(other.m_size))
    {
        other.head = nullptr;
        other.m_size = 0;
    }

    SinglyListContainer& operator=(SinglyListContainer&& other) noexcept {
        if (this == &other) return *this;

        clear();

        head = std::move(other.head);
        m_size = std::move(other.m_size);
        other.head = nullptr;
        other.m_size = 0;

        return *this;
    }

    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("position out of range");

        Node* node = new Node(value);
        if (pos == 0) {
            node->next = head;
            head = node;
            ++m_size;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i)
            current = current->next;

        node->next = current->next;
        current->next = node;
        ++m_size;
    }

    void insert(size_t pos, T&& value) {
        if (pos > m_size) throw std::out_of_range("position out of range");

        Node* node = new Node(std::move(value));
        if (pos == 0) {
            node->next = head;
            head = node;
            ++m_size;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i)
            current = current->next;

        node->next = current->next;
        current->next = node;
        ++m_size;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (!head) {
            head = node;
            ++m_size;
            return;
        }

        Node* current = head;
        while (current->next)
            current = current->next;
        current->next = node;
        ++m_size;
    }

    void push_back(T&& value) {
        Node* node = new Node(std::move(value));
        if (!head) {
            head = node;
            ++m_size;
            return;
        }

        Node* current = head;
        while (current->next)
            current = current->next;
        current->next = node;
        ++m_size;
    }

    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("position out of range");

        Node* temp;
        if (pos == 0) {
            temp = head;
            head = head->next;
            delete temp;
            --m_size;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i)
            current = current->next;

        temp = current->next;
        current->next = temp->next;
        delete temp;
        --m_size;
    }

    T& get(size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");

        Node* cur = head;
        for (size_t i = 0; i < index; ++i)
            cur = cur->next;

        return cur->data;
    }

    const T& get(size_t index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");

        Node* cur = head;
        for (size_t i = 0; i < index; ++i)
            cur = cur->next;

        return cur->data;
    }

    T& operator[](size_t index) { return get(index); }
    const T& operator[](size_t index) const { return get(index); }

    size_t size() const noexcept { return m_size; }

    void print() const {
        Node* p = head;
        while (p) {
            std::cout << p->data;
            if (p->next) std::cout << ", ";
            p = p->next;
        }
        std::cout << "\n";
    }

    class Iterator {
        Node* node;
    public:
        explicit Iterator(Node* n) : node(n) {}
        T& operator*() const { return node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};
