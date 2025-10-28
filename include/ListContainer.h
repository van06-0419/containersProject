#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class ListContainer {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t m_size;

public:
    ListContainer() : head(nullptr), tail(nullptr), m_size(0) {}
    ~ListContainer() {
        Node* current = head;
        while (current) {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
    }

    ListContainer(ListContainer&& other) noexcept
        : head(other.head), tail(other.tail), m_size(other.m_size) {
        other.head = other.tail = nullptr;
        other.m_size = 0;
    }

    ListContainer& operator=(ListContainer&& other) noexcept {
        if (this != &other) {
            this->~ListContainer();
            head = other.head;
            tail = other.tail;
            m_size = other.m_size;
            other.head = other.tail = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++m_size;
    }

    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("Invalid index");
        if (pos == m_size) { push_back(value); return; }

        Node* newNode = new Node(value);
        Node* current = head;
        for (size_t i = 0; i < pos; ++i)
            current = current->next;

        newNode->next = current;
        newNode->prev = current->prev;
        if (current->prev) current->prev->next = newNode;
        else head = newNode;
        current->prev = newNode;
        ++m_size;
    }

    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("Invalid index");
        Node* current = head;
        for (size_t i = 0; i < pos; ++i)
            current = current->next;

        if (current->prev) current->prev->next = current->next;
        else head = current->next;
        if (current->next) current->next->prev = current->prev;
        else tail = current->prev;

        delete current;
        --m_size;
    }

    size_t size() const { return m_size; }

    void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << ", ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    class Iterator {
        Node* node;
    public:
        Iterator(Node* n) : node(n) {}
        T& operator*() const { return node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};
