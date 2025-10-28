#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class SinglyListContainer {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    size_t m_size;

public:
    SinglyListContainer() : head(nullptr), m_size(0) {}
    ~SinglyListContainer() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) head = newNode;
        else {
            Node* current = head;
            while (current->next) current = current->next;
            current->next = newNode;
        }
        ++m_size;
    }

    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("Invalid index");
        Node* newNode = new Node(value);
        if (pos == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            for (size_t i = 0; i < pos - 1; ++i)
                current = current->next;
            newNode->next = current->next;
            current->next = newNode;
        }
        ++m_size;
    }

    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("Invalid index");
        if (pos == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        } else {
            Node* current = head;
            for (size_t i = 0; i < pos - 1; ++i)
                current = current->next;
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
        }
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
